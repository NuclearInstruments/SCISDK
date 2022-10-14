#include "bd_dt1260.h"
#include <functional>
#include <chrono>
#include <thread>

/*
BOARD DEVICE DRIVER FOR DT1260


Framework Compatible version: 1.0

*/

bd_dt1260::bd_dt1260(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {
    board_params.offset.value = 2048;
    board_params.offset.address = 4294967289;

    RegisterParameter("analog_offset", "set analog offset", SciSDK_Paramcb::Type::U32, this);
}


