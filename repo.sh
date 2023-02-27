#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

mkdir -p /etc/apt/sources.list.d
mkdir -p /etc/apt/keyrings
apt-get update
apt-get install -y ca-certificates curl gnupg lsb-release 
cat > /tmp/packages.nuclearinstruments.eu.list << EOF
deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/nuclearinstruments.gpg] http://packages.nuclearinstruments.eu/linux/apt/ubuntu/ $(lsb_release -cs) main

EOF

mv /tmp/packages.nuclearinstruments.eu.list /etc/apt/sources.list.d/
curl -fsSL http://packages.nuclearinstruments.eu/linux/apt/ubuntu/pubkey.gpg | sudo gpg --dearmor -o /etc/apt/keyrings/nuclearinstruments.gpg

apt-get update
