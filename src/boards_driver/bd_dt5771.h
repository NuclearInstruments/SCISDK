#ifndef HEADER_H_BD_DT5771
#define HEADER_H_BD_DT5771
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

class bd_dt5771 : public  SciSDK_Node {
public:
    bd_dt5771(SciSDK_HAL* hal, json j, string path);
    NI_RESULT ISetParamI32(string name, int32_t value);
    NI_RESULT IGetParamI32(string name, int32_t* value);
    NI_RESULT ISetParamString(string name, string value);
    NI_RESULT IGetParamString(string name, string* value);
    NI_RESULT ISetParamDouble(string name, double value);
    NI_RESULT IGetParamDouble(string name, double*value);
    NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size);
    NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void** buffer);
    NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void** buffer);
    NI_RESULT ReadData(void* buffer);
private:
    NI_RESULT UpdateHardware();

    enum class COUPLING {
        DC      = 0,
        NS300   = 1,
        US1     = 2,
        US10    = 3,
        FLOAT   = 4
    };

    enum class BANDWIDTH {
        FULL = 0,
        MHz20 = 1,
        MHz100 = 2,
        MHz200 = 3,
        MHz350 = 4,
        MHz650 = 5,
        MHz750 = 6
    };

    int findIndexInList(const std::list<std::string>& lst, const std::string& value);

    const std::list<std::string> listOfBw = { "full", "20mhz", "100mhz", "200mhz", "350mhz", "650mhz", "750mhz" };
    const std::list<std::string> listOfCoupling = { "dc", "300ns", "1us", "10us", "floating" };
    const std::list<std::string> listOfBoolean = { "false", "true" };
    const std::list<std::string> listOfRange = { "2v", "10v" };
    const std::list<std::string> listOfImpedance = { "1k", "50r" };
    const std::list<std::string> listOfDigitalImpedance = { "hi", "50r" };

    uint32_t dgtz_timeout;

    struct {
        int32_t offset_mV;
        uint32_t gain;
        bool div;
        bool r50;
        COUPLING coupling;
        BANDWIDTH bw;
    }analog_settings[1];

    struct {
        bool r50;
    }dlemo[2];
};
#endif 