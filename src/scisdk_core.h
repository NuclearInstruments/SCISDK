#ifndef HEADER_H_SCISDK
#define HEADER_H_SCISDK

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>

#include "scisdk_device.h"
#include "NIErrorCode.h"


using namespace std;



class SciSDK {

public:
	SciSDK();
	~SciSDK();

	NI_RESULT AddNewDevice(string DevicePath, string DeviceModel, string JSONFwFilePath, string Name);
	NI_RESULT DetachDevice(string Name);


private:
	list<SciSDK_Device *> devs;
	
	SciSDK_Device * FindDeviceByName(string Name);
	
};
#endif