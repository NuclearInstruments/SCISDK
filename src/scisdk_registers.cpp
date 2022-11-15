#include "scisdk_registers.h"

SciSDK_Registers::SciSDK_Registers(SciSDK_HAL * hal, json j, string path) : SciSDK_Node(hal, j, path) {
	address.base = (uint32_t)j.at("Address");
	properties.reg_count = j.at("Registers").size();

	for (auto r : j.at("Registers")) {
		//cout << r["Name"].get<string>() << " " << r["Address"].get<uint32_t>() << endl;
		RegisterParameter(r["Name"].get<string>(), "value stored inside the register on device", SciSDK_Paramcb::Type::U32, this);
		properties.registers.push_back(reg_elem(r["Name"].get<string>(), r["Address"].get<uint32_t>()));
	}
}

NI_RESULT SciSDK_Registers::ISetParamU32(string name, uint32_t value) {
	//cout << name << " --> " << value;
	for (auto r :properties.registers) {
		if (r.name == name){
			int res = _hal->WriteReg(value, r.offset);
			if (res) {
				return NI_ERROR_INTERFACE;
			}
			return NI_OK;
		}
	}
	

	return NI_SPECIFIC_ERROR;
}

NI_RESULT SciSDK_Registers::IGetParamU32(string name, uint32_t * value) {
	for (auto r :properties.registers) {
		if (r.name == name){
			int res =  _hal->ReadReg(value, r.offset); 
			if (res) {
				return NI_ERROR_INTERFACE;
			}
			return NI_OK;
		}
	}


	return NI_SPECIFIC_ERROR;
}