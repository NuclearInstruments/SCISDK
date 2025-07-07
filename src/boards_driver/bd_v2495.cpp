#include "bd_v2495.h"
#include <functional>
#include <chrono>
#include <thread>

/*
BOARD DEVICE DRIVER FOR V2740


Framework Compatible version: 1.0

*/

bd_v2495::bd_v2495(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {
    

    cout << "Registering V2495 endpoints" << endl;
    for (int i = 0; i < 32; i++) {
        RegisterParameter("boardapi/gd[" + std::to_string(i) + "].delay", "set gate&delay delay", SciSDK_Paramcb::Type::I32, this);
        RegisterParameter("boardapi/gd[" + std::to_string(i) + "].gate", "set gate&delay gate", SciSDK_Paramcb::Type::I32, this);
        RegisterParameter("boardapi/gd[" + std::to_string(i) + "].fine", "set gate&delay fine", SciSDK_Paramcb::Type::I32, this);
		board_params.gd[i].delay = 0;
		board_params.gd[i].gate = 0;
		board_params.gd[i].fine = 0;
		board_params.gd[i].enable = true;
    }

}


NI_RESULT bd_v2495::ISetParamI32(string name, int32_t value)
{
    for (int i = 0; i < 32; i++) {
        if (name == "gd[" + std::to_string(i) + "].delay") {
            if (value > 0xFFFF) {
                return NI_PARAMETER_OUT_OF_RANGE;
            }
			board_params.gd[i].delay = value;
			_hal->SetBoardParamater("gd:" + std::to_string(i), 
                std::to_string(board_params.gd[i].delay)+","+ 
                std::to_string(board_params.gd[i].gate) + "," + 
                std::to_string(board_params.gd[i].fine));
            return NI_OK;
        }
        if (name == "gd[" + std::to_string(i) + "].gate") {
            if (value > 0xFFFF) {
                return NI_PARAMETER_OUT_OF_RANGE;
            }
            _hal->SetBoardParamater("gd:" + std::to_string(i),
                std::to_string(board_params.gd[i].delay) + "," +
                std::to_string(board_params.gd[i].gate) + "," +
                std::to_string(board_params.gd[i].fine));

            return NI_OK;
        }
        if (name == "gd[" + std::to_string(i) + "].fine") {
			if (value > 0xFF) {
				return NI_PARAMETER_OUT_OF_RANGE;
			}
            _hal->SetBoardParamater("gd:" + std::to_string(i),
                std::to_string(board_params.gd[i].delay) + "," +
                std::to_string(board_params.gd[i].gate) + "," +
                std::to_string(board_params.gd[i].fine));

            return NI_OK;
        }
    }
    
    
    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_v2495::IGetParamI32(string name, int32_t *value)
{
    for (int i = 0; i < 32; i++) {
        if (name == "gd[" + std::to_string(i) + "].delay") {
			*value = board_params.gd[i].delay;
        }
        if (name == "gd[" + std::to_string(i) + "].gate") {
			*value = board_params.gd[i].gate;
        }
        if (name == "gd[" + std::to_string(i) + "].fine") {
			*value = board_params.gd[i].fine;
        }
    }
    return NI_INVALID_PARAMETER;
}



NI_RESULT bd_v2495::UpdateHardware()
{
    return NI_OK;

}