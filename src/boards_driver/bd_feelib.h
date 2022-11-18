#ifndef HEADER_H_BD_FEELIB_FRAME
#define HEADER_H_BD_FEELIB_FRAME
#include <iostream>
#include <algorithm>
#include <list>
#include "../scisdk_scinode.h"
#include "bd_common.h"

#include <mutex>
#include <thread>  
#include <queue>
#include <atomic> 
using json = nlohmann::json;

using namespace std;

enum class FEELIB_DATATYPE {
    SCOPE,
	DPP
} ;

class bd_feelib : public  SciSDK_Node {
    public:
	    bd_feelib(SciSDK_HAL *hal, json j, string path);
        NI_RESULT ISetParamI32(string name, uint32_t value);
        NI_RESULT IGetParamI32(string name, uint32_t *value);
        NI_RESULT ISetParamString(string name, string value);
        NI_RESULT IGetParamString(string name, string* value);
        NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size);
        NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void** buffer);
        NI_RESULT ExecuteCommand(string cmd, string param);
        NI_RESULT ReadData(void* buffer);
    private:
        int ExtractRootPath(string fullpath, string& rootpath, string& board_path);
        int ConfigureEndpoint();
		FEELIB_DATATYPE _datatype = FEELIB_DATATYPE::SCOPE;

        int acquisition_timeout_ms=10000;
};
#endif 