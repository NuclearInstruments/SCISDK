#ifndef HEADER_H_bd_v2495_FRAME
#define HEADER_H_bd_v2495_FRAME
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

class bd_v2495 : public  SciSDK_Node {
    public:
	    bd_v2495(SciSDK_HAL *hal, json j, string path);
        NI_RESULT ISetParamI32(string name, int32_t value);
        NI_RESULT IGetParamI32(string name, int32_t *value);
    private:
        NI_RESULT UpdateHardware();

        struct {
            struct {
                uint32_t delay;
                uint32_t gate;
                uint32_t fine;
                bool enable;

            } gd[32];
        } board_params;
};
#endif 