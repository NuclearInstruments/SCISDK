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
	NI_RESULT CloseConnection();

	NI_RESULT SetParameter(string Path, string value);
	NI_RESULT GetParameter(string Path, string *value);
	NI_RESULT SetParameter(string Path, uint32_t value);
	NI_RESULT GetParameter(string Path, uint32_t *value);
	NI_RESULT SetParameter(string Path, int32_t value);
	NI_RESULT GetParameter(string Path, int32_t *value);
	NI_RESULT SetParameter(string Path, uint64_t value);
	NI_RESULT GetParameter(string Path, uint64_t *value);
	NI_RESULT SetParameter(string Path, int64_t value);
	NI_RESULT GetParameter(string Path, int64_t *value);
	NI_RESULT SetParameter(string Path, double value);
	NI_RESULT GetParameter(string Path, double *value);
	NI_RESULT SetRegister(string Path, uint32_t value);
	NI_RESULT GetRegister(string Path, uint32_t *value);

	NI_RESULT ExecuteCommand(string Path);

	NI_RESULT AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer, int size);
	NI_RESULT FreeBuffer(string Path, T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT ReadData(string Path, void *buffer);
	NI_RESULT ReadStatus(string Path, void *buffer);


	NI_RESULT GetComponentList(string Type, string *res, bool return_json);
	NI_RESULT GetAllParameters(string Path, string *ret);
	NI_RESULT GetParameterDescription(string Path, string* ret);
	NI_RESULT GetParameterListOfValues(string Path, string* ret);
	NI_RESULT GetParameterMinimumValue(string Path, double *ret);
	NI_RESULT GetParameterMaximumValue(string Path, double *ret);

	NI_RESULT GetParametersProperties(string Path, string* ret);

	string GetName() { return _Name;  };
	string GetModel() { return _DeviceModel; };
	string GetPath() { return _DevicePath; };
private:
	std::vector<std::string> SplitPath(string path, char separator);
	NI_RESULT FindElement(string Path, SciElement *param);
	NI_RESULT BuildTree(json rs, string parent);
	NI_RESULT LocateParameter(string Path, string *name, SciSDK_Node **node);
	SciSDK_Node * FindMMC(string Path);
	SciSDK_Node* FindBoardApi(string Path);
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