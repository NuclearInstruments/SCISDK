#ifndef HEADER_H_SCISDK_DEVICE   
#define HEADER_H_SCISDK_DEVICE

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include "NIErrorCode.h"
#include "scisdk_SciElement.h"
#include "scisdk_hal.h"
#include "scisdk_scinode.h"

using namespace std;
#include "json.hpp"


using json = nlohmann::json;


class SciSDK_Device {

public:
	SciSDK_Device(string DevicePath, string DeviceModel, string JSONFwFilePath, string Name);
	~SciSDK_Device();

	NI_RESULT Connect();

	NI_RESULT SetParameter(string Path, string value);
	NI_RESULT GetParameter(string Path, string value);
	NI_RESULT ExecuteCommand(string Path);

	string GetName() { return _Name;  };

private:
	std::vector<std::string> SplitPath(string path, char separator);
	NI_RESULT FindElement(string Path, SciElement *param);
	NI_RESULT BuildTree(json rs, string parent);

	std::vector<SciSDK_Node *> mmcs;

	string _Name;
	string _DeviceModel;
	string _DevicePath;
	string _JSONFwFilePath;
	json jcfg;

	SciSDK_HAL *_hal;
	bool connected;

};	
// Code placed here is included only once per translation unit
#endif 