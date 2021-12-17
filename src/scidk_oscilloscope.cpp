#include "scidk_oscilloscope.h"

NI_RESULT SciSDK_Oscilloscope::ISetParamU32(string name, int32_t value) {

	if (name == "") {

		return NI_OK;
	}

	return NI_ERROR;
}