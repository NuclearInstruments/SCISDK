#include "scisdk_oscilloscope_dual.h"
#include <functional>
#include <chrono>

SciSDK_Oscilloscope_Dual::SciSDK_Oscilloscope_Dual(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address.has_timestamp = false;
	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "CONFIG_DECIMATOR") address.cfg_decimator = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_PRETRIGGER") address.cfg_pretrigger = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_TRIGGER_LEVEL") address.cfg_trigger_level = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_TRIGGER_MODE") address.cfg_trigger_mode = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_ARM") address.cmd_arm = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "READ_POSITION") address.status_position = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "READ_STATUS") address.status_read = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "READ_TIMESTAMP") {
			address.has_timestamp = true;
			address.timestamp = (uint32_t)r.at("Address");
		}
	}

	data_processing = DATA_PROCESSING::DECODE;
	auto_arm = true;

	settings.nanalog = 2;
	settings.ndigital = 4;
	settings.nchannels = (uint32_t)j.at("Channels");
	settings.nsamples = (uint32_t)j.at("nsamples");


	__buffer = (uint32_t *)malloc(settings.nchannels * settings.nsamples * sizeof(uint32_t));

	RegisterParameter("decimator", "set x-axis decimation factor", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("pretrigger", "set pretrigger memory length", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("trigger_level", "set trigger level", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("trigger_channel", "set trigger channel", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("auto_arm", "set 1 to enable the auto arm feature", SciSDK_Paramcb::Type::U32, this);

	const std::list<std::string> listOfTriggers = { "disabled", "self","analog","ext","digital" };
	RegisterParameter("trigger_mode", "set trigger mode", SciSDK_Paramcb::Type::str, listOfTriggers, this);
	const std::list<std::string> listOfDataProcessing = { "raw","decode" };
	RegisterParameter("data_processing", "set data processing mode", SciSDK_Paramcb::Type::str, listOfDataProcessing, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	const std::list<std::string> listOfTriggerPolarity = { "pos","neg" };
	RegisterParameter("trigger_polarity", "set analog trigger polarity", SciSDK_Paramcb::Type::str, listOfTriggerPolarity, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	RegisterParameter("nanalog", "number of analog traces", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("ndigital", "number of digital traces", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("nchannels", "number of channels", SciSDK_Paramcb::Type::I32, this);

}

NI_RESULT SciSDK_Oscilloscope_Dual::ISetParamU32(string name, uint32_t value) {

	if (name == "decimator") {
		decimator = value;
		return ConfigureOscilloscope();
	}
	else if (name == "pretrigger") {
		pretrigger = value;
		return ConfigureOscilloscope();
	}
	else if (name == "trigger_level") {
		trigger_level = value;
		return ConfigureOscilloscope();
	}
	else if (name == "trigger_channel") {
		trigger_channel = value;
		return ConfigureOscilloscope();
	}
	else if (name == "trigger_dtrack") {
		trigger_dtrack = value;
		return ConfigureOscilloscope();
	}
	else if (name == "auto_arm") {
		auto_arm = value ? true : false;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Oscilloscope_Dual::ISetParamI32(string name, int32_t value) {

	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	}
	else if (name == "nanalog") {
		return NI_PARAMETER_CAN_NOT_BE_SET;
	}
	else if (name == "ndigital") {
		return NI_PARAMETER_CAN_NOT_BE_SET;
	}
	else if (name == "nchannels") {
		return NI_PARAMETER_CAN_NOT_BE_SET;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Oscilloscope_Dual::ISetParamString(string name, string value) {
	if (name == "trigger_mode") {
		if (value == "disabled") {
			trigger = TRIGGER_TYPE::DISABLED;
			return ConfigureOscilloscope();
		}
		else if (value == "self") {
			trigger = TRIGGER_TYPE::SELF;
			return ConfigureOscilloscope();
		}
		else if (value == "analog") {
			trigger = TRIGGER_TYPE::ANALOG;
			return ConfigureOscilloscope();
		}
		else if (value == "ext") {
			trigger = TRIGGER_TYPE::EXT;
			return ConfigureOscilloscope();
		}
		else if (value == "digital") {
			trigger = TRIGGER_TYPE::DIGITAL_TRACK;
			return ConfigureOscilloscope();
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "data_processing") {
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
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "trigger_polarity") {
		if (value == "pos") {
			trigger_polarity = TRIGGER_POLARITY::POSITIVE;
			return ConfigureOscilloscope();
		}
		else if (value == "neg") {
			trigger_polarity = TRIGGER_POLARITY::NEGATIVE;
			return ConfigureOscilloscope();
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Oscilloscope_Dual::IGetParamU32(string name, uint32_t *value) {

	if (name == "decimator") {
		*value = decimator;
		return NI_OK;
	}
	else if (name == "pretrigger") {
		*value = pretrigger;
		return NI_OK;
	}
	else if (name == "trigger_level") {
		*value = trigger_level;
		return NI_OK;
	}
	else if (name == "trigger_channel") {
		*value = trigger_channel;
		return NI_OK;
	}
	else if (name == "trigger_dtrack") {
		*value = trigger_dtrack;
		return NI_OK;
	}
	else if (name == "auto_arm") {
		*value = auto_arm ? 1 : 0;
		return NI_OK;
	}


	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Oscilloscope_Dual::IGetParamI32(string name, int32_t *value) {

	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}
	else if (name == "nanalog") {
		*value = settings.nanalog;
		return NI_OK;
	}
	else if (name == "ndigital") {
		*value = settings.ndigital;
		return NI_OK;
	}
	else if (name == "nchannels") {
		*value = settings.nchannels;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Oscilloscope_Dual::IGetParamString(string name, string *value) {
	if (name == "trigger_mode") {
		if (trigger == TRIGGER_TYPE::DISABLED) {
			*value = "disabled";
			return NI_OK;
		}
		else if (trigger == TRIGGER_TYPE::SELF) {
			*value = "self";
			return NI_OK;
		}
		else if (trigger == TRIGGER_TYPE::ANALOG) {
			*value = "analog";
			return NI_OK;
		}
		else if (trigger == TRIGGER_TYPE::EXT) {
			*value = "ext";
			return NI_OK;
		}
		else if (trigger == TRIGGER_TYPE::DIGITAL_TRACK) {
			*value = "digital";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "data_processing") {
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
	if (name == "trigger_polarity") {
		if (trigger_polarity == TRIGGER_POLARITY::POSITIVE) {
			*value = "pos";
			return NI_OK;
		}
		else if (trigger_polarity == TRIGGER_POLARITY::NEGATIVE) {
			*value = "neg";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "buffer_type") {
		if (data_processing == DATA_PROCESSING::RAW) {
			*value = "SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER";
			return NI_OK;
		}
		else if (data_processing == DATA_PROCESSING::DECODE) {
			*value = "SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Oscilloscope_Dual::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

	if (bt == T_BUFFER_TYPE_DECODED) {
		*buffer = (SCISDK_OSCILLOSCOPE_DECODED_BUFFER *)malloc(sizeof(SCISDK_OSCILLOSCOPE_DECODED_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_OSCILLOSCOPE_DECODED_BUFFER *p;
		p = (SCISDK_OSCILLOSCOPE_DECODED_BUFFER*)*buffer;
		p->analog = (int32_t*)malloc(sizeof(int32_t) * (settings.nchannels*settings.nanalog * settings.nsamples + 8));
		p->digital = (uint8_t*)malloc(sizeof(uint8_t) * (settings.nchannels*settings.ndigital * settings.nsamples + 8));

		if ((p->analog == NULL) || (p->digital == NULL)) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_OSCILLOSCOPE_DECODED;
		p->timecode = 0;
		p->trigger_position = 0;
		p->info.channels = settings.nchannels;
		p->info.samples_analog = settings.nsamples;
		p->info.samples_digital = settings.nsamples;
		p->info.tracks_analog_per_channel = settings.nanalog;
		p->info.tracks_digital_per_channel = settings.ndigital;
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_OSCILLOSCOPE_RAW_BUFFER *)malloc(sizeof(SCISDK_OSCILLOSCOPE_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_OSCILLOSCOPE_RAW_BUFFER *p;
		p = (SCISDK_OSCILLOSCOPE_RAW_BUFFER*)*buffer;
		int32_t buffer_size = settings.nsamples * settings.nchannels;
		p->data = (uint32_t*)malloc(sizeof(uint32_t) * (buffer_size + 8));

		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_OSCILLOSCOPE_RAW;
		p->zero_position = 0;
		p->timecode = 0;
		p->trigger_position = 0;
		p->info.buffer_size = buffer_size;
		p->info.channels = settings.nchannels;
		p->info.samples_analog = settings.nsamples;
		p->info.samples_digital = settings.nsamples;
		p->info.tracks_analog_per_channel = settings.nanalog;
		p->info.tracks_digital_per_channel = settings.ndigital;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}

}

NI_RESULT SciSDK_Oscilloscope_Dual::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {
	if (bt == T_BUFFER_TYPE_DECODED) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_OSCILLOSCOPE_DECODED_BUFFER *p;
		p = (SCISDK_OSCILLOSCOPE_DECODED_BUFFER*)*buffer;
		if (p->analog != NULL) {
			free(p->analog);
			p->analog = NULL;
		}
		if (p->digital != NULL) {
			free(p->digital);
			p->digital = NULL;
		}
		p->magic = BUFFER_TYPE_INVALID;
		p->timecode = 0;
		p->trigger_position = 0;
		p->info.channels = 0;
		p->info.samples_analog = 0;
		p->info.samples_digital = 0;
		p->info.tracks_analog_per_channel = 0;
		p->info.tracks_digital_per_channel = 0;
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {
		SCISDK_OSCILLOSCOPE_RAW_BUFFER *p;
		p = (SCISDK_OSCILLOSCOPE_RAW_BUFFER*)*buffer;
		if (p->data != NULL) {
			free(p->data);
			p->data = NULL;
		}
		p->magic = BUFFER_TYPE_INVALID;
		p->timecode = 0;
		p->trigger_position = 0;
		p->zero_position = 0;
		p->info.channels = 0;
		p->info.samples_analog = 0;
		p->info.samples_digital = 0;
		p->info.tracks_analog_per_channel = 0;
		p->info.tracks_digital_per_channel = 0;
		p->info.buffer_size = 0;

		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_Oscilloscope_Dual::ReadData(void *buffer) {
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
	CheckOscilloscopeStatus(&ready, &armed, &running);
	if ((!armed) && (!ready) && (!running)) {
		if (!auto_arm) {
			return NI_NOT_ARMED;
		}
		else {
			CmdArm();
			CheckOscilloscopeStatus(&ready, &armed, &running);
			if ((!armed) && (!ready) && (!running)) return NI_NOT_ARMED;
		}
	}

	if (acq_mode == ACQ_MODE::BLOCKING) {
		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		while (!ready && ((timeout >= 0) && (elapsed_time_ms < timeout))) {
			auto t_end = std::chrono::high_resolution_clock::now();
			elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
			CheckOscilloscopeStatus(&ready, &armed, &running);
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

	if (_hal->ReadReg(&zero_posizition, address.status_position)) return NI_ERROR_INTERFACE;
	if (zero_posizition > settings.nsamples) return NI_ERROR_INTERFACE;

	if (address.has_timestamp) {
		if (_hal->ReadData(rtimestamp, 2, address.timestamp, 100, &dv)) return NI_ERROR_INTERFACE;
		timestamp = (uint64_t)rtimestamp[0] + (((uint64_t)rtimestamp[1]) << 32UL);
	}
	else {
		std::chrono::time_point<std::chrono::steady_clock> ts = std::chrono::steady_clock::now();
		timestamp = std::chrono::duration_cast<std::chrono::microseconds>(ts.time_since_epoch()).count();
	}


	if (data_processing == DATA_PROCESSING::DECODE) {
		SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER *p;
		p = (SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER*)buffer;
		//check user buffer
		if (p->magic != BUFFER_TYPE_OSCILLOSCOPE_DECODED) return NI_INVALID_BUFFER_TYPE;
		if (p->info.channels != settings.nchannels) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.samples_analog != settings.nsamples) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.samples_digital != settings.nsamples) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.tracks_analog_per_channel != settings.nanalog) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.tracks_digital_per_channel != settings.ndigital) return NI_INCOMPATIBLE_BUFFER;

		//check service buffer
		if (__buffer == NULL) {
			return NI_ERROR_GENERIC;
		}
		//download data
		uint32_t buffer_size = settings.nchannels * settings.nsamples / 2;

		if (_hal->ReadData(__buffer, buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;
		/*for (int i = 0; i < 4; i++) {
			cout << __buffer[i] << endl;
		}
		cout << "..." << endl;
		for (int i = 0; i < 100; i++) {
			cout << __buffer[i] << endl;
		}
		cout << "----" << endl;*/
		CmdResetReadValidFlag();
		
		if (dv < buffer_size) {
			return NI_INCOMPLETE_READ;
		}
		//decode data
		p->trigger_position = pretrigger;
		p->timecode = timestamp;
		for (int n = 0; n < settings.nchannels / 2; n++)
		{
			int current = zero_posizition - pretrigger + 2;
			if (current > 0)
			{
				int k = 0;
				for (int i = current; i < settings.nsamples; i++)
				{
					p->analog[k + (settings.nsamples*(n * 2 + 0))] = __buffer[i + (settings.nsamples*n)] & 0x3FFF;
					p->analog[k + (settings.nsamples*(n * 2 + 1))] = (__buffer[i + (settings.nsamples*n)] >> 14) & 0x3FFF;
					//read_digital0[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 28 & 1;
					//read_digital1[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 29 & 1;
					//read_digital2[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 30 & 1;
					//read_digital3[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 31 & 1;
					for (int d = 0; d < settings.ndigital; d++) {
						p->digital[k + (settings.nsamples * d) + (settings.nsamples * settings.ndigital * n)] = __buffer[i + (settings.nsamples*n)] >> (28 + d) & 1;
					}
					k++;
				}
				for (int i = 0; i < current; i++)
				{
					p->analog[k + (settings.nsamples*(n * 2 + 0))] = __buffer[i + (settings.nsamples*n)] & 0x3FFF;
					p->analog[k + (settings.nsamples*(n * 2 + 1))] = (__buffer[i + (settings.nsamples*n)] >> 14) & 0x3FFF;
					//read_digital0[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 28 & 1;
					//read_digital1[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 29 & 1;
					//read_digital2[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 30 & 1;
					//read_digital3[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 31 & 1;
					for (int d = 0; d < settings.ndigital; d++) {
						p->digital[k + (settings.nsamples * d) + (settings.nsamples * settings.ndigital * n)] = __buffer[i + (settings.nsamples*n)] >> (28 + d) & 1;
					}
					k++;
				}
			}
			else
			{
				int k = 0;
				for (int i = settings.nsamples + current; i < settings.nsamples; i++)
				{
					p->analog[k + (settings.nsamples*(n * 2 + 0))] = __buffer[i + (settings.nsamples*n)] & 0x3FFF;
					p->analog[k + (settings.nsamples*(n * 2 + 1))] = (__buffer[i + (settings.nsamples*n)] >> 14) & 0x3FFF;
					//read_digital0[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 28 & 1;
					//read_digital1[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 29 & 1;
					//read_digital2[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 30 & 1;
					//read_digital3[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 31 & 1;
					for (int d = 0; d < settings.ndigital; d++) {
						p->digital[k + (settings.nsamples * d) + (settings.nsamples * settings.ndigital * n)] = __buffer[i + (settings.nsamples*n)] >> (28 + d) & 1;
					}
					k++;
				}
				for (int i = 0; i < settings.nsamples + current; i++)
				{
					p->analog[k + (settings.nsamples*(n * 2 + 0))] = __buffer[i + (settings.nsamples*n)] & 0x3FFF;
					p->analog[k + (settings.nsamples*(n * 2 + 1))] = (__buffer[i + (settings.nsamples*n)] >> 14) & 0x3FFF;
					//read_digital0[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 28 & 1;
					//read_digital1[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 29 & 1;
					//read_digital2[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 30 & 1;
					//read_digital3[k + (n_samples*n)] = data_osc[i + (n_samples*n)] >> 31 & 1;
					for (int d = 0; d < settings.ndigital; d++) {
						p->digital[k + (settings.nsamples * d) + (settings.nsamples * settings.ndigital * n)] = __buffer[i + (settings.nsamples*n)] >> (28 + d) & 1;
					}
					k++;
				}
			}
		}

		return NI_OK;
	}
	else if (data_processing == DATA_PROCESSING::RAW) {
		SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER *p;
		p = (SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER*)buffer;
		//check user buffer
		if (p->magic != BUFFER_TYPE_OSCILLOSCOPE_RAW) return NI_INVALID_BUFFER_TYPE;
		if (p->info.buffer_size < settings.nchannels * settings.nsamples) return NI_INCOMPATIBLE_BUFFER;
		//download data
		if (_hal->ReadData(p->data, p->info.buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;
		CmdResetReadValidFlag();
		if (dv < p->info.buffer_size) {
			return NI_INCOMPLETE_READ;
		}
		p->trigger_position = pretrigger;
		p->zero_position = zero_posizition + 1;
		p->timecode = timestamp;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
	return NI_OK;
}

NI_RESULT SciSDK_Oscilloscope_Dual::ConfigureOscilloscope() {
	int ret = 0;
	ret |= _hal->WriteReg(decimator, address.cfg_decimator);
	ret |= _hal->WriteReg(pretrigger, address.cfg_pretrigger);
	ret |= _hal->WriteReg(trigger_level, address.cfg_trigger_level);
	int32_t trigger_reg = 0;
	switch (trigger) {
	case TRIGGER_TYPE::DISABLED:
		trigger_reg = 2;
		break;
	case TRIGGER_TYPE::SELF:
		trigger_reg = 2 + (1 << 7);
		break;
	case TRIGGER_TYPE::ANALOG:
		trigger_reg = 1;
		break;
	case TRIGGER_TYPE::EXT:
		trigger_reg = 0;
		break;
	case TRIGGER_TYPE::DIGITAL_TRACK:
		trigger_reg = 4 + (trigger_dtrack & 0x3);
		break;

	}

	switch (trigger_polarity) {
	case TRIGGER_POLARITY::POSITIVE:
		trigger_reg += 0 << 3;
		break;
	case TRIGGER_POLARITY::NEGATIVE:
		trigger_reg += 1 << 3;
		break;
	}
	trigger_reg += (trigger_channel & 0xFF) << 8;
	ret |= _hal->WriteReg(trigger_reg, address.cfg_trigger_mode);



	if (ret)
		return NI_ERROR_INTERFACE;
	else {
		CmdResetReadValidFlag();
		return NI_OK;
	}
}

NI_RESULT SciSDK_Oscilloscope_Dual::CmdArm() {
	int ret = 0;
	ret |= _hal->WriteReg(0, address.cmd_arm);
	ret |= _hal->WriteReg(1, address.cmd_arm);
	ret |= _hal->WriteReg(0, address.cmd_arm);

	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_Oscilloscope_Dual::CmdResetReadValidFlag() {
	int ret = 0;
	ret |= _hal->WriteReg(0, address.cmd_arm);
	ret |= _hal->WriteReg(2, address.cmd_arm);
	ret |= _hal->WriteReg(0, address.cmd_arm);

	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_Oscilloscope_Dual::ExecuteCommand(string cmd, string param) {
	if (cmd == "arm") {
		return CmdArm();
	}
	else if (cmd == "reset_read_valid_flag") {
		return CmdResetReadValidFlag();
	}

	return NI_INVALID_COMMAND;
}

NI_RESULT SciSDK_Oscilloscope_Dual::CheckOscilloscopeStatus(bool *ready, bool *armed, bool *running) {
	uint32_t status;
	if (_hal->ReadReg(&status, address.status_read)) return NI_ERROR_INTERFACE;
	*armed = (status >> 1) & 0x1 ? true : false;
	*ready = status & 0x1 ? true : false;
	*running = (status >> 2) & 0x1 ? true : false;
	return NI_OK;
}

NI_RESULT SciSDK_Oscilloscope_Dual::ReadStatus(void *buffer) {
	SCISDK_OSCILLOSCOPE_STATUS *p = (SCISDK_OSCILLOSCOPE_STATUS*)buffer;
	return CheckOscilloscopeStatus(&p->ready, &p->armed, &p->running);
}