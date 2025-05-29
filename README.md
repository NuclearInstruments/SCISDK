#  SciSDK Library Introduction 
<b> The Software Development kit for SciCompiler User Firmware </b>


The SciSDK is a cross-platform library to write custom control and readout software for SciCompiler generated user firmware.

SciCompiler is a Nuclear Instruments (http://www.nuclearinstruments.eu) tool to generate FPGA firmware con NI/Caen OpenHardware FPGA (https://www.caen.it/families/open-fpga-digitizers/)


# Documentation

<b>You can access to the SciSDK User Guide website (https://nuclearinstruments.github.io/SCISDK)</b>


# Installation on Windows

Download last setup from [Github releases](https://github.com/NuclearInstruments/SCISDK/releases) and install it in your system. The libraries for all supported board and the SciSDK library will be installed in your system.

# Installation on Linux from source

** SciSDK does not include the low levels so library required to communicate with the board **

You must prior install the low level libraries and then compile and install the libSCISDK.


## Install board specific low level library


The list of the repository is the following:
- [SCIDK](https://github.com/NuclearInstruments/SCIDK-SDKLinux/)
- [R5560/R5560SE/DT5560](https://github.com/NuclearInstruments/r5560_sdk)
- [DT5550/DT5550W](https://github.com/NuclearInstruments/DT5550Wgcc)
- [V/DT274X/FELib](https://www.caen.it/products/caen-felib-library/)


# Verify installed version of the library [Windows]
In the bin\x64 folder or bin\Win32 folder you can find the executable file "SciSDK_DLL_Info.exe" that prints the version of the library installed in your system.

The tool will print the path of the library and the version of the library.

Execute this file in the folder you want to use the library (for example the folder where you are compiling your application ) and run it.

It will display the information of the library that will be used by your application.

This tool will also print the board low level library path.

# Verify installed version of the library [linux]

In folder tools/src you can find the source code of the tool "SciSDK_DLL_Info.cpp" that prints the version of the library installed in your system.

g++ SciSDK_DLL_Info.cpp -o scisdk_info -ldl

Execute this file in the folder you want to use the library (for example the folder where you are compiling your application ) and run it.

It will display the information of the library that will be used by your application.


### Example: How to compiler low level library for DT1260

Update your system:

```
sudo apt update
sudo apt install build-essential
```

Download and install FTDI drivers:
```
wget https://ftdichip.com/wp-content/uploads/2022/07/libftd2xx-x86_64-1.4.27.tgz
tar xvf libftd2xx-x86_64-1.4.27.tgz
sudo cp  ./release/build/libftd2xx.so.1.4.27 /usr/local/lib/.
cd /usr/local/lib
sudo ln -s libftd2xx.so.1.4.27 libftd2xx.so 
sudo chmod 0755 libftd2xx.so.1.4.27
```

Change udev permission and install DT1260 low level library in the system
```
cd
git clone https://github.com/NuclearInstruments/SCIDK-SDKLinux.git
cd SCIDK-SDKLinux/
cd udev
sudo cp *.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules 
sudo udevadm trigger
cd ..
make
sudo make install
```


## Compile and install SciSDK in your system


In order to install on Ubuntu the library:


Update your system:

```
sudo apt update
sudo apt install build-essential
sudo apt-get install autoconf libtool
```

Download and install library:

```
cd
git clone https://github.com/NuclearInstruments/SCISDK.git
cd SCISDK
autoreconf  -f -i -Wall,no-obsolete
mkdir -p linuxbuild
cd linuxbuild
../configure
make -j16
sudo make install
```


check that /usr/local/lib is in your LD_LIBRARY_PATH.
``` bash
echo $LD_LIBRARY_PATH
```

if not you can add it simply (temporany)

``` bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
```

or permanent (you must log out and login again to apply it)

``` bash
nano ~/.profile
LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
```

# Install SciSDK from apt repository on Ubuntu

You can install SciSDK from the apt repository on Ubuntu by adding the Nuclear Instruments repository to your system.
```bash
sudo mkdir -p /etc/apt/{sources.list.d,keyrings} && \
sudo apt update && \
sudo apt install -y ca-certificates curl gnupg lsb-release && \
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/nuclearinstruments.gpg] http://packages.nuclearinstruments.eu/linux/apt/ubuntu/ $(lsb_release -cs) main" \
  | sudo tee /etc/apt/sources.list.d/nuclearinstruments.list > /dev/null && \
curl -fsSL http://packages.nuclearinstruments.eu/linux/apt/ubuntu/pubkey.gpg \
  | sudo gpg --dearmor -o /etc/apt/keyrings/nuclearinstruments.gpg && \
sudo apt update && \
sudo apt install -y scisdk
```