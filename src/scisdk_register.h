#ifndef HEADER_H_SCISDK_REGISTER
#define HEADER_H_SCISDK_REGISTER
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"


using json = nlohmann::json;

using namespace std;

class SciSDK_Register : public  SciSDK_Node {
public:
	SciSDK_Register(SciSDK_HAL *hal, json j, string path): SciSDK_Node(hal, j, path)
	{
		address = (uint32_t)j.at("Address");
	}
	
	NI_RESULT SetValueU32(uint32_t value) override;

	NI_RESULT GetValueU32(uint32_t *value) override;

	uint32_t address;
private:

	

};
#endif 