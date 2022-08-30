#include "scisdk_registers.h"

SciSDK_Registers::SciSDK_Registers(SciSDK_HAL * hal, json j, string path) : SciSDK_Node(hal, j, path) {
	address.base = (uint32_t)j.at("Address");
}

NI_RESULT SciSDK_Registers::ISetParamU32(string name, uint32_t value)
{
	return NI_OK;
}

NI_RESULT SciSDK_Registers::IGetParamU32(string name, uint32_t * value)
{
	return NI_OK;
}
