#include "scisdk_SciElement.h"



bool SciElement::isSubParameter() {
	if (params.size() > 1) {
		return true;
	}
	else { return false; }
}


bool SciElement::getAddress(uint32_t *address) {
	try
	{
		json rs = j;
		for (int i = 1; i<params.size(); i++) {
			rs = rs.at(params[i]);
		}
		*address = rs.at("Address");
		return true;
	}
	catch (json::exception& e) {
		return false;
	}
}