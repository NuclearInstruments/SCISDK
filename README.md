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
