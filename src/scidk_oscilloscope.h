#ifndef HEADER_H_SCISDK_OSCILLOSCOPE
#define HEADER_H_SCISDK_OSCILLOSCOPE
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"


using json = nlohmann::json;

using namespace std;

class SciSDK_Oscilloscope : public  SciSDK_Node {
public:
	SciSDK_Oscilloscope(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
	{
		address = (uint32_t)j.at("Address");
		cout << "Oscilloscope: " << name << " addr: " << address << endl;
	}


private:

	uint32_t address;

};
#endif 