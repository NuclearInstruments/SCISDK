#include "bd_feelib.h"
#include <functional>
#include <chrono>
#include <thread>

/*
BOARD DEVICE DRIVER FOR FEE-LIB COMPATIBLE DEVICES


Framework Compatible version: 1.0

*/

bd_feelib::bd_feelib(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {
    RegisterParameter("boardapi/**", "router all board api to string access", SciSDK_Paramcb::Type::str, this);
}


NI_RESULT bd_feelib::ISetParamI32(string name, uint32_t value)
{
	
    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_feelib::IGetParamI32(string name, uint32_t *value)
{

    return NI_INVALID_PARAMETER;
}


NI_RESULT bd_feelib::ISetParamString(string name, string value)
{
	cout << "set string " << name << " to " << value << endl;
    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_feelib::IGetParamString(string name, string* value)
{
	cout << "get string " << name << endl;
    return NI_INVALID_PARAMETER;
}
