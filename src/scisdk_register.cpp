#include "scisdk_register.h"

NI_RESULT SciSDK_Register::SetValueU32(uint32_t value) {
	NI_RESULT res = _hal->WriteReg(value, address);
	return res;
}

NI_RESULT SciSDK_Register::GetValueU32(uint32_t *value) {
	uint32_t v;
	NI_RESULT res = _hal->ReadReg(value, address);
	return res;

}