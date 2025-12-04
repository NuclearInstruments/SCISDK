#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

MAX_RETRIES=5
SLEEP_SECONDS=10

# Ensure all the variables from catalog are defined
[ -z OS_NAME ] && echo "Export OS_NAME environment variable" && exit 1
[ -z LINUXRELEASE ] && echo "Export LINUXRELEASE environment variable" && exit 1
[ -z ARCHITECTURE ] && echo "Export ARCHITECTURE environment variable" && exit 1

# Ensure variables from CI pipeline are defined
[ -z NI_PACKAGE_REGISTRY_URL ] && echo "Export NI_PACKAGE_REGISTRY_URL environment variable" && exit 1
[ -z NI_PACKAGE_REGISTRY_TOKEN ] && echo "Export NI_PACKAGE_REGISTRY_TOKEN environment variable" && exit 1
[ -z CI_COMMIT_TAG ] && echo "This script should be run in a tagged pipeline." && exit 1


attempt=1

while [ $attempt -le $MAX_RETRIES ]; do
    echo "Upload attempt $attempt/$MAX_RETRIES..."

    response=$( \
        curl -X POST \
        --location "${NI_PACKAGE_REGISTRY_URL}/${OS_NAME}/upload" \
        --header "Authorization: ${NI_PACKAGE_REGISTRY_TOKEN}" \
        --form "file=@./debs/scisdk_${CI_COMMIT_TAG}_${LINUXRELEASE}_${ARCHITECTURE}.deb" \
        --form "distribution=${LINUXRELEASE}" \
        -s -w "\n%{http_code}" \
    )
    http_code=$(tail -n1 <<< "$response")
    echo "HTTP code: $http_code"

    if [ "$http_code" = "200" ]; then
        echo "Upload succeeded."
        break
    fi

    if [ $attempt -lt $MAX_RETRIES ]; then
        echo "Upload failed. Retrying in $SLEEP_SECONDS seconds..."
        sleep $SLEEP_SECONDS
    fi

    attempt=$((attempt + 1))
done

if [ "$http_code" != "200" ]; then
    echo "Upload failed after $MAX_RETRIES attempts."
    exit 1
fi