#include "scisdk_logicanalyser.h"
#include <functional>
#include <chrono>

/*
	DEVICE DRIVER FOR LOGIC ANALYSER

	IP Compatible version: 1.0

*/

SciSDK_LogicAnalyser::SciSDK_LogicAnalyser(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG") address.config = (uint32_t)r.at("Address");
	}

	// nsamples in JSON is the total number of words to read from FIFO
	uint32_t total_words = (uint32_t)j.at("nsamples");

	// Count digital signals from ChannelsList
	settings.ntraces = 0;
	if (j.contains("ChannelsList")) {
		settings.ntraces = j.at("ChannelsList").size();
	}

	// Calculate words per sample: round up ntraces to next multiple of 32
	settings.words_per_sample = (settings.ntraces + 31) / 32;

	// Calculate logical samples: total_words / words_per_sample
	settings.nsamples = total_words / settings.words_per_sample;

	// Allocate buffer for total_words
	__buffer = (uint32_t *)malloc(total_words * sizeof(uint32_t));

	isRunning = false;

	const std::list<std::string> listOfTriggerMode = { "software","signal" };
	RegisterParameter("trigger_mode", "set trigger mode: software or signal", SciSDK_Paramcb::Type::str, listOfTriggerMode, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);

	RegisterParameter("ntraces", "number of digital traces", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("nsamples", "number of samples", SciSDK_Paramcb::Type::I32, this);

	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);
}

NI_RESULT SciSDK_LogicAnalyser::ISetParamU32(string name, uint32_t value) {
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_LogicAnalyser::ISetParamI32(string name, int32_t value) {

	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	} else if (name == "ntraces") {
		return NI_PARAMETER_CAN_NOT_BE_SET;
	} else if (name == "nsamples") {
		return NI_PARAMETER_CAN_NOT_BE_SET;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_LogicAnalyser::ISetParamString(string name, string value) {
	if (name == "trigger_mode") {
		if (value == "software") {
			trigger_mode = TRIGGER_MODE::SOFTWARE;
			return NI_OK;
		}
		else if (value == "signal") {
			trigger_mode = TRIGGER_MODE::SIGNAL;
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
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_LogicAnalyser::IGetParamU32(string name, uint32_t *value) {
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_LogicAnalyser::IGetParamI32(string name, int32_t *value) {

	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	} else if (name == "ntraces") {
		*value = settings.ntraces;
		return NI_OK;
	} else if (name == "nsamples") {
		*value = settings.nsamples;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_LogicAnalyser::IGetParamString(string name, string *value) {
	if (name == "trigger_mode") {
		if (trigger_mode == TRIGGER_MODE::SOFTWARE) {
			*value = "software";
			return NI_OK;
		}
		else if (trigger_mode == TRIGGER_MODE::SIGNAL) {
			*value = "signal";
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
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "buffer_type") {
		*value = "SCISDK_LOGICANALYSER_DECODED_BUFFER";
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_LogicAnalyser::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

	if (bt == T_BUFFER_TYPE_DECODED) {
		*buffer = (SCISDK_LOGICANALYSER_DECODED_BUFFER *)malloc(sizeof(SCISDK_LOGICANALYSER_DECODED_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_LOGICANALYSER_DECODED_BUFFER *p;
		p = (SCISDK_LOGICANALYSER_DECODED_BUFFER*)*buffer;

		// Allocate space: nsamples * words_per_sample
		// Each sample can use multiple 32-bit words if ntraces > 32
		p->data = (uint32_t*)malloc(sizeof(uint32_t) * (settings.nsamples * settings.words_per_sample + 8));

		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_LOGICANALYSER_DECODED;
		p->timecode = 0;
		p->info.samples = settings.nsamples;
		p->info.ntraces = settings.ntraces;
		p->info.words_per_sample = settings.words_per_sample;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_LogicAnalyser::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {
	if (bt == T_BUFFER_TYPE_DECODED) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_LOGICANALYSER_DECODED_BUFFER *p;
		p = (SCISDK_LOGICANALYSER_DECODED_BUFFER*)*buffer;
		if (p->data != NULL) {
			free(p->data);
			p->data = NULL;
		}
		p->magic = BUFFER_TYPE_INVALID;
		p->timecode = 0;
		p->info.samples = 0;
		p->info.ntraces = 0;
		p->info.words_per_sample = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_LogicAnalyser::ReadData(void *buffer) {
	uint32_t dv;
	uint64_t timestamp;

	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}

	SCISDK_LOGICANALYSER_DECODED_BUFFER *p;
	p = (SCISDK_LOGICANALYSER_DECODED_BUFFER*)buffer;

	// Check user buffer
	if (p->magic != BUFFER_TYPE_LOGICANALYSER_DECODED) return NI_INVALID_BUFFER_TYPE;
	if (p->info.samples != settings.nsamples) return NI_INCOMPATIBLE_BUFFER;
	if (p->info.ntraces != settings.ntraces) return NI_INCOMPATIBLE_BUFFER;
	if (p->info.words_per_sample != settings.words_per_sample) return NI_INCOMPATIBLE_BUFFER;

	// Check service buffer
	if (__buffer == NULL) return NI_ERROR_GENERIC;

	// Check if data available
	uint32_t status;
	NI_RESULT ret = CheckLogicAnalyserStatus(&status);
	if (ret != NI_OK) return ret;

	bool data_available = (status & 0x1) ? true : false;

	if (acq_mode == ACQ_MODE::BLOCKING) {
		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		while (!data_available && ((timeout < 0) || (elapsed_time_ms < timeout))) {
			auto t_end = std::chrono::high_resolution_clock::now();
			elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
			CheckLogicAnalyserStatus(&status);
			data_available = (status & 0x1) ? true : false;
		}
		if (!data_available) {
			return NI_TIMEOUT;
		}
	}
	else {
		if (!data_available) {
			return NI_TIMEOUT;
		}
	}

	// Download data from FIFO
	// Total words to read = nsamples * words_per_sample
	// For 44 traces with 1024 logical samples: words_per_sample = 2, nsamples = 1024, total = 2048 words
	uint32_t buffer_size = settings.nsamples * settings.words_per_sample;
	uint32_t valid_data = 0;

	// Use HAL API to download data
	ret = _hal->ReadFIFO(__buffer, buffer_size, address.base, address.status, timeout, &valid_data);

	if (ret != NI_OK) return NI_ERROR_INTERFACE;
	if (valid_data < buffer_size) {
		return NI_INCOMPLETE_READ;
	}

	// Get timestamp
	std::chrono::time_point<std::chrono::steady_clock> ts = std::chrono::steady_clock::now();
	timestamp = std::chrono::duration_cast<std::chrono::microseconds>(ts.time_since_epoch()).count();

	// Copy data to user buffer
	// Data format: words_per_sample consecutive words per logical sample
	// Example: data[0..1] = sample 0, data[2..3] = sample 1, etc.
	p->timecode = timestamp;
	for (int i = 0; i < buffer_size; i++) {
		p->data[i] = __buffer[i];
	}

	return NI_OK;
}

NI_RESULT SciSDK_LogicAnalyser::ConfigureLogicAnalyser() {
	return NI_OK;
}

NI_RESULT SciSDK_LogicAnalyser::CmdStart() {

	// Reset logic analyser
	int r1 = _hal->WriteReg(2, address.config);
	int r2 = _hal->WriteReg(1, address.config);

	// Configure trigger mode
	uint32_t triggermode = 0;
	if (trigger_mode == TRIGGER_MODE::SOFTWARE) {
		triggermode = 0x10;  // Software trigger
	} else {
		triggermode = 1 << 2;  // Signal trigger
	}
	int r3 = _hal->WriteReg(triggermode, address.config);

	if ((r1 == 0) && (r2 == 0) && (r3 == 0)) {
		isRunning = true;
		return NI_OK;
	}
	else {
		return NI_ERROR_INTERFACE;
	}
}

NI_RESULT SciSDK_LogicAnalyser::CmdReset() {
	int ret = _hal->WriteReg(2, address.config);
	if (ret == 0) {
		isRunning = false;
		return NI_OK;
	}
	else {
		return NI_ERROR_INTERFACE;
	}
}

NI_RESULT SciSDK_LogicAnalyser::CheckLogicAnalyserStatus(uint32_t *status) {
	if (_hal->ReadReg(status, address.status)) return NI_ERROR_INTERFACE;
	*status = *status & 0xf;
	return NI_OK;
}

NI_RESULT SciSDK_LogicAnalyser::ExecuteCommand(string cmd, string param) {
	if (cmd == "start") {
		return CmdStart();
	}
	else if (cmd == "reset") {
		return CmdReset();
	}

	return NI_INVALID_COMMAND;
}

NI_RESULT SciSDK_LogicAnalyser::ReadStatus(void *buffer) {
	return NI_OK;
}

NI_RESULT SciSDK_LogicAnalyser::Detach() {
	if (__buffer) {
		free(__buffer);
		__buffer = NULL;
	}
	isRunning = false;
	return NI_OK;
}
