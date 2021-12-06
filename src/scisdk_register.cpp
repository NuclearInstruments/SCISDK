#include "scisdk_register.h"

NI_RESULT SciSDK_Register::SetValueU32(uint32_t value) {
	uint32_t reg_address=0;
	/*_elem.getAddress(&reg_address);*/
	cout << "set register " << reg_address << " -> " << value << endl;
	NI_RESULT res = _hal->WriteReg(value, reg_address);
	return res;
}

NI_RESULT SciSDK_Register::GetValueU23(uint32_t *value) {
	uint32_t reg_address=0;
	uint32_t v;
	/*_elem.getAddress(&reg_address);
	*/
	NI_RESULT res = _hal->ReadReg(value, reg_address);
	cout << "get register " << reg_address << " -> " << *value << endl;

	return res;

}