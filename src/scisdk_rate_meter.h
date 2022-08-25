#ifndef HEADER_H_SCISDK_RATEMETER
#define HEADER_H_SCISDK_RATEMETER
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>
#include <thread>  
#include <queue>
#include <atomic> 
using json = nlohmann::json;

using namespace std;


class SciSDK_Ratemeter : public  SciSDK_Node {
public:
	SciSDK_Ratemeter(SciSDK_HAL *hal, json j, string path);

	NI_RESULT ISetParamU32(string name, uint32_t value);
	NI_RESULT ISetParamI32(string name, int32_t value);
	NI_RESULT ISetParamString(string name, string value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
	NI_RESULT IGetParamI32(string name, int32_t *value);
	NI_RESULT IGetParamString(string name, string *value);

	NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer, int size);
	NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void **buffer);

	NI_RESULT ReadData(void *buffer);

	NI_RESULT ExecuteCommand(string cmd, string param);
	NI_RESULT ReadStatus(void *buffer);

private:
	uint32_t address;

	struct {
		uint32_t nchannels;
	}settings;

};

#endif