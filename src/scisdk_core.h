#ifndef HEADER_H_SCISDK
#define HEADER_H_SCISDK

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>

#include "scisdk_defines.h"
#include "scisdk_device.h"
#include "NIErrorCode.h"
#include "scisdk_version.h"


using namespace std;



class SciSDK {

public:
	SciSDK();
	~SciSDK();

	NI_RESULT AddNewDevice(string DevicePath, string DeviceModel, string JSONFwFilePath, string Name);
	NI_RESULT DetachDevice(string Name);
	NI_RESULT SetRegister(string Path, uint32_t value);
	NI_RESULT GetRegister(string Path, uint32_t *value);
	NI_RESULT StrobeRegister(string Path, string strobe_polarity);
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
	NI_RESULT SetParameter(string Path, string value);
	NI_RESULT GetParameter(string Path, string *value);
	NI_RESULT ExecuteCommand(string Path, string parameter);
	NI_RESULT AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer, int size);
	NI_RESULT FreeBuffer(string Path, T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT ReadData(string Path, void *buffer);
	NI_RESULT WriteData(string Path, void* buffer);
	NI_RESULT DecodeData(string Path, void *buffer_in, void *buffer_out);
	NI_RESULT ReadStatus(string Path, void *buffer);
	NI_RESULT GetComponentList(string name, string Type, string *res, bool return_json);
	NI_RESULT GetAllParameters(string path, string* ret);
	NI_RESULT GetParameterDescription(string path, string* ret);
	NI_RESULT GetParameterListOfValues(string path, string* ret);
	NI_RESULT GetParameterMinimumValue(string path, double* ret);
	NI_RESULT GetParameterMaximumValue(string path, double* ret);
	NI_RESULT GetParametersProperties(string path, string* ret);
	NI_RESULT GetAttachedDevicesList(string* devices);
	NI_RESULT GetLibraryVersion(string* ret);

	string s_error(int err_no);
	void p_error(int err_no);
	void p_error(string fnc, int err_no);
	void p_error(int fnc, int err_no);
	void p_error(string file, int line, int err_no);
private:
	list<SciSDK_Device *> devs;

	SciSDK_Device * FindDeviceByName(string Name);
	std::vector<std::string> SplitPath(string path, char separator);
	NI_RESULT LocateDevice(string path, SciSDK_Device **dev, string *subpath);
};
#endif