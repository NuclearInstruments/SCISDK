#include "scisdk_digitizer.h"
#include <functional>
#include <chrono>
#include <thread>

/*
DEVICE DRIVER FOR DIGITIZER

IP Compatible version: 1.0

*/

SciSDK_Digitizer::SciSDK_Digitizer(SciSDK_HAL* hal, json j, string path) : SciSDK_Node(hal, j, path) {

	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "INFO") address.info = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG") address.cfg_global = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "ACQ_LEN") address.cfg_acqlen = (uint32_t)r.at("Address");
	}

	data_processing = DATA_PROCESSING::DECODE;
	settings.nchannels = (uint32_t)j.at("Channels");
	settings.nsamples = (uint32_t)j.at("nsamples");
	int ws = (uint32_t)j.at("WordSize");
	settings.usedma = (bool)j.at("UseDMA");
	enabledch = settings.nchannels;
	switch (ws)
	{
	case 0:
		settings.wordsize = 2;
		break;
	default:
		break;
	}
	acq_mode = ACQ_MODE::BLOCKING;
	transfer_size = settings.nchannels * settings.nsamples;
	threaded_buffer_size = 100000;
	__buffer = (uint32_t*)malloc(transfer_size * settings.wordsize * sizeof(uint8_t) * 2);
	threaded = false;
	high_performance = false;

	std::list<int> listOfchannels;
	int q = 0, qi = 0;
	while (qi <= settings.nchannels) {
		qi = 1 << q; q++;
		listOfchannels.push_back(qi);
	}

	RegisterParameter("enabledch", "enabled channels selector", SciSDK_Paramcb::Type::U32, listOfchannels, this);
	RegisterParameter("acq_len", "acquisition length in samples", SciSDK_Paramcb::Type::U32, 2, (double)settings.nsamples * settings.nchannels, this);
	const std::list<std::string> listOfDataProcessing = { "raw","decode" };
	RegisterParameter("data_processing", "set data processing mode", SciSDK_Paramcb::Type::str, listOfDataProcessing, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);
	//RegisterParameter("thread", "enable internal data download thread", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("high_performance", "if true, the internal thread will lock the bus to wait for data", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("threaded_buffer_size", "size of the fifo buffer in number of waves", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	producer.isRunning = false;
	producer.canRun = false;
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
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "thread") {
		if (value == "true") {
			threaded = true;
			return NI_OK;
		}
		else if (value == "false") {
			threaded = false;
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "high_performance") {
		if (value == "true") {
			high_performance = true;
			return NI_OK;
		}
		else if (value == "false") {
			high_performance = false;
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Digitizer::IGetParamU32(string name, uint32_t* value) {
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
	else if (name == "channel_count") {
		*value = (int32_t)settings.nchannels;
		return NI_OK;
	}
	else if (name == "fifo_size") {
		int mult = settings.nchannels > 1 ? 1 : 2;
		*value = (int32_t)settings.nsamples * mult;
		return NI_OK;
	}
	else if (name == "max_ch_samples") {
		int mult = enabledch > 1 ? 1 : 2;
		int scale = settings.nchannels / enabledch;
		*value = (int32_t)settings.nsamples * mult * scale;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Digitizer::IGetParamI32(string name, int32_t* value) {
	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Digitizer::IGetParamString(string name, string* value) {
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
	else if (name == "thread") {
		if (threaded == true) {
			*value = "true";
			return NI_OK;
		}
		else {
			*value = "false";
			return NI_OK;
		}
	}
	else if (name == "high_performance") {
		if (high_performance == true) {
			*value = "true";
			return NI_OK;
		}
		else {
			*value = "false";
			return NI_OK;
		}
	}
	else if (name == "buffer_type") {
		if (data_processing == DATA_PROCESSING::RAW) {
			*value = "SCISDK_DIGITIZER_RAW_BUFFER";
			return NI_OK;
		}
		else if (data_processing == DATA_PROCESSING::DECODE) {
			*value = "SCISDK_DIGITIZER_DECODED_BUFFER";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Digitizer::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer) {

	if (bt == T_BUFFER_TYPE_DECODED) {
		*buffer = (SCISDK_DIGITIZER_DECODED_BUFFER*)malloc(sizeof(SCISDK_DIGITIZER_DECODED_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_DIGITIZER_DECODED_BUFFER* p;
		p = (SCISDK_DIGITIZER_DECODED_BUFFER*)*buffer;
		p->analog = (int32_t*)malloc(sizeof(int32_t) * (settings.nchannels * settings.nsamples + 8));
		if (p->analog == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_DIGITIZER_DECODED;
		p->timecode = 0;
		p->counter = 0;
		p->hits = 0;
		p->user = 0;
		p->info.channels = settings.nchannels;
		p->info.samples = settings.nsamples;
		p->info.valid_samples = 0;
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {

		return NI_INVALID_PARAMETER;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_Digitizer::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size) {

	if (bt == T_BUFFER_TYPE_DECODED) {
		int ret = 0;
		for (int i = 0; i < size; i++) {
			ret |= AllocateBuffer(T_BUFFER_TYPE_DECODED, buffer + sizeof(SCISDK_DIGITIZER_DECODED_BUFFER*) * i);
		}
		return ret;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_DIGITIZER_RAW_BUFFER*)malloc(sizeof(SCISDK_DIGITIZER_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		SCISDK_DIGITIZER_RAW_BUFFER* p;
		p = (SCISDK_DIGITIZER_RAW_BUFFER*)*buffer;
		p->data = (int32_t*)malloc(sizeof(int32_t) * size);
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_DIGITIZER_RAW;
		p->info.channels = settings.nchannels;
		p->info.samples = settings.nsamples;
		p->info.buffer_size = size;
		p->info.valid_samples = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_Digitizer::FreeBuffer(T_BUFFER_TYPE bt, void** buffer) {
	if (bt == T_BUFFER_TYPE_DECODED) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_DIGITIZER_DECODED_BUFFER* p;
		p = (SCISDK_DIGITIZER_DECODED_BUFFER*)*buffer;
		if (p->analog != NULL) {
			free(p->analog);
			p->analog = NULL;
		}

		p->magic = BUFFER_TYPE_INVALID;
		p->timecode = 0;
		p->hits = 0;
		p->user = 0;
		p->counter = 0;
		p->info.channels = 0;
		p->info.samples = 0;
		p->info.valid_samples = 0;
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_RAW) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_DIGITIZER_RAW_BUFFER* p;
		p = (SCISDK_DIGITIZER_RAW_BUFFER*)*buffer;
		if (p->data != NULL) {
			free(p->data);
			p->data = NULL;
		}

		p->magic = BUFFER_TYPE_INVALID;
		p->info.channels = 0;
		p->info.samples = 0;
		p->info.valid_samples = 0;
		p->info.buffer_size = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_Digitizer::ReadData(void* buffer) {
	uint64_t timestamp = 0;
	uint32_t counter = 0;
	uint64_t hits = 0;
	uint32_t user = 0;
	uint32_t header_size = 0;
	int ii = 0;
	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}
	if (data_processing == DATA_PROCESSING::DECODE) {
		SCISDK_DIGITIZER_DECODED_BUFFER* p;
		p = (SCISDK_DIGITIZER_DECODED_BUFFER*)buffer;

		if (p->magic != BUFFER_TYPE_DIGITIZER_DECODED) return NI_INVALID_BUFFER_TYPE;
		if (p->info.channels != settings.nchannels) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.samples != settings.nsamples) return NI_INCOMPATIBLE_BUFFER;

		switch (settings.nchannels) {
		case 1: header_size = 7; break;
		case 2: header_size = 7; break;
		case 4: header_size = 8; break;
		case 8: header_size = 8; break;
		case 16: header_size = 8; break;
		case 32: header_size = 16; break;
		case 64: header_size = 32; break;
		}
		int pQ_minsize = header_size + acq_len * enabledch * 1 / settings.wordsize;

		h_mutex.lock();
		while ((pQ.size() > 0) && (pQ.front() != 0xFFFFFFFF)) pQ.pop();
		h_mutex.unlock();

		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		if (acq_mode == ACQ_MODE::BLOCKING) {
			int s = pQ.size();
			while (!(s >= pQ_minsize) && ((timeout < 0) || (elapsed_time_ms < timeout))) {
				auto t_end = std::chrono::high_resolution_clock::now();
				elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				h_mutex.lock(); s = pQ.size(); h_mutex.unlock();
			}
		}

		if (pQ.size() >= pQ_minsize) {
			h_mutex.lock();
			int pQi = 0;
			int pQs = 0;
			int pQt = enabledch / 2;
			int filler = header_size - 7;
			while (pQi < pQ_minsize) {
				switch (pQi) {
				case 0:
					if (pQ.front() != 0xFFFFFFFF)
						cout << "error in decode data" << endl;
					break;
				case 1:
					timestamp = pQ.front();
					break;
				case 2:
					timestamp += ((uint64_t)(pQ.front())) << 32UL;
					break;
				case 3:
					counter = pQ.front();
					break;
				case 4:
					hits = pQ.front();
					break;
				case 5:
					hits += ((uint64_t)(pQ.front())) << 32UL;
					break;
				case 6:
					user = pQ.front();
					break;
				default:
					if (filler == 0) {
						if (enabledch == 1) {
							p->analog[ii++] = pQ.front() & 0xFFFF;
							p->analog[ii++] = (pQ.front() >> 16) & 0xFFFF;
						}
						else {
							p->analog[ii + (2 * pQs * acq_len)] = pQ.front() & 0xFFFF;
							p->analog[ii + ((2 * pQs + 1) * acq_len)] = (pQ.front() >> 16) & 0xFFFF;
							pQs++;
							if (pQs == pQt) {
								pQs = 0;
								ii++;
							}
						}
					}
					else {
						filler--;
					}
				}

				pQ.pop();
				pQi++;
			}
			p->counter = counter;
			p->hits = hits;
			p->timecode = timestamp;
			p->user = user;
			p->info.valid_samples = acq_len;
			p->info.enabled_channels = enabledch;
			h_mutex.unlock();
			return NI_OK;
		}
		else {
			p->info.valid_samples = 0;
			return NI_NO_DATA_AVAILABLE;
		}


	}
	else if (data_processing == DATA_PROCESSING::RAW) {
		SCISDK_DIGITIZER_RAW_BUFFER* p;
		p = (SCISDK_DIGITIZER_RAW_BUFFER*)buffer;

		if (p->magic != BUFFER_TYPE_DIGITIZER_RAW) return NI_INVALID_BUFFER_TYPE;
		if (p->info.channels != settings.nchannels) return NI_INCOMPATIBLE_BUFFER;
		if (p->info.samples != settings.nsamples) return NI_INCOMPATIBLE_BUFFER;

		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		if (acq_mode == ACQ_MODE::BLOCKING) {
			int s = pQ.size();
			while ((s < p->info.buffer_size) && ((timeout < 0) || (elapsed_time_ms < timeout))) {
				auto t_end = std::chrono::high_resolution_clock::now();
				elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				h_mutex.lock(); s = pQ.size(); h_mutex.unlock();
				cout << "." << endl;
			}
		}


		if (pQ.size() > 0) {
			h_mutex.lock();
			ii = 0;
			while ((pQ.size() > 0) && (ii < p->info.buffer_size)) {
				p->data[ii++] = pQ.front();
				pQ.pop();
			}
			p->info.valid_samples = ii;
			h_mutex.unlock();
			return NI_OK;
		}
		else {
			return NI_NO_DATA_AVAILABLE;
		}

	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}

}

NI_RESULT SciSDK_Digitizer::ExecuteCommand(string cmd, string param) {
	if (cmd == "start") {
		return CmdStart();
	}
	else if (cmd == "stop") {
		return CmdStop();
	}
	return NI_INVALID_COMMAND;
}

NI_RESULT SciSDK_Digitizer::ReadStatus(void* buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_Digitizer::Detach()
{
	return CmdStop();
}

NI_RESULT SciSDK_Digitizer::ConfigureDigitizer() {
	return NI_OK;
}

NI_RESULT SciSDK_Digitizer::CmdStart() {
	if (producer.isRunning) {
		return NI_ALREADY_RUNNING;
	}

	//Critical section : configure and clear list
	h_mutex.lock();
	while (!pQ.empty()) pQ.pop();
	_hal->WriteReg(acq_len - 1, address.cfg_acqlen);
	_hal->WriteReg(2 + (enabledch << 8), address.cfg_global);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	_hal->WriteReg(0 + (enabledch << 8), address.cfg_global);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	_hal->WriteReg(1 + (enabledch << 8), address.cfg_global);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	h_mutex.unlock();

	producer.canRun = true;

	producer.t = new std::thread(&SciSDK_Digitizer::producer_thread, this);
	producer.isRunning = true;
	return NI_OK;
}

NI_RESULT SciSDK_Digitizer::CmdStop() {
	if (!producer.isRunning) {
		return NI_NOT_RUNNING;
	}

	//Critical section : set stop
	producer.canRun = false;

	producer.t->join();

	_hal->WriteReg(0 + (enabledch << 8), address.cfg_global);
	producer.isRunning = false;
	return NI_OK;
}

void SciSDK_Digitizer::producer_thread() {

	while (producer.canRun) {
		uint32_t vd = 0;
		uint32_t _size = 0;
		bool go = false;
		do {
			h_mutex.lock();
			go = pQ.size() < threaded_buffer_size ? true : false;
			h_mutex.unlock();
			if (!go) {
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
			if (!producer.canRun) return;
		} while (go == false);

		if (!high_performance) {
			uint32_t status;
			NI_RESULT ret = _hal->ReadReg(&status, address.status);
			_size = (status >> 8) & 0xFFFFFF;
		}

		else {
			_size = transfer_size;
		}

		_size = _size > transfer_size ? transfer_size : _size;

		if (_size > 0) {
			NI_RESULT ret = _hal->ReadFIFO(__buffer, _size, address.base, address.status, 100, &vd);
			if (ret == NI_OK) {
				if (vd > 0) {
					h_mutex.lock();
					for (int i = 0; i < vd; i++) {
						pQ.push(__buffer[i]);
					}
					h_mutex.unlock();
				}
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}

}