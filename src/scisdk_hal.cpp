#include "scisdk_hal.h"

#ifdef _MSC_VER 
	
#else
	#define __cdecl 
	#include <dlfcn.h>
#endif
//#include "../headers/SCIDK_API_C.h"

NI_RESULT SciSDK_HAL::Connect(string Path, string model) {

	std::transform(model.begin(), model.end(), model.begin(), ::toupper);

	if ((model == "DT1260") || (model == "SCIDK")) {
		_model = BOARD_MODEL::DT1260;
		//load the correct dll
		#ifdef _MSC_VER 
			h_lib_instance = LoadLibrary(L"SCIDK_Lib.dll");
		#else
 			h_lib_instance = dlopen("SCIDK_Lib.so", RTLD_LAZY);
		#endif
		if (h_lib_instance == NULL) {
			cout << "SCIDK_Lib library not loaded ..." << endl;
			return NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY;
		}
		else {
			cout << "SCIDK_Lib library loaded correclty ..." << endl;
		}
		
		//cout << "Last error " << GetLastError() << endl;
	}
	if ((model == "DT5550") || (model == "DT5550W")) {
		_model = BOARD_MODEL::DT5550X;
	}
	if ((model == "R5560") || (model == "R5560SE") || (model == "DT5560")) {
		_model = BOARD_MODEL::X5560;
		#ifdef _MSC_VER 
			h_lib_instance = LoadLibrary(L"R5560_SDKLib.dll");
		#else
			h_lib_instance = dlopen("./libr5560.so", RTLD_LAZY);
			if (!h_lib_instance) {
				/* fail to load the library */
				fprintf(stderr, "Error: %s\n", dlerror());
			}
		#endif
		if (h_lib_instance == NULL) {
			cout << "R5560_SDKLib library not loaded ..." << endl;
			return NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY;
		}
		else {
			cout << "R5560_SDKLib library loaded correclty ..." << endl;
		}

	}
	if ((model == "V2495") || (model == "DT2495")) {
		_model = BOARD_MODEL::X2495;
	}
	if ((model == "V2740") || (model == "DT2740")) {
		_model = BOARD_MODEL::X2495;
	}

	vector<std::string> p = SplitPath(Path, ':');
	switch (_model) {
	case BOARD_MODEL::DT1260:
		// connection to dt1260 board
		if (p[0] == "usb") {
			_handle = malloc(sizeof(NI_HANDLE));
			if (h_lib_instance != NULL) {
				
				#ifdef _MSC_VER 
					typedef int(__cdecl *CONNECT_PROC_PTR)(char*, NI_HANDLE*);
					CONNECT_PROC_PTR connectUSB = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "SCIDK_ConnectUSB");
				#else
					int (*connectUSB)(char*, NI_HANDLE*);
					*(void**)(&connectUSB)  =  dlsym(h_lib_instance, "SCIDK_ConnectUSB");
				#endif
				if (connectUSB) {
					mtx.lock();
					NI_RESULT r = connectUSB((char*)p[1].c_str(), (NI_HANDLE*)_handle);
					mtx.unlock();
					return r;
				}
			}

			return NI_ERROR;
		}
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// connection to x5560 board
		if (h_lib_instance != NULL) {
			_handle = malloc(sizeof(tR5560_Handle));
			#ifdef _MSC_VER 
				typedef int(__cdecl *CONNECT_PROC_PTR)(char* address, int port, tR5560_Handle* handle);
				CONNECT_PROC_PTR connectTCP = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "R5560_ConnectTCP");
			#else
				int (*connectTCP)(char* address, int port, tR5560_Handle* handle);
				*(void**)(&connectTCP)  =  dlsym(h_lib_instance, "R5560_ConnectTCP");
			#endif
			if (connectTCP) {
				if (p[1].find_first_not_of("0123456789") == -1) {
					int port = stoi(p[1]);
					mtx.lock();
					int error_code = connectTCP((char*)p[0].c_str(), port, (tR5560_Handle*)_handle);
					mtx.unlock();
					if (error_code == 0) {
						return NI_OK;
					}
					else {
						return NI_ERROR;
					}
				}
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	
	return NI_OK;
}

NI_RESULT SciSDK_HAL::CloseConnection() {

	switch (_model) {
	case BOARD_MODEL::DT1260:
		// close connection with DT1260 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *CLOSE_CONNECTION_PROC_PTR)(NI_HANDLE*);
				CLOSE_CONNECTION_PROC_PTR close_connection = (CLOSE_CONNECTION_PROC_PTR)GetProcAddress(h_lib_instance, "NI_CloseConnection");
			#else
				int (*close_connection)(NI_HANDLE*);
				*(void**)(&close_connection)  =  dlsym(h_lib_instance, "NI_CloseConnection");
			#endif
			if (close_connection) {
				mtx.lock();
				NI_RESULT r = close_connection((NI_HANDLE*)_handle);
				mtx.unlock();
				#ifdef _MSC_VER 
					FreeLibrary(h_lib_instance);
				#else
					dlclose(h_lib_instance);
				#endif
				free(_handle);
				return r;
			}
		}
		free(_handle);
		return NI_ERROR;

		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// close connection with X5560 board
		if (h_lib_instance != NULL) {
			
			#ifdef _MSC_VER 
				typedef int(__cdecl *CLOSE_CONNECTION_PROC_PTR)(tR5560_Handle*);
				CLOSE_CONNECTION_PROC_PTR close_connection = (CLOSE_CONNECTION_PROC_PTR)GetProcAddress(h_lib_instance, "NI_CloseConnection");
			#else
				int (*close_connection)(tR5560_Handle*);
				*(void**)(&close_connection)  =  dlsym(h_lib_instance, "NI_CloseConnection");
			#endif
			if (close_connection) {
				mtx.lock();
				int res = close_connection((tR5560_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					#ifdef _MSC_VER 
						FreeLibrary(h_lib_instance);
					#else
						dlclose(h_lib_instance);
					#endif
					free(_handle);
					return NI_OK;
				}
				else {
					return NI_ERROR;
				}
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::Enumerate(string board_model, vector<string> devices) {
	switch (_model) {
	case BOARD_MODEL::DT1260:
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::WriteReg(uint32_t value,
	uint32_t address) {

	switch (_model) {
	case BOARD_MODEL::DT1260:
		// write register of DT1260 board
		if (h_lib_instance != NULL) {
			
			#ifdef _MSC_VER 
				typedef int(__cdecl *WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, NI_HANDLE* handle);
				WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteReg");
			#else
				int (*write_reg)(uint32_t value, uint32_t address, NI_HANDLE* handle);
				*(void**)(&write_reg)  =  dlsym(h_lib_instance, "NI_WriteReg");
			#endif
			if (write_reg) {
				mtx.lock();
				NI_RESULT r = write_reg(value, address, (NI_HANDLE*)_handle);
				mtx.unlock();
				return r;
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// write register of X5560 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, tR5560_Handle* handle);
				WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteReg");
			#else
				int (*write_reg)(uint32_t value, uint32_t address, tR5560_Handle* handle);
				*(void**)(&write_reg)  =  dlsym(h_lib_instance, "NI_WriteReg");
			#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(value, address, (tR5560_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
		}
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadReg(uint32_t *value,
	uint32_t address) {

	switch (_model) {
	case BOARD_MODEL::DT1260:
		// read register form DT1260 board
		if (h_lib_instance != NULL) {
			
			#ifdef _MSC_VER 
				typedef int(__cdecl *READ_REG_PROC_PTR)(uint32_t *value, uint32_t address, NI_HANDLE * handle);
				READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadReg");
			#else
				int (*read_reg)(uint32_t *value, uint32_t address, NI_HANDLE * handle);
				*(void**)(&read_reg)  =  dlsym(h_lib_instance, "NI_ReadReg");
			#endif
			if (read_reg) {
				mtx.lock();
				NI_RESULT r = read_reg(value, address, (NI_HANDLE*)_handle);
				mtx.unlock();
				return r;
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// read register from X5560 board
		if (h_lib_instance != NULL) {
			
			#ifdef _MSC_VER 
				typedef int(__cdecl *READ_REG_PROC_PTR)(uint32_t *value, uint32_t address, tR5560_Handle * handle);
				READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadReg");
			#else
				int (*read_reg)(uint32_t *value, uint32_t address, tR5560_Handle * handle);
				*(void**)(&read_reg)  =  dlsym(h_lib_instance, "NI_ReadReg");
			#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(value, address, (tR5560_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::WriteData(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t timeout_ms,
	uint32_t *written_data) {

	switch (_model) {
	case BOARD_MODEL::DT1260:
		// write data to DT1260 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *WRITE_DATA_PROC_PTR)(uint32_t *value, uint32_t length, uint32_t address, uint32_t BusMode, uint32_t timeout_ms, NI_HANDLE * handle, uint32_t *written_data);
				WRITE_DATA_PROC_PTR write_data_proc = (WRITE_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteData");
			#else
				int (*write_data_proc)(uint32_t *value, uint32_t length, uint32_t address, uint32_t BusMode, uint32_t timeout_ms, NI_HANDLE * handle, uint32_t *written_data);
				*(void**)(&write_data_proc)  =  dlsym(h_lib_instance, "NI_WriteData");
			#endif
			if (write_data_proc) {
				mtx.lock();
				int res = write_data_proc(value, length, address, STREAMING, timeout_ms, (NI_HANDLE*)_handle, written_data);
				mtx.unlock();
				if (res) {
					return NI_OK;
				}
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// write data to X5560 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *WRITE_DATA_PROC_PTR)(uint32_t *data, uint32_t count, uint32_t address, tR5560_Handle *handle, uint32_t *written_data);
				WRITE_DATA_PROC_PTR write_data_proc = (WRITE_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteData");
			#else
				int (*write_data_proc)(uint32_t *data, uint32_t count, uint32_t address, tR5560_Handle *handle, uint32_t *written_data);
				*(void**)(&write_data_proc)  =  dlsym(h_lib_instance, "NI_WriteData");
			#endif
			if (write_data_proc) {
				mtx.lock();
				int res = write_data_proc(value, length, address, (tR5560_Handle*)_handle, written_data);
				mtx.unlock();
				if (res) {
					return NI_OK;
				}
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadData(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t timeout_ms,
	uint32_t *read_data) {
	uint32_t rd, vd;
	switch (_model) {
	case BOARD_MODEL::DT1260:
		// write data from DT2160 board
		if (h_lib_instance != NULL) {			
			#ifdef _MSC_VER 
				typedef int(__cdecl *READ_DATA_PROC_PTR)(uint32_t *value, uint32_t length, uint32_t address, uint32_t BusMode, uint32_t timeout_ms, NI_HANDLE * handle, uint32_t *read_data, uint32_t *valid_data);// Pointer to read data function in DLL
				READ_DATA_PROC_PTR read_data_proc = (READ_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadData");
			#else
				int (*read_data_proc)(uint32_t *value, uint32_t length, uint32_t address, uint32_t BusMode, uint32_t timeout_ms, NI_HANDLE * handle, uint32_t *read_data, uint32_t *valid_data);
				*(void**)(&read_data_proc)  =  dlsym(h_lib_instance, "NI_ReadData");
			#endif
			if (read_data_proc) {
				mtx.lock();
				NI_RESULT r = read_data_proc(value, length, address, REG_ACCESS, timeout_ms, (NI_HANDLE*)_handle, &rd, read_data);
				mtx.unlock();
				return r;
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// read data from X5560 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *READ_DATA_PROC_PTR)(uint32_t *data, uint32_t count, uint32_t address, tR5560_Handle *handle, uint32_t *read_data);
				READ_DATA_PROC_PTR read_data_proc = (READ_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadData");
			#else
				int (*read_data_proc)(uint32_t *data, uint32_t count, uint32_t address, tR5560_Handle *handle, uint32_t *read_data);
				*(void**)(&read_data_proc)  =  dlsym(h_lib_instance, "NI_ReadData");

			#endif
			if (read_data_proc) {
				mtx.lock();
				int res = read_data_proc(value, length, address, (tR5560_Handle*)_handle, &rd);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}

	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadFIFO(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t addressStatus,
	uint32_t timeout_ms,
	uint32_t *read_data) {
	uint32_t rd, vd;
	switch (_model) {
	case BOARD_MODEL::DT1260:
		// read FIFO from DT1260 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *READ_FIFO_PROC_PTR)(uint32_t *value, uint32_t length, uint32_t address, uint32_t BusMode, uint32_t timeout_ms, NI_HANDLE * handle, uint32_t *read_data, uint32_t *valid_data);
				READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadFIFO");
			#else
				int (*read_data_proc)(uint32_t *value, uint32_t length, uint32_t address, uint32_t BusMode, uint32_t timeout_ms, NI_HANDLE * handle, uint32_t *read_data, uint32_t *valid_data);
				*(void**)(&read_data_proc)  =  dlsym(h_lib_instance, "NI_ReadFIFO");
			#endif
			if (read_data_proc) {
				mtx.lock();
				NI_RESULT r = read_data_proc(value, length, address, STREAMING, timeout_ms, (NI_HANDLE*)_handle, &rd, read_data);
				mtx.unlock();
				return r;
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		// read FIFO from X5560 board
		if (h_lib_instance != NULL) {
			#ifdef _MSC_VER 
				typedef int(__cdecl *READ_FIFO_PROC_PTR)(uint32_t *data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tR5560_Handle *handle, uint32_t *read_data);
				READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadFIFO");
			#else
				int (*read_data_proc)(uint32_t *data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tR5560_Handle *handle, uint32_t *read_data);
				*(void**)(&read_data_proc)  =  dlsym(h_lib_instance, "NI_ReadFIFO");
			#endif
			if (read_data_proc) {
				mtx.lock();
				NI_RESULT r = read_data_proc(value, length, address, addressStatus, STREAMING, timeout_ms, (tR5560_Handle*)_handle, &rd);
				mtx.unlock();
				return r;
			}
		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}

	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadFIFODMA(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t addressStatus,
	uint32_t timeout_ms,
	uint32_t *read_data) {
	uint32_t rd, vd;
	switch (_model) {
	case BOARD_MODEL::DT1260:
		return NI_NOT_IMPLEMENTED;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}

	return NI_OK;
}

string SciSDK_HAL::ReadFirmwareInformationApp() {

	switch (_model) {
	case BOARD_MODEL::DT1260:
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}

	return NI_OK;
}

std::vector<std::string> SciSDK_HAL::SplitPath(string path, char separator) {
	std::stringstream test(path);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, separator)) {
		seglist.push_back(segment);
	}
	return seglist;
}
