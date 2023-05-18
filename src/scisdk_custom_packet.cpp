#include "scisdk_custom_packet.h"
#include <functional>
#include <chrono>
#include <thread>

/*
DEVICE DRIVER FOR DIGITIZER

IP Compatible version: 1.0

*/

SciSDK_CustomPacket::SciSDK_CustomPacket(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {

	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "READ_STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG") address.config = (uint32_t)r.at("Address");
	}

	check_align_word = false;

	int idx = 0;
	valid_align_word = false;
	for (auto& r : j.at("listOfWord")) {
		if (idx == 0) {
			if (((string)r.at("PioWE")[0].at("Type") == "Constant") && ((string)r.at("PioWE")[0].at("ValueFormat") == "Hex")) {
				string first_word_txt = (string)r.at("PioWE")[0].at("Value");
				cout << "********* " << first_word_txt << endl;
				first_word_const_value = std::stoul(first_word_txt, nullptr, 16);
				valid_align_word = true;
			}
		}
		idx++;
	}

	if (j.contains("UseDMA"))
		settings.usedma = (bool)j.at("UseDMA");
	else
		settings.usedma = false;

	data_processing = DATA_PROCESSING::DECODE;
	acq_mode = ACQ_MODE::BLOCKING;
	settings.acq_len = j.at("BufferLength");
	settings.packet_size = 0;
	for (auto& r : j.at("listOfWord")) {
		settings.packet_size++;
	}

	if (settings.packet_size == 0) settings.packet_size = 1;

	transfer_size = floor((double)settings.acq_len / (double)settings.packet_size)* settings.packet_size;
	threaded_buffer_size = 100000;
	isRunning = false;

	__buffer = NULL;

	timeout = 100;

	RegisterParameter("acq_len", "acquisition length in samples", SciSDK_Paramcb::Type::U32, 2, (double)1024 * 1024 * 1024, this);
	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("thread", "enable internal data download thread", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("high_performance", "if true, the internal thread will lock the bus to wait for data", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("threaded_buffer_size", "size of the fifo buffer in number of waves", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("check_align_word", "if true, check the packet alignment", SciSDK_Paramcb::Type::str, listOfBool, this);

	const std::list<std::string> listOfDataProcessing = { "raw","decode" };
	RegisterParameter("data_processing", "set data processing mode", SciSDK_Paramcb::Type::str, listOfDataProcessing, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	producer.isRunning = false;
	producer.canRun = false;
}


NI_RESULT SciSDK_CustomPacket::ISetParamU32(string name, uint32_t value) {

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
NI_RESULT SciSDK_CustomPacket::ISetParamI32(string name, int32_t value) {
	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_CustomPacket::ISetParamString(string name, string value) {
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
	else if (name == "check_align_word") {
		if (!valid_align_word) {
			return NI_PARAMETER_CAN_NOT_BE_SET_WITH_THIS_CONFIG;
		}
		if (value == "true") {
			check_align_word = true;
			return NI_OK;
		}
		else if (value == "false") {
			check_align_word = false;
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_CustomPacket::IGetParamU32(string name, uint32_t *value) {
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
NI_RESULT SciSDK_CustomPacket::IGetParamI32(string name, int32_t *value) {
	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_CustomPacket::IGetParamString(string name, string *value) {
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
	else if (name == "check_align_word") {
		if (check_align_word == true) {
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
			*value = "SCISDK_CP_RAW_BUFFER";
			return NI_OK;
		}
		else if (data_processing == DATA_PROCESSING::DECODE) {
			*value = "SCISDK_CP_DECODED_BUFFER";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_CustomPacket::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {
	return NI_PARAMETER_OUT_OF_RANGE;
}

NI_RESULT SciSDK_CustomPacket::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer, int size) {
	if (bt == T_BUFFER_TYPE_DECODED) {
		*buffer = (SCISDK_CP_DECODED_BUFFER *)malloc(sizeof(SCISDK_CP_DECODED_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		uint32_t buffer_size = size;
		SCISDK_CP_DECODED_BUFFER *p;
		p = (SCISDK_CP_DECODED_BUFFER*)*buffer;
		p->data = (SCISDK_CP_PACKET*)malloc(sizeof(SCISDK_CP_PACKET) * (buffer_size));
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		for (int i = 0; i < size; i++) {
			p->data[i].n = settings.packet_size;
			p->data[i].row = (uint32_t*)malloc(sizeof(uint32_t) * (p->data[i].n));
			if (p->data[i].row == NULL) {
				return NI_ALLOC_FAILED;
			}
		}

		p->magic = BUFFER_TYPE_CP_DECODED;
		p->info.buffer_size = buffer_size;
		p->info.packet_size = settings.packet_size;
		p->info.valid_data = 0;
		return NI_OK;
	}
	else	if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_CP_RAW_BUFFER *)malloc(sizeof(SCISDK_CP_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		uint32_t buffer_size = size;
		SCISDK_CP_RAW_BUFFER *p;
		p = (SCISDK_CP_RAW_BUFFER*)*buffer;
		p->data = (uint32_t*)malloc(sizeof(int32_t) * (buffer_size + 8));
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_CP_RAW;

		p->info.buffer_size = buffer_size;
		p->info.valid_data = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}
NI_RESULT SciSDK_CustomPacket::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {
	if (bt == T_BUFFER_TYPE_RAW) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_CP_RAW_BUFFER *p;
		p = (SCISDK_CP_RAW_BUFFER*)*buffer;
		if (p->data != NULL) {
			free(p->data);
			p->data = NULL;
		}

		p->magic = BUFFER_TYPE_INVALID;
		p->info.valid_data = 0;
		p->info.buffer_size = 0;
		return NI_OK;
	}
	else if (bt == T_BUFFER_TYPE_DECODED) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}

		SCISDK_CP_DECODED_BUFFER *p;
		p = (SCISDK_CP_DECODED_BUFFER*)*buffer;
		if (p->data != NULL) {
			for (int i = 0; i < p->info.buffer_size; i++) {
				if (p->data[i].row != NULL) {
					free(p->data[i].row);
				}
			}
			free(p->data);
			p->data = NULL;
		}

		p->magic = BUFFER_TYPE_INVALID;
		p->info.buffer_size = 0;
		p->info.valid_data = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_CustomPacket::ReadData(void *buffer) {

	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}


	if (threaded) {
		//Threaded data download. Take data from the internal queue and transfer data 
		//to user without interaction with the hardware.
		if (data_processing == DATA_PROCESSING::DECODE) {
			int exit_code = -1;
			SCISDK_CP_DECODED_BUFFER *p;
			p = (SCISDK_CP_DECODED_BUFFER *)buffer;
			if (p->magic != BUFFER_TYPE_CP_DECODED) return NI_INVALID_BUFFER_TYPE;
			p->info.valid_data = 0;

			auto t_start = std::chrono::high_resolution_clock::now();

repeat_blocking:		
			h_mutex.lock();
			if (pQ.size() > 0) {					
				int ridx = 0;
				bool store;
				if (pQ.size() >= settings.packet_size) {
					while (pQ.size() > 0) {
						if (check_align_word == true) {
							if (ridx == 0) {
								if (pQ.front() == first_word_const_value) {
									if (pQ.size() < settings.packet_size) {
										break;
									}
									store = true;
								}
								else {
									pQ.pop();
									store = false;
								}
							}
							else {

							}
						}
						else {
							store = true;
						}

						if (store == true) {
							p->data[p->info.valid_data].row[ridx] = pQ.front();
							pQ.pop();
							ridx++;
							if (ridx == settings.packet_size) {
								p->info.valid_data++;
								ridx = 0;
								if (pQ.size() < settings.packet_size){	//no more data in the buffer to complete a packet
									break;
								}
								if (p->info.valid_data >= p->info.buffer_size) { // no more space in output buffer
									break;
								}
							}
						}
						
					}

					exit_code = 0;
				}
				else {
					exit_code = -1;
				}
			}
			else {
				exit_code = -1;
			}

			h_mutex.unlock();
			if (acq_mode == ACQ_MODE::NON_BLOCKING) {
				if (exit_code == 0) {
					return NI_OK;
				}
				else {
					return NI_NO_DATA_AVAILABLE;
				}
			}
			else {

				auto t_end = std::chrono::high_resolution_clock::now();
				double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

				if (elapsed_time_ms > timeout) {
					return NI_TIMEOUT;
				}
				else {
					if (p->info.valid_data >= p->info.buffer_size) {
						return NI_OK;
					}
					else {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						goto repeat_blocking;
					}

				}
			}
			
		}
		else {
			auto t_start = std::chrono::high_resolution_clock::now();

			// read raw data
			int exit_code = -1;
			SCISDK_CP_RAW_BUFFER *p;
			p = (SCISDK_CP_RAW_BUFFER *)buffer;
			uint32_t buffer_size_dw = p->info.buffer_size;
			p->info.valid_data = 0;

repeat_blocking_raw:
			h_mutex.lock();
			if (pQ.size() > 0) {
				uint32_t *data;
				data = (uint32_t*)p->data;
				while ((pQ.size() > 0) && (p->info.valid_data < buffer_size_dw)) {
					data[p->info.valid_data++] = pQ.front();
					pQ.pop();
				}
				exit_code = 0;
			}
			h_mutex.unlock();

			if (acq_mode == ACQ_MODE::NON_BLOCKING) {
				if (exit_code == 0) {
					return NI_OK;
				}
				else {
					return NI_NO_DATA_AVAILABLE;
				}
			}
			else {
				auto t_end = std::chrono::high_resolution_clock::now();
				double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

				if (elapsed_time_ms > timeout) {
					return NI_TIMEOUT;
				}
				else {
					if (p->info.valid_data >= p->info.buffer_size) {
						return NI_OK;
					}
					else {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						goto repeat_blocking_raw;
					}
				}
			}
		}
	}
	else {
		//Non threaded mode: data are downloaded under the control of the user
		uint32_t vd;
		uint32_t _size;


		if (data_processing == DATA_PROCESSING::DECODE) {
			SCISDK_CP_DECODED_BUFFER *p;
			p = (SCISDK_CP_DECODED_BUFFER *)buffer;

			if (p->magic != BUFFER_TYPE_CP_DECODED) return NI_INVALID_BUFFER_TYPE;

			uint32_t buffer_size_dw = p->info.buffer_size * p->info.packet_size;

			if (acq_mode == ACQ_MODE::NON_BLOCKING) {
				uint32_t status;
				NI_RESULT ret = _hal->ReadReg(&status, address.status);
				_size = (status >> 8) & 0xFFFFFF;
			}
			else {
				_size = buffer_size_dw;
			}

			_size = buffer_size_dw > _size ? _size : buffer_size_dw;
			_size = floor((double)_size / (double)settings.packet_size) * settings.packet_size;
			if (_size > 0) {
				uint32_t *buffer = (uint32_t*)malloc((_size + 8) * sizeof(uint32_t));
				NI_RESULT ret = _hal->ReadFIFO(buffer, _size, address.base, address.status, timeout, &vd);

				if (vd == 0) {
					free(buffer);
					return NI_NO_DATA_AVAILABLE;
				}

				int pkcount = 0;
				int ridx = 0;
				bool store;
				for (int i = 0; i < vd; i++) {
					if (check_align_word == true) {
						if (ridx == 0) {
							if (buffer[i] == first_word_const_value) {
								store = true;
							}
							else {
								store = false;
							}
						}
						else {

						}
					}
					else {
						store = true;
					}

					if (store == true) {
						p->data[pkcount].row[ridx] = buffer[i];
						ridx++;
						if (ridx == settings.packet_size) {
							pkcount++;
							ridx = 0;
						}
					}
				}
				p->info.valid_data = pkcount;
				free(buffer);
				return NI_OK;
			}
			else {
				return NI_NO_DATA_AVAILABLE;
			}

		}
		else {
			SCISDK_CP_RAW_BUFFER *p;
			p = (SCISDK_CP_RAW_BUFFER *)buffer;

			if (p->magic != BUFFER_TYPE_CP_RAW) return NI_INVALID_BUFFER_TYPE;

			uint32_t buffer_size_dw = p->info.buffer_size;

			if (acq_mode == ACQ_MODE::NON_BLOCKING) {
				uint32_t status;
				NI_RESULT ret = _hal->ReadReg(&status, address.status);
				_size = (status >> 8) & 0xFFFFFF;
			}
			else {
				_size = buffer_size_dw;
			}

			if (_size > 0) {
				int ret = _hal->ReadFIFO(p->data, _size, address.base, address.status, timeout, &vd);
				p->info.valid_data = vd;
				if (vd == 0) {
					cout << "TIMEOUT" << endl;
					return NI_NO_DATA_AVAILABLE;
				}
			}
			else {
				cout << "NO-DATA" << endl;
				return NI_NO_DATA_AVAILABLE;
			}
			return NI_OK;
		}
	}



}

NI_RESULT SciSDK_CustomPacket::ExecuteCommand(string cmd, string param) {
	if (cmd == "start") {
		return CmdStart();
	}
	else if (cmd == "stop") {
		return CmdStop();
	}
	return NI_INVALID_COMMAND;
}
NI_RESULT SciSDK_CustomPacket::ReadStatus(void *buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_CustomPacket::Detach()
{
	return CmdStop();
}

NI_RESULT SciSDK_CustomPacket::ConfigureList() {

	return NI_OK;
}

NI_RESULT SciSDK_CustomPacket::CmdStart() {
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

	if (__buffer)
		free(__buffer);

	//transfer_size = settings.nchannels * settings.acq_len;
	transfer_size = settings.acq_len * settings.packet_size;
	__buffer = (uint32_t*)malloc((transfer_size + 8) * sizeof(uint32_t));

	if (threaded) {
		producer.canRun = true;
		producer.t = new std::thread(&SciSDK_CustomPacket::producer_thread, this);
		producer.isRunning = true;
		isRunning = true;
	}
	else {
		isRunning = true;
	}
	return NI_OK;
}

NI_RESULT SciSDK_CustomPacket::CmdStop() {
	if (!isRunning) {
		return NI_NOT_RUNNING;
	}

	//Critical section : set stop
	producer.canRun = false;
	if (producer.isRunning)
		producer.t->join();

	_hal->WriteReg(0, address.config);
	
	if (__buffer)
		free(__buffer);
		__buffer = NULL;

	producer.isRunning = false;
	isRunning = false;
	return NI_OK;
}

void SciSDK_CustomPacket::producer_thread() {
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

		_size = floor((double)_size / (double)settings.packet_size) * settings.packet_size;

		if (_size > 0) {

			NI_RESULT ret = _hal->ReadFIFO(__buffer, _size, address.base, address.status, timeout, &vd);
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