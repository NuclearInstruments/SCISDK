#include "scisdk_fftmon.h"
#include <functional>
#include <chrono>

/*
		DEVICE DRIVER FOR OSCILLOSCPE

		IP Compatible version: 1.4

*/

SciSDK_FFT::SciSDK_FFT(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address.has_timestamp = false;
	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "CONFIG_DECIMATOR") address.cfg_decimator = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_ARM") address.cmd_arm = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "READ_STATUS") address.status_read = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "READ_TIMESTAMP") { 
			address.has_timestamp = true;
			address.timestamp = (uint32_t)r.at("Address"); }
	}
	
	data_processing = DATA_PROCESSING::DECODE;
	auto_arm=true;

	settings.nsamples = (uint32_t)j.at("nsamples");
	settings.nchannels = (uint32_t)j.at("Channels");


	__buffer = (uint32_t *)malloc(2*settings.nchannels * settings.nsamples * sizeof(uint32_t));

	cout << "FFT: " << name << " addr: " << address.base << endl;
	RegisterParameter("decimator", "set x-axis decimation factor", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("auto_arm", "set 1 to enable the auto arm feature", SciSDK_Paramcb::Type::U32, this);

	const std::list<std::string> listOfDataProcessing = { "raw","decode" };
	RegisterParameter("data_processing", "set data processing mode", SciSDK_Paramcb::Type::str, listOfDataProcessing, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);
}

NI_RESULT SciSDK_FFT::ISetParamU32(string name, uint32_t value) {

	if (name == "decimator") {
		decimator = value;
		return ConfigureFFT();
	} else if (name == "auto_arm") {
		auto_arm = value ? true : false;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}


NI_RESULT SciSDK_FFT::ISetParamI32(string name, int32_t value) {

	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_FFT::ISetParamString(string name, string value) {
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
	} else if (name == "acq_mode") {
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

NI_RESULT SciSDK_FFT::IGetParamU32(string name, uint32_t *value) {

	if (name == "decimator") {
		*value = decimator;
		return NI_OK;
	}
	else if (name == "auto_arm") {
		*value = auto_arm ? 1 : 0;
		return NI_OK;
	}


	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_FFT::IGetParamI32(string name, int32_t *value) {

	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_FFT::IGetParamString(string name, string *value) {
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
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "buffer_type") {
		if (data_processing == DATA_PROCESSING::RAW) {
			*value = "SCISDK_FFT_RAW_BUFFER";
			return NI_OK;
		}
		else if (data_processing == DATA_PROCESSING::DECODE) {
			*value = "SCISDK_FFT_DECODED_BUFFER";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}


	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_FFT::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

	if (bt == T_BUFFER_TYPE_DECODED) {
		*buffer = (SCISDK_FFT_DECODED_BUFFER *)malloc(sizeof(SCISDK_FFT_DECODED_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_FFT_DECODED_BUFFER *p;
		p = (SCISDK_FFT_DECODED_BUFFER*)*buffer;
		p->mag = (double*)malloc(sizeof(double) * (settings.nchannels*settings.nsamples +8));
		p->ph = (double*)malloc(sizeof(double) * (settings.nchannels*settings.nsamples +8));

		if ((p->mag == NULL) || (p->ph == NULL)) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_FFT_DECODED;
		p->timecode = 0;
		p->info.channels = settings.nchannels;
		p->info.samples = settings.nsamples;
	
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_FFT_RAW_BUFFER *)malloc(sizeof(SCISDK_FFT_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_FFT_RAW_BUFFER *p;
		p = (SCISDK_FFT_RAW_BUFFER*)*buffer;
		int32_t buffer_size = 2*settings.nsamples * settings.nchannels ;
		p->data = (uint32_t*)malloc(sizeof(uint32_t) * (buffer_size+8));
		
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_FFT_RAW;
		p->timecode = 0;
		p->info.buffer_size = buffer_size;
		p->info.channels = settings.nchannels;
		p->info.samples = settings.nsamples;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
	
}

NI_RESULT SciSDK_FFT::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {
	if (bt == T_BUFFER_TYPE_DECODED) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_FFT_DECODED_BUFFER *p;
		p = (SCISDK_FFT_DECODED_BUFFER*)*buffer;
		if (p->mag != NULL) {
			free(p->mag);
			p->mag = NULL;
		}
		if (p->ph != NULL) {
			free(p->ph);
			p->ph = NULL;
		}
		p->magic = BUFFER_TYPE_INVALID;
		p->timecode = 0;
		p->info.channels = 0;
		p->info.samples = 0;
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {
		SCISDK_FFT_RAW_BUFFER *p;
		p = (SCISDK_FFT_RAW_BUFFER*)*buffer;
		if (p->data != NULL) {
			free(p->data);
			p->data = NULL;
		}
		p->magic = BUFFER_TYPE_INVALID;
		p->timecode = 0;
		p->info.channels = 0;
		p->info.samples = 0;
		p->info.buffer_size = 0;

		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_FFT::ReadData(void *buffer) {
	uint32_t dv;
	uint32_t zero_posizition;
	uint32_t rtimestamp[2];
	uint64_t timestamp;
	bool ready = false;
	bool armed = false;
	bool running = false;
	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}
	CheckFFTStatus(&ready, &armed, &running);
	if ((!armed) && (!ready) && (!running)) {
		if (!auto_arm) {
			return NI_NOT_ARMED;
		}
		else {
			CmdArm();
			CheckFFTStatus(&ready, &armed, &running);
			if ((!armed) && (!ready) && (!running)) return NI_NOT_ARMED;
		}
	}

	if (acq_mode == ACQ_MODE::BLOCKING) {
		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		while (!ready && ((timeout >= 0) && (elapsed_time_ms < timeout))) {
			auto t_end = std::chrono::high_resolution_clock::now();
			elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
			CheckFFTStatus(&ready, &armed, &running);
		}
		if (!ready) {
			return NI_TIMEOUT;
		}
	}
	else {
		if (!ready) {
			return NI_TIMEOUT;
		}
	}

	if (address.has_timestamp) {
		if (_hal->ReadData(rtimestamp, 2, address.timestamp, 100, &dv)) return NI_ERROR_INTERFACE;
		timestamp = (uint64_t)rtimestamp[0] + (((uint64_t)rtimestamp[1]) << 32UL);
	}
	else {
		std::chrono::time_point<std::chrono::steady_clock> ts = std::chrono::steady_clock::now();
		timestamp = std::chrono::duration_cast<std::chrono::microseconds>(ts.time_since_epoch()).count();
	}
	
	
	if (data_processing  == DATA_PROCESSING::DECODE) {
		SCISDK_FFT_DECODED_BUFFER *p;
		p = (SCISDK_FFT_DECODED_BUFFER*)buffer;
		//check user buffer
		if (p->magic != BUFFER_TYPE_FFT_DECODED) return NI_INVALID_BUFFER_TYPE;
		if (p->info.channels != settings.nchannels) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.samples != settings.nsamples) return NI_INCOMPATIBLE_BUFFER;

		
		//check service buffer
		if (__buffer == NULL) return NI_ERROR_GENERIC;

		//download data
		uint32_t buffer_size = settings.nchannels * settings.nsamples*2;
		if (_hal->ReadData(__buffer, buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;
		for (int i = 0; i < 4; i++) {
			cout << __buffer[i] << endl;
		}
		cout << "..." << endl;
		for (int i = buffer_size-4; i < buffer_size; i++) {
			cout << __buffer[i] << endl;
		}
		cout << "----" << endl;
		CmdResetReadValidFlag();
		if (dv < buffer_size) {
			return NI_INCOMPLETE_READ;
		}
		//decode data
		p->timecode = timestamp;

		for (int n = 0; n< settings.nchannels; n++)
		{
			uint32_t choffset = n*settings.nsamples*2;
			for (int i = 0; i < settings.nsamples; i++)
			{
				double re = (double)__buffer[choffset + (i*2)];
				double im = (double)__buffer[choffset + (i*2)+1];
				p->mag[i + (settings.nsamples*n)] = sqrt(pow(re,2)+pow(im,2));
				p->ph[i + (settings.nsamples*n)] = atan(im/re);
			}
		}
		return NI_OK;
	}
	else if (data_processing == DATA_PROCESSING::RAW) {
		SCISDK_FFT_RAW_BUFFER *p;
		p = (SCISDK_FFT_RAW_BUFFER*)buffer;
		//check user buffer
		if (p->magic != BUFFER_TYPE_FFT_RAW) return NI_INVALID_BUFFER_TYPE;
		if (p->info.buffer_size < settings.nchannels * settings.nsamples*2) return NI_INCOMPATIBLE_BUFFER;
		//download data
		if (_hal->ReadData(p->data, p->info.buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;
		CmdResetReadValidFlag();
		if (dv < p->info.buffer_size) {
			return NI_INCOMPLETE_READ;
		}
		p->timecode = timestamp;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
	return NI_OK;
}

NI_RESULT SciSDK_FFT::ConfigureFFT() {
	int ret = 0;
	ret |= _hal->WriteReg(decimator, address.cfg_decimator);

	

	if (ret)
		return NI_ERROR_INTERFACE;
	else{
		CmdResetReadValidFlag();
		return NI_OK;
	}
}

NI_RESULT SciSDK_FFT::CmdArm() {
	int ret = 0;
	ret |= _hal->WriteReg(0,address.cmd_arm);
	ret |= _hal->WriteReg(1, address.cmd_arm);
	ret |= _hal->WriteReg(0, address.cmd_arm);

	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_FFT::CmdResetReadValidFlag() {
	int ret = 0;
	ret |= _hal->WriteReg(0, address.cmd_arm);
	ret |= _hal->WriteReg(2, address.cmd_arm);
	ret |= _hal->WriteReg(0, address.cmd_arm);

	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_FFT::ExecuteCommand(string cmd, string param) {
	if (cmd == "arm") {
		return CmdArm();
	} else if (cmd == "reset_read_valid_flag") {
		return CmdResetReadValidFlag();
	}

	return NI_INVALID_COMMAND;
}

NI_RESULT SciSDK_FFT::CheckFFTStatus(bool *ready, bool *armed, bool *running) {
	uint32_t status;
	if (_hal->ReadReg(&status, address.status_read)) return NI_ERROR_INTERFACE;
	*armed = (status >> 1) & 0x1 ? true : false;
	*ready = status & 0x1 ? true : false;
	*running = (status >> 2) & 0x1 ? true : false;
	return NI_OK;
}


NI_RESULT SciSDK_FFT::ReadStatus(void *buffer) {
	SCISDK_FFT_STATUS *p = (SCISDK_FFT_STATUS*)buffer;
	return CheckFFTStatus(&p->ready, &p->armed, &p->running);
}