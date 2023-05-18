#include "scisdk_list.h"
#include <functional>
#include <chrono>
#include <thread>

#ifdef _MSC_VER
#else
#include <unistd.h>
#endif
/*
DEVICE DRIVER FOR DIGITIZER

IP Compatible version: 1.0

*/

SciSDK_List::SciSDK_List(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {

	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG") address.config = (uint32_t)r.at("Address");
	}

	settings.nchannels = (uint32_t)j.at("Channels");
	int ws = (uint32_t)j.at("WordSize");
	if (j.contains("UseDMA"))
		settings.usedma = (bool)j.at("UseDMA");
	else
		settings.usedma = false;

	settings.wordsize = (ws + 1) * 4;

	acq_mode = ACQ_MODE::BLOCKING;
	settings.acq_len = 1024;
	transfer_size = settings.nchannels * settings.acq_len;
	threaded_buffer_size = 100000;
	isRunning = false;

	threaded=false;
	high_performance=false;
	__buffer = NULL;

	timeout = 100;

	RegisterParameter("acq_len", "acquisition length in samples", SciSDK_Paramcb::Type::U32, 2, (double)1024 * 1024 * 1024, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("thread", "enable internal data download thread", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("high_performance", "if true, the internal thread will lock the bus to wait for data", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("threaded_buffer_size", "size of the fifo buffer in number of waves", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	producer.isRunning = false;
	producer.canRun = false;
}


NI_RESULT SciSDK_List::ISetParamU32(string name, uint32_t value) {

	if (name == "acq_len") {
		if (isRunning) return NI_PARAMETER_CAN_NOT_BE_CANGHED_IN_RUN;
		settings.acq_len = value;
		return ConfigureList();
	}
	else if (name == "threaded_buffer_size") {
		if (isRunning) return NI_PARAMETER_CAN_NOT_BE_CANGHED_IN_RUN;
		threaded_buffer_size = value;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_List::ISetParamI32(string name, int32_t value) {
	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_List::ISetParamString(string name, string value) {
	if (name == "acq_mode") {
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
		if (isRunning) return NI_PARAMETER_CAN_NOT_BE_CANGHED_IN_RUN;
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
		if (isRunning) return NI_PARAMETER_CAN_NOT_BE_CANGHED_IN_RUN;
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

NI_RESULT SciSDK_List::IGetParamU32(string name, uint32_t *value) {
	if (name == "acq_len") {
		*value = settings.acq_len;
		return NI_OK;
	}
	else if (name == "threaded_buffer_size") {
		*value = threaded_buffer_size;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_List::IGetParamI32(string name, int32_t *value) {
	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_List::IGetParamString(string name, string *value) {
	if (name == "acq_mode") {
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
		*value = "SCISDK_LIST_RAW_BUFFER";
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_List::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {
	return NI_PARAMETER_OUT_OF_RANGE;
}

NI_RESULT SciSDK_List::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer, int size) {

	if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_LIST_RAW_BUFFER *)malloc(sizeof(SCISDK_LIST_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		uint32_t buffer_size = settings.nchannels * size * settings.wordsize;
		SCISDK_LIST_RAW_BUFFER *p;
		p = (SCISDK_LIST_RAW_BUFFER*)*buffer;
		p->data = (char*)malloc(sizeof(int8_t) * (buffer_size + 8));
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_LIST_RAW;
		p->info.channels = settings.nchannels;
		p->info.samples = size;
		p->info.buffer_size = buffer_size;
		p->info.valid_samples = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}
NI_RESULT SciSDK_List::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {
	if (bt == T_BUFFER_TYPE_RAW) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_LIST_RAW_BUFFER *p;
		p = (SCISDK_LIST_RAW_BUFFER*)*buffer;
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

NI_RESULT SciSDK_List::ReadData(void *buffer) {

	int ii = 0;
	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}

	SCISDK_LIST_RAW_BUFFER *p;
	p = (SCISDK_LIST_RAW_BUFFER *)buffer;

	if (p->magic != BUFFER_TYPE_LIST_RAW) return NI_INVALID_BUFFER_TYPE;
	if (p->info.channels != settings.nchannels) return NI_INCOMPATIBLE_BUFFER;
	uint32_t buffer_size_dw = p->info.buffer_size / 4;

	if (threaded) {
		//Threaded data download. Take data from the internal queue and transfer data 
		//to user without interaction with the hardware.
		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		if (acq_mode == ACQ_MODE::BLOCKING) {
			int s = pQ.size();
			while ((s < buffer_size_dw) && ((timeout < 0) || (elapsed_time_ms < timeout))) {
				auto t_end = std::chrono::high_resolution_clock::now();
				elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
				std::this_thread::sleep_for(std::chrono::microseconds(10));
				h_mutex.lock(); s = pQ.size(); h_mutex.unlock();
			}
		}


		if (pQ.size() > 0) {
			h_mutex.lock();
			ii = 0;
			uint32_t *data;
			data = (uint32_t*)p->data;
			while ((pQ.size() > 0) && (ii < buffer_size_dw)) {
				data[ii++] = pQ.front();
				pQ.pop();
			}
			p->info.valid_samples = ii * 4;
			h_mutex.unlock();
			return NI_OK;
		}
		else {
			return NI_NO_DATA_AVAILABLE;
		}
	}
	else {
		//Non threaded mode: data are downloaded under the control of the user
		uint32_t vd;
		uint32_t _size;
		uint32_t* data;
		uint32_t last_index = 0;
		data = (uint32_t*)p->data;
		p->info.valid_samples = 0;
		
		auto t_start = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = 0;
		
		if (acq_mode == ACQ_MODE::NON_BLOCKING) {
			uint32_t status;
			NI_RESULT ret = _hal->ReadReg(&status, address.status);
			_size = (status >> 8) & 0xFFFFFF;
		}
		else {
			_size = buffer_size_dw;
		}	
		
		_size = buffer_size_dw > _size ? _size : buffer_size_dw;
		uint32_t chunk_size = (settings.nchannels * settings.wordsize) / 4;
		_size = floor(_size / chunk_size) * chunk_size;
		if (_size > 0) {
			while (_size > 0) {
				uint32_t single_transfer_size = 0;
				if (acq_mode == ACQ_MODE::NON_BLOCKING) {
					single_transfer_size = _size;
				} else {
					uint32_t status;
					NI_RESULT ret = _hal->ReadReg(&status, address.status);
					single_transfer_size = (status >> 8) & 0xFFFFFF;
				}
				if (single_transfer_size > 0) {
					NI_RESULT ret = _hal->ReadFIFO(&data[last_index], single_transfer_size, address.base, address.status, timeout, &vd);
					p->info.valid_samples += vd * 4;
					_size = _size - vd;
					last_index += vd;
				}
				if (acq_mode == ACQ_MODE::NON_BLOCKING) {
					if (vd == 0) return NI_NO_DATA_AVAILABLE;
					else return NI_OK;
				}
				else {
					auto t_end = std::chrono::high_resolution_clock::now();
					elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
					if (elapsed_time_ms >= timeout) {
						if (p->info.valid_samples == 0) {
							return NI_NO_DATA_AVAILABLE;
						}
						else {
							return NI_OK;
						}
					}
				}
			}
			return NI_OK;
		}
		else {
			return NI_NO_DATA_AVAILABLE;
		}
	}



}

NI_RESULT SciSDK_List::ExecuteCommand(string cmd, string param) {
	if (cmd == "start") {
		return CmdStart();
	}
	else if (cmd == "stop") {
		return CmdStop();
	}
	return NI_INVALID_COMMAND;
}
NI_RESULT SciSDK_List::ReadStatus(void *buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_List::Detach()
{
	return CmdStop();
}

NI_RESULT SciSDK_List::ConfigureList() {


	return NI_OK;
}

NI_RESULT SciSDK_List::CmdStart() {
	if (isRunning) {
		return NI_ALREADY_RUNNING;
	}

	//Critical section : configure and clear list
	h_mutex.lock();
	while (!pQ.empty()) pQ.pop();
	_hal->WriteReg(2, address.config);
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	_hal->WriteReg(0, address.config);
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	_hal->WriteReg(1, address.config);
	h_mutex.unlock();

	if (__buffer) {
		free(__buffer);
		__buffer = NULL;
	}
	transfer_size = settings.nchannels * settings.acq_len;
	__buffer = (uint32_t *)malloc(transfer_size * settings.wordsize * sizeof(uint8_t) * 2);

	if (threaded) {
		producer.canRun = true;
		producer.t = new std::thread(&SciSDK_List::producer_thread, this);
		producer.isRunning = true;
		isRunning = true;
	}
	else {
		isRunning = true;
	}
	return NI_OK;
}

NI_RESULT SciSDK_List::CmdStop() {
	if (!isRunning) {
		return NI_NOT_RUNNING;
	}

	//Critical section : set stop
	
	producer.canRun = false;
	if (producer.isRunning)
		producer.t->join();

	_hal->WriteReg(0, address.config);

	if (__buffer) {
		free(__buffer);
		__buffer = NULL;
	}
	producer.isRunning = false;
	isRunning = false;
	return NI_OK;
}

void SciSDK_List::producer_thread() {
	bool toTarget = false;
	while (!toTarget && producer.canRun) {
		uint32_t vd = 0;
		uint32_t _size;
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
		uint32_t chunk_size = (settings.nchannels * settings.wordsize) / 4;
		_size = floor(_size / chunk_size) * chunk_size;

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