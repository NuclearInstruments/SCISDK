#include "bd_x5560se.h"
#include <functional>
#include <chrono>
#include <thread>

/*
BOARD DEVICE DRIVER FOR DT1260


Framework Compatible version: 1.0

*/

bd_x5560se::bd_x5560se(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {
    board_params.offset.value = 2048;
    board_params.offset.address = 4294967289;

    RegisterParameter("analog_offset", "set analog offset", SciSDK_Paramcb::Type::U32, this);
}


NI_RESULT bd_x5560se::ISetParamI32(string name, uint32_t value)
{
	if (name == "analog_offset") {
        if ((value > 4095) || (value<0)) {
            return NI_PARAMETER_OUT_OF_RANGE;
        } else {
            board_params.offset.value = value;
            return NI_OK;
        }
    }

    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_x5560se::IGetParamI32(string name, uint32_t *value)
{
    if (name == "analog_offset") {
        *value = board_params.offset.value;
        return NI_OK;
    }

    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_x5560se::UpdateHardware()
{
    int ret = _hal->WriteReg(board_params.offset.value, board_params.offset.address);
		
    if (ret) {
        return NI_ERROR_INTERFACE;
    } else
    return NI_OK;

}