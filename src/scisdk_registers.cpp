#include "scisdk_registers.h"

SciSDK_Registers::SciSDK_Registers(SciSDK_HAL * hal, json j, string path) : SciSDK_Node(hal, j, path) {
	address.base = (uint32_t)j.at("Address");
	properties.reg_count = j.at("Registers").size();

	for (int i = 0; i < properties.reg_count; i++) {
		RegisterParameter("register_" + to_string(i), "value stored inside the register on device", SciSDK_Paramcb::Type::U32, this);
	}
}

NI_RESULT SciSDK_Registers::ISetParamU32(string name, uint32_t value) {
	for (int i = 0; i < properties.reg_count; i++) {
		if (name == "register_" + to_string(i)) {
			int res = _hal->WriteReg(value, address.base + 1 + i);
			if (res) {
				return NI_ERROR_INTERFACE;
			}
			return NI_OK;
		}
	}

	return NI_SPECIFIC_ERROR;
}

NI_RESULT SciSDK_Registers::IGetParamU32(string name, uint32_t * value) {
	for (int i = 0; i < properties.reg_count; i++) {
		if (name == "register_" + to_string(i)) {
			int res = _hal->ReadReg(value, address.base + 1 + i);
			if (res) {
				return NI_ERROR_INTERFACE;
			}
			return NI_OK;
		}
	}

	return NI_SPECIFIC_ERROR;
}