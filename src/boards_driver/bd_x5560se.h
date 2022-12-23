#ifndef HEADER_H_BD_DT1260_FRAME
#define HEADER_H_BD_DT1260_FRAME
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

class bd_x5560se : public  SciSDK_Node {
    public:
	    bd_x5560se(SciSDK_HAL *hal, json j, string path);
        NI_RESULT ISetParamI32(string name, uint32_t value);
        NI_RESULT IGetParamI32(string name, uint32_t *value);
    private:
        NI_RESULT UpdateHardware();
    struct {
        bd_reg offset;
    } board_params;
};
#endif 