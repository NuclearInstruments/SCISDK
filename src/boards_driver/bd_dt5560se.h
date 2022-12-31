#ifndef HEADER_H_BD_DT5560SE
#define HEADER_H_BD_DT5560SE
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

class bd_dt5560se : public  SciSDK_Node {
    public:
	  bd_dt5560se(SciSDK_HAL *hal, json j, string path);
        NI_RESULT ISetParamI32(string name, int32_t value);
        NI_RESULT IGetParamI32(string name, int32_t *value);
        NI_RESULT ISetParamString(string name, string value);
        NI_RESULT IGetParamString(string name, string* value);
    private:

        enum class COUPLING  {
            DC,
            US1,
            US10,
            US30
        } ;

        enum class SyncType {
            LEMO0,
            LEMO1,
            LEMO2,
            SYNC0,
            SYNC1,
            SYNC2,
            CLK,
            REG0,
            REG1,
            REG2,
            INVALID
        };
        SyncType stringToSyncType(const std::string& str);
        std::string syncTypeToString(bd_dt5560se::SyncType t);
            
        struct {
            int32_t offset_mV[32];
			uint32_t gain[16];
            bool div2[16];
			bool r50[16];
			COUPLING coupling[2];
        }analog_settings;

        struct {
            SyncType syncsource[3];
            SyncType daqsyncsource[3];
            SyncType lemooutsource[3];

        }sync;

};
#endif 