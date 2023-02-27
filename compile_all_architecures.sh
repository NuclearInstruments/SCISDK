#!/bin/bash

rm -rf build_*
unset CC
unset CXX

autoreconf -if
BUILD="$(echo $MACHTYPE | sed "s/$(echo $MACHTYPE | cut -d- -f2)/cross/")"

if [ "$ARCHITECTURE" = "x64" ]; then
    #compile x64 (native)
    rm -rf build_x64
    mkdir -p build_x64
    cd build_x64
    ../configure --prefix=$(pwd)/output CFLAGS="-O3" CXXFLAGS="-O3"
    make -j4 && make install
    cd output
    fpm \
    -s dir -t deb \
    -p ../../debs/scisdk-$DOCKER_IMAGE-x64.deb \
    --name scisdk \
    --license agpl3 \
    --version $CI_COMMIT_TAG \
    --architecture x86_64 \
    --description "Nuclear Instruments SciCompiler SDK" \
    --url "https://github.com/NuclearInstruments/SCISDK" \
    --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
    .    
    tar zcvf ../../tars/scisdk-$DOCKER_IMAGE-x64.tar.gz .
    cd ../..
elif [ "$ARCHITECTURE" = "i386" ]; then
    #compile x86 
    rm -rf build_i386
    mkdir -p build_i386
    cd build_i386
    ../configure --host=i686-linux-gnu CFLAGS="-m32 -O3" CXXFLAGS="-m32 -O3" LDFLAGS=-m32 --prefix=$(pwd)/output
    make -j4 && make install
    cd output
    fpm \
    -s dir -t deb \
    -p ../../debs/scisdk-$DOCKER_IMAGE-i386.deb \
    --name scisdk \
    --license agpl3 \
    --version $CI_COMMIT_TAG \
    --architecture i386 \
    --description "Nuclear Instruments SciCompiler SDK" \
    --url "https://github.com/NuclearInstruments/SCISDK" \
    --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
    .    
    tar zcvf ../../tars/scisdk-$DOCKER_IMAGE-i386.tar.gz .
    cd ../..
elif [ "$ARCHITECTURE" = "arm64" ]; then
    #compile arm64
    rm -rf build_arm64
    mkdir -p build_arm64
    cd build_arm64
    export CC=aarch64-linux-gnu-gcc
    export CXX=aarch64-linux-gnu-g++
    ../configure --host=aarch64-linux-gnu --prefix=$(pwd)/output --build=$BUILD CFLAGS="-O3" CXXFLAGS="-O3"
    make -j4 && make install
    cd output
    fpm \
    -s dir -t deb \
    -p ../../debs/scisdk-$DOCKER_IMAGE-arm64.deb \
    --name scisdk \
    --license agpl3 \
    --version $CI_COMMIT_TAG \
    --architecture arm64 \
    --description "Nuclear Instruments SciCompiler SDK" \
    --url "https://github.com/NuclearInstruments/SCISDK" \
    --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
    .    
    tar zcvf ../../tars/scisdk-$DOCKER_IMAGE-arm64.tar.gz .
    cd ../..
elif [ "$ARCHITECTURE" = "armhf" ]; then
    #compile armhf
    rm -rf build_armhf
    mkdir -p build_armhf
    cd build_armhf
    export CC=arm-linux-gnueabihf-gcc
    export CXX=arm-linux-gnueabihf-g++
    ../configure --host=arm-linux-gnueabihf --prefix=$(pwd)/output --build=$BUILD CFLAGS="-O3" CXXFLAGS="-O3"
    make -j4 && make install
    cd output
    fpm \
    -s dir -t deb \
    -p ../../debs/scisdk-$DOCKER_IMAGE-armhf.deb \
    --name scisdk \
    --license agpl3 \
    --version $CI_COMMIT_TAG \
    --architecture armhf \
    --description "Nuclear Instruments SciCompiler SDK" \
    --url "https://github.com/NuclearInstruments/SCISDK" \
    --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
    .    
    tar zcvf ../../tars/scisdk-$DOCKER_IMAGE-armhf.tar.gz .
    cd ../..
else
    echo "Invalid architecture specified."
    exit 1
fi

