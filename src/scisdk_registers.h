#ifndef HEADER_H_SCISDK_REGISTERS
#define HEADER_H_SCISDK_REGISTERS

#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>

using json = nlohmann::json;

using namespace std;

class SciSDK_Registers : public  SciSDK_Node {
public:
	SciSDK_Registers(SciSDK_HAL *hal, json j, string path);

	NI_RESULT ISetParamU32(string name, uint32_t value);
	//NI_RESULT ISetParamI32(string name, int32_t value);
	//NI_RESULT ISetParamString(string name, string value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
	//NI_RESULT IGetParamI32(string name, int32_t *value);
	//NI_RESULT IGetParamString(string name, string *value);

	//NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer);
	//NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void **buffer);

	//NI_RESULT ReadData(void *buffer);

	//NI_RESULT ExecuteCommand(string cmd, string param);
	//NI_RESULT ReadStatus(void *buffer);
private:
	struct {
		uint32_t base;
	}address;

	uint32_t reg_count;//number of available registers

};
#endif