#include "scisdk_digitizer.h"
#include <functional>
#include <chrono>

/*
DEVICE DRIVER FOR DIGITIZER

IP Compatible version: 1.0

*/

SciSDK_Digitizer::SciSDK_Digitizer(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {

	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "INFO") address.info = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG") address.cfg_global = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "ACQ_LEN") address.cfg_acqlen = (uint32_t)r.at("Address");
	}

	settings.nchannels = (uint32_t)j.at("Channels");
	settings.nsamples = (uint32_t)j.at("nsamples");
	int ws = (uint32_t)j.at("WordSize");
	settings.usedma = (bool)j.at("UseDMA");

	switch (ws)
	{
	case 0:
		settings.wordsize = 4;
		break;
	default:
		break;
	}

	__buffer = (uint32_t *)malloc(settings.nchannels * settings.nsamples * (settings.wordsize) * sizeof(uint8_t));

	cout << "Digitizer: " << name << " addr: " << address.base << endl;


	RegisterParameter("enabledch", "enabled channels selector", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("acq_len", "acquisition length in samples", SciSDK_Paramcb::Type::U32, this);
	const std::list<std::string> listOfDataProcessing = { "raw","decode" };
	RegisterParameter("data_processing", "set data processing mode", SciSDK_Paramcb::Type::str, listOfDataProcessing, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking","threaded" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);

	RegisterParameter("threaded_buffer_size", "size of the fifo buffer in number of waves", SciSDK_Paramcb::Type::U32, this);

}


NI_RESULT SciSDK_Digitizer::ISetParamU32(string name, uint32_t value) {
	if (name == "enabledch") {
		enabledch = value;
		return ConfigureDigitizer();
	}
	else if (name == "acq_len") {
		acq_len = value;
		return ConfigureDigitizer();
	}
	else if (name == "threaded_buffer_size") {
		threaded_buffer_size = value;
		return NI_OK;
	}
	
	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Digitizer::ISetParamI32(string name, int32_t value) {
	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Digitizer::ISetParamString(string name, string value) {
	if (name == "data_processing") {
		if (value == "raw") {
			data_processing = DATA_PROCESSING::RAW;
			return NI_OK;
		}
		else if (value == "decode") {
			data_processing = DATA_PROCESSING::DECODE;
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	} 
	else if (name == "acq_mode") {
		if (value == "blocking") {
			acq_mode = ACQ_MODE::BLOCKING;
			return NI_OK;
		}
		else if (value == "non-blocking") {
			acq_mode = ACQ_MODE::NON_BLOCKING;
			return NI_OK;
		}
		else if (value == "threaded") {
			acq_mode = ACQ_MODE::THREADED;
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Digitizer::IGetParamU32(string name, uint32_t *value) {
	if (name == "enabledch") {
		*value = enabledch;
		return NI_OK;
	}
	else if (name == "acq_len") {
		*value = acq_len;
		return NI_OK;
	}
	else if (name == "threaded_buffer_size") {
		*value = threaded_buffer_size;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Digitizer::IGetParamI32(string name, int32_t *value) {
	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Digitizer::IGetParamString(string name, string *value) {
	if (name == "data_processing") {
		if (data_processing == DATA_PROCESSING::RAW) {
			*value = "raw";
			return NI_OK;
		}
		else if (data_processing == DATA_PROCESSING::DECODE) {
			*value = "decode";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "acq_mode") {
		if (acq_mode == ACQ_MODE::BLOCKING) {
			*value = "blocking";
			return NI_OK;
		}
		else if (acq_mode == ACQ_MODE::NON_BLOCKING) {
			*value = "non-blocking";
			return NI_OK;
		}
		else if (acq_mode == ACQ_MODE::THREADED) {
			*value = "threaded";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Digitizer::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

	return NI_OK;
}
NI_RESULT SciSDK_Digitizer::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_Digitizer::ReadData(void *buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_Digitizer::ExecuteCommand(string cmd, string param) {

	return NI_OK;
}
NI_RESULT SciSDK_Digitizer::ReadStatus(void *buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_Digitizer::ConfigureDigitizer() {

	return NI_OK;
}