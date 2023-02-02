#include "scisdk_frame.h"
#include <functional>
#include <chrono>
#include <thread>

/*
DEVICE DRIVER FOR DIGITIZER

IP Compatible version: 1.0

*/

SciSDK_Frame::SciSDK_Frame(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {

	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "READ_STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_ARM") address.config = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_TRIGGER_MODE") address.config_trigger_mode  = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_T0_MASK") address.config_t0mask = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_WAIT") address.config_wait = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_SYNC") address.config_sync = (uint32_t)r.at("Address");
	}


	threaded = true;
	if (j.contains("UseDMA"))
		settings.usedma = (bool)j.at("UseDMA");
	else
		settings.usedma = false;

	data_processing = DATA_PROCESSING::DECODE;
	acq_mode = ACQ_MODE::BLOCKING;
	settings.acq_len = j.at("BufferLength");

	settings.channels = j.at("Channels");
	
	header_size = 9;

	settings.packet_size = settings.channels + header_size;

	transfer_size = floor((double)settings.acq_len / (double)settings.packet_size)* settings.packet_size;
	threaded_buffer_size = 100000;
	isRunning = false;

	__buffer = NULL;

	timeout = 100;
	wait_coincidence = 0;
	sync_mode = SYNC_MODE::MASTER;
	trigger_mode = TRIGGER_MODE::TRIG;
	t0mask = 0x0;

	const std::list<std::string> listOfAcqMode = { "blocking","non-blocking" };
	RegisterParameter("acq_mode", "set data acquisition mode", SciSDK_Paramcb::Type::str, listOfAcqMode, this);
	RegisterParameter("timeout", "set acquisition timeout in blocking mode (ms)", SciSDK_Paramcb::Type::I32, this);
	RegisterParameter("thread", "enable internal data download thread", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("high_performance", "if true, the internal thread will lock the bus to wait for data", SciSDK_Paramcb::Type::str, listOfBool, this);
	RegisterParameter("threaded_buffer_size", "size of the fifo buffer in number of waves", SciSDK_Paramcb::Type::U32, this);

	const std::list<std::string> listOfTrgMode = { "or","and", "trig", "sync_trig" };
	RegisterParameter("trigger_mode", "set acquisition trigger mode", SciSDK_Paramcb::Type::str, listOfTrgMode, this);
	RegisterParameter("trigger_mask", "set bit to 1 to mask specific channel from trigger", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("wait_coincidence", "coincidence window width from trigger (clk cycles) waiting for all channels energy", SciSDK_Paramcb::Type::U32, this);
	const std::list<std::string> listOfSyncMode = { "slave","master" };
	RegisterParameter("sync_mode", "master generate timestamp clock, reset and trigger out, slave use sync clock for timestamp and receive reset and trigger from master", SciSDK_Paramcb::Type::str, listOfSyncMode, this);


	const std::list<std::string> listOfDataProcessing = { "raw","decode" };
	RegisterParameter("data_processing", "set data processing mode", SciSDK_Paramcb::Type::str, listOfDataProcessing, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	producer.isRunning = false;
	producer.canRun = false;
}


NI_RESULT SciSDK_Frame::ISetParamU32(string name, uint32_t value) {

	if (name == "threaded_buffer_size") {
		if (isRunning) return NI_PARAMETER_CAN_NOT_BE_CANGHED_IN_RUN;
		threaded_buffer_size = value;
		return NI_OK;
	}
	else if (name == "trigger_mask") {
		t0mask = value;
		return Configure();
	}
	else if (name == "wait_coincidence") {
		wait_coincidence = value;
		return Configure();
	}
	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Frame::ISetParamI32(string name, int32_t value) {
	if (name == "timeout") {
		timeout = value;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Frame::ISetParamString(string name, string value) {
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
	else if (name == "trigger_mode") {
		if (value == "or") {
			trigger_mode = TRIGGER_MODE::OR;
			return Configure();
		}
		else if (value == "and") {
			trigger_mode = TRIGGER_MODE::AND;
			return Configure();
		}
		else if (value == "trig") {
			trigger_mode = TRIGGER_MODE::TRIG;
			return Configure();
		}
		else if (value == "sync_trig") {
			trigger_mode = TRIGGER_MODE::SYNCTRIG;
			return Configure();
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "sync_mode") {
		if (value == "slave") {
			sync_mode = SYNC_MODE::SLAVE;
			return Configure();
		}
		else if (value == "master") {
			sync_mode = SYNC_MODE::MASTER;
			return Configure();
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Frame::IGetParamU32(string name, uint32_t *value) {
	if (name == "threaded_buffer_size") {
		*value = threaded_buffer_size;
		return NI_OK;
	}
	else if (name == "trigger_mask") {
		*value = t0mask;
		return NI_OK;
	}
	else if (name == "wait_coincidence") {
		*value = wait_coincidence;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Frame::IGetParamI32(string name, int32_t *value) {
	if (name == "timeout") {
		*value = timeout;
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Frame::IGetParamString(string name, string *value) {
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
	else if (name == "buffer_type") {
		if (data_processing == DATA_PROCESSING::RAW) {
			*value = "SCISDK_FRAME_RAW_BUFFER";
			return NI_OK;
		}
		else if (data_processing == DATA_PROCESSING::DECODE) {
			*value = "SCISDK_FRAME_DECODED_BUFFER";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "trigger_mode") {
		if (trigger_mode == TRIGGER_MODE::OR) {
			*value = "or";
			return NI_OK;
		}
		else if (trigger_mode == TRIGGER_MODE::AND) {
			*value = "and";
			return NI_OK;
		}
		else if (trigger_mode == TRIGGER_MODE::TRIG) {
			*value = "trig";
			return NI_OK;
		}
		else if (trigger_mode == TRIGGER_MODE::SYNCTRIG) {
			*value = "sync_trig";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "sync_mode") {
		if (sync_mode == SYNC_MODE::SLAVE) {
			*value = "slave";
			return NI_OK;
		}
		else if (sync_mode == SYNC_MODE::MASTER) {
			*value = "master";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Frame::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {
	return NI_PARAMETER_OUT_OF_RANGE;
}

NI_RESULT SciSDK_Frame::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer, int size) {
	if (bt == T_BUFFER_TYPE_DECODED) {
		*buffer = (SCISDK_FRAME_DECODED_BUFFER *)malloc(sizeof(SCISDK_FRAME_DECODED_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		uint32_t buffer_size = size;
		SCISDK_FRAME_DECODED_BUFFER *p;
		p = (SCISDK_FRAME_DECODED_BUFFER*)*buffer;
		p->data = (SCISDK_FRAME_PACKET*)malloc(sizeof(SCISDK_FRAME_PACKET) * (buffer_size));
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		for (int i = 0; i < size; i++) {
			p->data[i].n = settings.channels;
			p->data[i].pixel = (uint32_t*)malloc(sizeof(uint32_t) * (settings.channels));
			if (p->data[i].pixel == NULL) {
				return NI_ALLOC_FAILED;
			}
			p->data[i].info.event_count = 0;
			p->data[i].info.hits = 0x0;
			p->data[i].info.timestamp = 0x0;
			p->data[i].info.trigger_count = 0x0;
		}

		p->magic = BUFFER_TYPE_FRAME_DECODED;
		p->info.buffer_size = buffer_size;
		p->info.valid_data = 0;
		
		return NI_OK;
	}
	else	if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_FRAME_RAW_BUFFER *)malloc(sizeof(SCISDK_FRAME_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}
		uint32_t buffer_size = size;
		SCISDK_FRAME_RAW_BUFFER *p;
		p = (SCISDK_FRAME_RAW_BUFFER*)*buffer;
		p->data = (uint32_t*)malloc(sizeof(int32_t) * (buffer_size + 8));
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_FRAME_RAW;

		p->info.buffer_size = buffer_size;
		p->info.packet_size = settings.packet_size;
		p->info.valid_data = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}
NI_RESULT SciSDK_Frame::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {
	if (bt == T_BUFFER_TYPE_RAW) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_FRAME_RAW_BUFFER *p;
		p = (SCISDK_FRAME_RAW_BUFFER*)*buffer;
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

		SCISDK_FRAME_DECODED_BUFFER *p;
		p = (SCISDK_FRAME_DECODED_BUFFER*)*buffer;
		if (p->data != NULL) {
			for (int i = 0; i < p->info.buffer_size; i++) {
				if (p->data[i].pixel != NULL) {
					free(p->data[i].pixel);
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

NI_RESULT SciSDK_Frame::ReadData(void *buffer) {

	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}


	if (threaded) {
		//Threaded data download. Take data from the internal queue and transfer data 
		//to user without interaction with the hardware.
		if (data_processing == DATA_PROCESSING::DECODE) {
			int exit_code = -1;
			SCISDK_FRAME_DECODED_BUFFER *p;
			p = (SCISDK_FRAME_DECODED_BUFFER *)buffer;
			if (p->magic != BUFFER_TYPE_FRAME_DECODED) return NI_INVALID_BUFFER_TYPE;
			p->info.valid_data = 0;

			auto t_start = std::chrono::high_resolution_clock::now();

repeat_blocking:		
			h_mutex.lock();
			if (pQ.size() > 0) {					
				int ridx = 0;
				
				if (pQ.size() >= settings.packet_size) {
					DECODE_SM sm;
					sm = DECODE_SM::HEADER_1;
					while (pQ.size() > 0) {
						switch (sm) {
							case DECODE_SM::HEADER_1 : 
								if (pQ.size() < settings.packet_size) {
									break;
								}
								if (pQ.front() == 0xFFFFFFFF){
									sm = DECODE_SM::HEADER_2;
									ridx = 0;
								} 
								break;
							case DECODE_SM::HEADER_2:
								if (pQ.front() == 0x12345678) {
									sm = DECODE_SM::TIMESTAMP_1;
								}
								break;
							case DECODE_SM::TIMESTAMP_1:
								p->data[p->info.valid_data].info.timestamp = ((uint64_t)pQ.front()) << 32UL;
								sm = DECODE_SM::TIMESTAMP_2;
								break;
							case DECODE_SM::TIMESTAMP_2:
								p->data[p->info.valid_data].info.timestamp += ((uint64_t)pQ.front());
								sm = DECODE_SM::COUNT_IN_1;
								break;
							case DECODE_SM::COUNT_IN_1:
								p->data[p->info.valid_data].info.trigger_count = ((uint64_t)pQ.front()) << 32UL;
								sm = DECODE_SM::COUNT_IN_2;
								break;
							case DECODE_SM::COUNT_IN_2:
								p->data[p->info.valid_data].info.trigger_count += ((uint64_t)pQ.front());
								sm = DECODE_SM::COUNT_OUT_1;
								break;
							case DECODE_SM::COUNT_OUT_1:
								p->data[p->info.valid_data].info.event_count = ((uint64_t)pQ.front()) << 32UL;
								sm = DECODE_SM::COUNT_OUT_2;
								break;
							case DECODE_SM::COUNT_OUT_2:
								p->data[p->info.valid_data].info.event_count += ((uint64_t)pQ.front());
								sm = DECODE_SM::HITS_1;
								break;
							case DECODE_SM::HITS_1:
								p->data[p->info.valid_data].info.hits = ((uint64_t)pQ.front());
								if (settings.channels > 32) {
									sm = DECODE_SM::HITS_2;
								}
								else {
									sm = DECODE_SM::PIXELS;
								}
								break;
							case DECODE_SM::HITS_2:
								p->data[p->info.valid_data].info.hits += ((uint64_t)pQ.front()) << 32UL;
								sm = DECODE_SM::PIXELS;
								break;
							case DECODE_SM::PIXELS:
								p->data[p->info.valid_data].pixel[ridx++] = pQ.front();
								if (ridx == settings.channels) {
									p->info.valid_data++;
									sm = DECODE_SM::HEADER_1;
								}
								else {
									sm = DECODE_SM::PIXELS;
								}

								break;
						}

						pQ.pop();
						if (p->info.valid_data >= p->info.buffer_size) { // no more space in output buffer
							break;
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
			SCISDK_FRAME_RAW_BUFFER *p;
			p = (SCISDK_FRAME_RAW_BUFFER *)buffer;
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
			SCISDK_FRAME_DECODED_BUFFER *p;
			p = (SCISDK_FRAME_DECODED_BUFFER *)buffer;

			if (p->magic != BUFFER_TYPE_FRAME_DECODED) return NI_INVALID_BUFFER_TYPE;

			uint32_t buffer_size_dw = p->info.buffer_size * settings.packet_size;

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
				auto t_start = std::chrono::high_resolution_clock::now();
				uint32_t *buffer = (uint32_t*)malloc((_size + 8) * sizeof(uint32_t));
				if (buffer == NULL) return NI_ALLOC_FAILED;
				uint32_t vdd = 0;
				uint32_t single_transfer_size = 0;
				while (_size > 0) {
					if (acq_mode == ACQ_MODE::NON_BLOCKING) {
						single_transfer_size = _size;
					}
					else {
						uint32_t status;
						NI_RESULT ret = _hal->ReadReg(&status, address.status);
						single_transfer_size = (status >> 8) & 0xFFFFFF;
					}
					if (single_transfer_size > 0) {
						NI_RESULT ret = _hal->ReadFIFO(&buffer[vdd], single_transfer_size, address.base, address.status, timeout, &vd);
						_size = _size - vd;
						vdd += vd;
					}
					if (acq_mode == ACQ_MODE::NON_BLOCKING) {

					}
					else {
						auto t_end = std::chrono::high_resolution_clock::now();
						double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
						if (elapsed_time_ms >= timeout) {
							break;
						}
					}
				}
				//NI_RESULT ret = _hal->ReadFIFO(buffer, _size, address.base,  address.status, timeout, &vd);

				if (vdd == 0) {
					free(buffer);
					return NI_NO_DATA_AVAILABLE;
				}

				p->info.valid_data = 0;
				int ridx = 0;
				DECODE_SM sm;
				sm = DECODE_SM::HEADER_1;
				for (int i = 0; i < vdd; i++) {
					switch (sm) {
					case DECODE_SM::HEADER_1:
						if (buffer[i] == 0xFFFFFFFF) {
							sm = DECODE_SM::HEADER_2;
							ridx = 0;
						}
						break;
					case DECODE_SM::HEADER_2:
						if (buffer[i] == 0x12345678) {
							sm = DECODE_SM::TIMESTAMP_1;
						}
						break;
					case DECODE_SM::TIMESTAMP_1:
						p->data[p->info.valid_data].info.timestamp = ((uint64_t)buffer[i]) << 32UL;
						sm = DECODE_SM::TIMESTAMP_2;
						break;
					case DECODE_SM::TIMESTAMP_2:
						p->data[p->info.valid_data].info.timestamp += ((uint64_t)buffer[i]);
						sm = DECODE_SM::COUNT_IN_1;
						break;
					case DECODE_SM::COUNT_IN_1:
						p->data[p->info.valid_data].info.trigger_count = ((uint64_t)buffer[i]) << 32UL;
						sm = DECODE_SM::COUNT_IN_2;
						break;
					case DECODE_SM::COUNT_IN_2:
						p->data[p->info.valid_data].info.trigger_count += ((uint64_t)buffer[i]);
						sm = DECODE_SM::COUNT_OUT_1;
						break;
					case DECODE_SM::COUNT_OUT_1:
						p->data[p->info.valid_data].info.event_count = ((uint64_t)buffer[i]) << 32UL;
						sm = DECODE_SM::COUNT_OUT_2;
						break;
					case DECODE_SM::COUNT_OUT_2:
						p->data[p->info.valid_data].info.event_count += ((uint64_t)buffer[i]);
						sm = DECODE_SM::HITS_1;
						break;
					case DECODE_SM::HITS_1:
						p->data[p->info.valid_data].info.hits = ((uint64_t)buffer[i]);
						if (settings.channels > 32) {
							sm = DECODE_SM::HITS_2;
						}
						else {
							sm = DECODE_SM::PIXELS;
						}
						break;
					case DECODE_SM::HITS_2:
						p->data[p->info.valid_data].info.hits += ((uint64_t)buffer[i]) << 32UL;
						sm = DECODE_SM::PIXELS;
						break;
					case DECODE_SM::PIXELS:
						p->data[p->info.valid_data].pixel[ridx++] = buffer[i];
						if (ridx == settings.channels) {
							p->info.valid_data++;
							sm = DECODE_SM::HEADER_1;
						}
						else {
							sm = DECODE_SM::PIXELS;
						}
						break;
					}
				}
				free(buffer);
				return NI_OK;
			}
			else {
				return NI_NO_DATA_AVAILABLE;
			}

		}
		else {
			SCISDK_FRAME_RAW_BUFFER *p;
			p = (SCISDK_FRAME_RAW_BUFFER *)buffer;

			if (p->magic != BUFFER_TYPE_FRAME_RAW) return NI_INVALID_BUFFER_TYPE;

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
					return NI_NO_DATA_AVAILABLE;
				}
			}
			else {
				return NI_NO_DATA_AVAILABLE;
			}
			return NI_OK;
		}

	}



}

NI_RESULT SciSDK_Frame::ExecuteCommand(string cmd, string param) {
	if (cmd == "start") {
		return CmdStart();
	}
	else if (cmd == "stop") {
		return CmdStop();
	}
	return NI_INVALID_COMMAND;
}
NI_RESULT SciSDK_Frame::ReadStatus(void *buffer) {

	return NI_OK;
}

NI_RESULT SciSDK_Frame::Configure() {
	switch (trigger_mode) {
	case TRIGGER_MODE::OR:
		_hal->WriteReg(0, address.config_trigger_mode);
		break;
	case TRIGGER_MODE::AND:
		_hal->WriteReg(1, address.config_trigger_mode);
		break;
	case TRIGGER_MODE::TRIG:
		_hal->WriteReg(2, address.config_trigger_mode);
		break;
	case TRIGGER_MODE::SYNCTRIG:
		_hal->WriteReg(4, address.config_trigger_mode);
		break;
	}

	switch (sync_mode) {
	case SYNC_MODE::MASTER:
		_hal->WriteReg(0, address.config_sync);
		break;
	case SYNC_MODE::SLAVE:
		_hal->WriteReg(1, address.config_sync);
		break;
	}

	_hal->WriteReg(t0mask, address.config_t0mask);
	_hal->WriteReg(wait_coincidence, address.config_wait );
	return NI_OK;
}

NI_RESULT SciSDK_Frame::CmdStart() {
	if (isRunning) {
		return NI_ALREADY_RUNNING;
	}

	Configure();
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
		producer.t = new std::thread(&SciSDK_Frame::producer_thread, this);
		producer.isRunning = true;
		isRunning = true;
	}
	else {
		isRunning = true;
	}
	return NI_OK;
}

NI_RESULT SciSDK_Frame::CmdStop() {
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

void SciSDK_Frame::producer_thread() {
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