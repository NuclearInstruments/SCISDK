#ifndef HEADER_H_SCISDK_REGISTERS
#define HEADER_H_SCISDK_REGISTERS

#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>

using json = nlohmann::json;

using namespace std;

class SciSDK_Registers : public  SciSDK_Node {
public:
	SciSDK_Registers(SciSDK_HAL *hal, json j, string path);

	NI_RESULT ISetParamU32(string name, uint32_t value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
private:
	class reg_elem{
		public:
		string name;
		uint32_t offset;
		reg_elem(string _name, uint32_t _offset) {
			name = _name;
			offset = _offset;
		}

	} ;
	struct {
		uint32_t base;//base address of registers
	}address;

	struct {
		uint32_t reg_count;//number of available registers
		list<reg_elem> registers;
	}properties;
};
#endif