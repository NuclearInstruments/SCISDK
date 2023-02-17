
#!/bin/bash

rm -rf build_*
unset CC
unset CXX

autoreconf -if
BUILD="$(echo $MACHTYPE | sed "s/$(echo $MACHTYPE | cut -d- -f2)/cross/")"

#compile x64 (native)
rm -rf build_x64
mkdir -p build_x64
cd build_x64
../configure --prefix=$(pwd)/output
make -j16 && make install
cd ..

#compile x86 
rm -rf build_i386
mkdir -p build_i386
cd build_i386
../configure --host=i686-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32 --prefix=$(pwd)/output
make -j16 && make install
cd ..


#compile arm64
rm -rf build_arm64
mkdir -p build_arm64
cd build_arm64
export CC=aarch64-linux-gnu-gcc
export CXX=aarch64-linux-gnu-g++
../configure --host=aarch64-linux-gnu --prefix=$(pwd)/output --build=$BUILD
make -j16 && make install
cd ..

#compile armhf
rm -rf build_armhf
mkdir -p build_armhf
cd build_armhf
export CC=aarm-linux-gnueabihf-gcc
export CXX=arm-linux-gnueabihf-g++
../configure --host=arm-linux-gnueabihf --prefix=$(pwd)/output --build=$BUILD
make -j16 && make install
cd ..

