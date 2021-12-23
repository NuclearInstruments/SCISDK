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
		RegisterParameter("decimator", "set x-axis decimation factor", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("pretrigger", "set pretrigger memory length", SciSDK_Paramcb::Type::U32, this);
		const std::list<std::string> listOfTriggers = { "self","analog","ext","digital" };
		RegisterParameter("trigger_mode", "set trigger mode", SciSDK_Paramcb::Type::str, listOfTriggers, this);

	}


	NI_RESULT ISetParamU32(string name, int32_t value);
private:
	uint32_t decimator;
	uint32_t pretrigger;

	NI_RESULT ConfigureOscilloscope();
	uint32_t address;

};
#endif 