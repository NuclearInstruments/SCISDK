#ifndef HEADER_H_SCISDK_ELEMENT
#define HEADER_H_SCISDK_ELEMENT
#include <iostream>
#include <algorithm>
#include <list>
#include "NIErrorCode.h"

#include "json.hpp"


using json = nlohmann::json;

using namespace std;

enum class MMC_ELEMENT {
	REGISTER,
	OSCILLOSCOPE,
	WAVEDUMP,
	LIST,
	SPECTRUM,

};

class SciElement {
public:
	MMC_ELEMENT type;
	vector<string> params;
	json j;
	bool isSubParameter();
	bool getAddress(uint32_t *address);
	
};
#endif