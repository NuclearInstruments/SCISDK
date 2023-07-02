pip install -i http://pypi-ni.lanni/simple --trusted-host pypi-ni.lanni nirelease

ROOT=$(pwd)
cd /tmp/
python3 -m nirelease --pull -u R5560SE/rev-B-A/sub/fw-linux-sdk/r5560se-daq-sdk/
cd r5560se-daq-sdk
chmod +x sdk.sh
./sdk.sh -y -d /tmp/sdk
cd /tmp/sdk
source  /tmp/sdk/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi

cd $ROOT
autoreconf -if
mkdir build_zynq
cd build_zynq
../configure --host=arm-xilinx-linux-gnueabi --prefix=$(pwd)/output/ CFLAGS="-O3" CXXFLAGS="-O3"
make -j4 
make install
cd output
tar cvf ../scisdk-zynq.tar.gz .



