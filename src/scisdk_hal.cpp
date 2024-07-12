#include "scisdk_hal.h"
#include <stdarg.h>
#ifdef _MSC_VER

#else
	#define __cdecl
	#include <dlfcn.h>
#endif
//#include "../headers/SCIDK_API_C.h"

NI_RESULT SciSDK_HAL::Connect(string Path, string model) {

	std::transform(model.begin(), model.end(), model.begin(), ::toupper);
	if (model == "FAKEBOARD") {
		_model = BOARD_MODEL::FAKEBOARD;
		cout << "FAKEBOARD library loaded ..." << endl;
	}
	if ((model == "DT1260") || (model == "SCIDK")) {
		_model = BOARD_MODEL::DT1260;
		//load the correct dll
		#ifdef _MSC_VER
			h_lib_instance = LoadLibrary(L"SCIDK_Lib.dll");
		#else
 			h_lib_instance = dlopen("libSCIDK.so", RTLD_LAZY);
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
			h_lib_instance = dlopen("libr5560.so", RTLD_LAZY);
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
	if ((model == "DT5771") ) {
		_model = BOARD_MODEL::DT5771;
#ifdef _MSC_VER
		h_lib_instance = LoadLibrary(L"DT5771_SDKLib.dll");
#else
		h_lib_instance = dlopen("libdt5771.so", RTLD_LAZY);
		if (!h_lib_instance) {
			/* fail to load the library */
			fprintf(stderr, "Error: %s\n", dlerror());
		}
#endif
		if (h_lib_instance == NULL) {
			cout << "DT5771_SDKLib library not loaded ..." << endl;
			return NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY;
		}
		else {
			cout << "DT5771_SDKLib library loaded correclty ..." << endl;
		}

	}
	if ((model == "DT4810")) {
		_model = BOARD_MODEL::DT4810;
#ifdef _MSC_VER
		h_lib_instance = LoadLibrary(L"DT4810_SDKLib.dll");
#else
		h_lib_instance = dlopen("libdt4810.so", RTLD_LAZY);
		if (!h_lib_instance) {
			/* fail to load the library */
			fprintf(stderr, "Error: %s\n", dlerror());
		}
#endif
		if (h_lib_instance == NULL) {
			cout << "DT4810_SDKLib library not loaded ..." << endl;
			return NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY;
		}
		else {
			cout << "DT4810_SDKLib library loaded correclty ..." << endl;
		}

	}
	if ((model == "V2495") || (model == "DT2495")) {
		_model = BOARD_MODEL::X2495;
	}
	if ((model == "V2740") || (model == "DT2740") || (model == "V2745") || (model == "DT2745")
		|| (model == "V2730") || (model == "DT2730") ) {
		_model = BOARD_MODEL::X2740;
#ifdef _MSC_VER
		h_lib_instance = LoadLibrary(L"CAEN_FELib.dll");
#else
		h_lib_instance = dlopen("libCAEN_FELib.so", RTLD_LAZY);
		if (!h_lib_instance) {
			/* fail to load the library */
			fprintf(stderr, "Error: %s\n", dlerror());
		}
#endif
		if (h_lib_instance == NULL) {
			cout << "CAEN_FELib library not loaded ..." << endl;
			return NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY;
		}
		else {
			cout << "CAEN_FELib library loaded correclty ..." << endl;
		}
	}

	vector<std::string> p = SplitPath(Path, ':');
	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "FAKEBOARD " << Path << "connected." << endl;
		return NI_OK;
		break;
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
		else {
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
						cout << "Connection successful" << endl;
						return NI_OK;
					}
					else {
						cout << "Connection Error Code: " << error_code << endl;
						return NI_ERROR;
					}
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5771:
		// connection to DT5771 board
		if (h_lib_instance != NULL) {
			_handle = malloc(sizeof(tDT5771_Handle));
#ifdef _MSC_VER
			typedef int(__cdecl* CONNECT_PROC_PTR)(char* address, int port, tDT5771_Handle* handle);
			CONNECT_PROC_PTR connectTCP = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "DT5771_ConnectTCP");
#else
			int (*connectTCP)(char* address, int port, tDT5771_Handle * handle);
			*(void**)(&connectTCP) = dlsym(h_lib_instance, "DT5771_ConnectTCP");
#endif
			if (connectTCP) {
				if (p[1].find_first_not_of("0123456789") == -1) {
					int port = stoi(p[1]);
					mtx.lock();
					int error_code = connectTCP((char*)p[0].c_str(), port, (tDT5771_Handle*)_handle);
					mtx.unlock();
					if (error_code == 0) {
						cout << "Connection successful" << endl;
						return NI_OK;
					}
					else {
						cout << "Connection Error Code: " << error_code << endl;
						return NI_ERROR;
					}
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT4810:
		// connection to DT4810 board
		if (h_lib_instance != NULL) {
			_handle = malloc(sizeof(tDT4810_Handle));
#ifdef _MSC_VER
			typedef int(__cdecl* CONNECT_PROC_PTR)(char* address, int port, tDT4810_Handle* handle);
			CONNECT_PROC_PTR connectTCP = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "DT4810_ConnectTCP");
#else
			int (*connectTCP)(char* address, int port, tDT4810_Handle* handle);
			*(void**)(&connectTCP) = dlsym(h_lib_instance, "DT4810_ConnectTCP");
#endif
			if (connectTCP) {
				if (p[1].find_first_not_of("0123456789") == -1) {
					int port = stoi(p[1]);
					mtx.lock();
					int error_code = connectTCP((char*)p[0].c_str(), port, (tDT4810_Handle*)_handle);
					mtx.unlock();
					if (error_code == 0) {
						cout << "Connection successful" << endl;
						return NI_OK;
					}
					else {
						cout << "Connection Error Code: " << error_code << endl;
						return NI_ERROR;
					}
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		if (h_lib_instance != NULL) {
			_handle = malloc(sizeof(FEELibHandle));
#ifdef _MSC_VER
			typedef int(__stdcall* CONNECT_PROC_PTR)(const char* url, FEELibHandle* handle);
			CONNECT_PROC_PTR connectTCP = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_Open");
#else
			int (*connectTCP)(const char* url, FEELibHandle * handle);
			*(void**)(&connectTCP) = dlsym(h_lib_instance, "CAEN_FELib_Open");
#endif
			if (connectTCP) {
				mtx.lock();
				string url = "dig2://" + p[0];
				int error_code = connectTCP(url.c_str(), (FEELibHandle*)_handle);
				mtx.unlock();


				if (error_code == 0) {
					return NI_OK;
				}
				else {
					return NI_ERROR;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;
	default:
		break;
	}

	return NI_OK;
}

NI_RESULT SciSDK_HAL::CloseConnection() {

	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "FAKEBOARD disconnected." << endl;
		return NI_OK;
		break;

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
			else {
				return NI_INVALID_METHOD;
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
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT5771:
		// close connection with DT5771 board
		if (h_lib_instance != NULL) {

#ifdef _MSC_VER
			typedef int(__cdecl* CLOSE_CONNECTION_PROC_PTR)(tDT5771_Handle*);
			CLOSE_CONNECTION_PROC_PTR close_connection = (CLOSE_CONNECTION_PROC_PTR)GetProcAddress(h_lib_instance, "NI_CloseConnection");
#else
			int (*close_connection)(tDT5771_Handle*);
			*(void**)(&close_connection) = dlsym(h_lib_instance, "NI_CloseConnection");
#endif
			if (close_connection) {
				mtx.lock();
				int res = close_connection((tDT5771_Handle*)_handle);
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
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT4810:
		// close connection with DT4810 board
		if (h_lib_instance != NULL) {

#ifdef _MSC_VER
			typedef int(__cdecl* CLOSE_CONNECTION_PROC_PTR)(tDT4810_Handle*);
			CLOSE_CONNECTION_PROC_PTR close_connection = (CLOSE_CONNECTION_PROC_PTR)GetProcAddress(h_lib_instance, "NI_CloseConnection");
#else
			int (*close_connection)(tDT4810_Handle*);
			*(void**)(&close_connection) = dlsym(h_lib_instance, "NI_CloseConnection");
#endif
			if (close_connection) {
				mtx.lock();
				int res = close_connection((tDT4810_Handle*)_handle);
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
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* CLOSE_CONNECTION_PROC_PTR)(FEELibHandle);
			CLOSE_CONNECTION_PROC_PTR close_connection = (CLOSE_CONNECTION_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_Close");
#else
			int (*close_connection)(FEELibHandle);
			*(void**)(&close_connection) = dlsym(h_lib_instance, "NI_ClosCAEN_FELib_CloseeConnection");
#endif
			if (close_connection) {
				mtx.lock();
				int res = close_connection(*((FEELibHandle*)(_handle)));
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
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::Enumerate(string board_model, vector<string> devices) {
	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "FAKEBOARD found." << endl;
		return NI_OK;
		break;
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
	case BOARD_MODEL::FAKEBOARD:
		cout << "Write reg  @" << address << " Value: " << value << endl;
		return NI_OK;
		break;
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
			else {
				return NI_INVALID_METHOD;
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
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;

	case BOARD_MODEL::DT5771:
		// write register of DT5771 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, tDT5771_Handle* handle);
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteReg");
#else
			int (*write_reg)(uint32_t value, uint32_t address, tDT5771_Handle * handle);
			*(void**)(&write_reg) = dlsym(h_lib_instance, "NI_WriteReg");
#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(value, address, (tDT5771_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;


	case BOARD_MODEL::DT4810:
		// write register of DT4810 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, tDT4810_Handle* handle);
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteReg");
#else
			int (*write_reg)(uint32_t value, uint32_t address, tDT4810_Handle * handle);
			*(void**)(&write_reg) = dlsym(h_lib_instance, "NI_WriteReg");
#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(value, address, (tDT4810_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		// write register of X274X board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* WRITE_REG_PROC_PTR)(FEELibHandle handle, uint32_t address, uint32_t value);
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SetUserRegister");
#else
			int (*write_reg)(FEELibHandle handle, uint32_t address, uint32_t value);
			*(void**)(&write_reg) = dlsym(h_lib_instance, "CAEN_FELib_SetUserRegister");
#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(*((FEELibHandle*)(_handle)), address*4, value);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadReg(uint32_t *value,
	uint32_t address) {

	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "Read reg  @" << address  << endl;
		*value = 0;
		return NI_OK;
		break;
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
			else {
				return NI_INVALID_METHOD;
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
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT5771:
		// read register from DT5771 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_REG_PROC_PTR)(uint32_t* value, uint32_t address, tDT5771_Handle* handle);
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadReg");
#else
			int (*read_reg)(uint32_t * value, uint32_t address, tDT5771_Handle * handle);
			*(void**)(&read_reg) = dlsym(h_lib_instance, "NI_ReadReg");
#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(value, address, (tDT5771_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT4810:
		// read register from DT4810 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_REG_PROC_PTR)(uint32_t* value, uint32_t address, tDT4810_Handle* handle);
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadReg");
#else
			int (*read_reg)(uint32_t * value, uint32_t address, tDT4810_Handle * handle);
			*(void**)(&read_reg) = dlsym(h_lib_instance, "NI_ReadReg");
#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(value, address, (tDT4810_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		// write register of X274X board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* READ_REG_PROC_PTR)(FEELibHandle handle, uint32_t address, uint32_t *value);
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetUserRegister");
#else
			int (*read_reg)(FEELibHandle handle, uint32_t address, uint32_t *value);
			*(void**)(&read_reg) = dlsym(h_lib_instance, "CAEN_FELib_GetUserRegister");
#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(*((FEELibHandle*)(_handle)), address*4, value);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
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
	case BOARD_MODEL::FAKEBOARD:
		cout << "WriteData  @" << address << " Size: " << length << endl;
		return NI_OK;
		break;
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
				if (res==0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
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
				typedef int(__stdcall*WRITE_DATA_PROC_PTR)(uint32_t *data, uint32_t count, uint32_t address, tR5560_Handle *handle, uint32_t *written_data);
				WRITE_DATA_PROC_PTR write_data_proc = (WRITE_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteData");
			#else
				int (*write_data_proc)(uint32_t *data, uint32_t count, uint32_t address, tR5560_Handle *handle, uint32_t *written_data);
				*(void**)(&write_data_proc)  =  dlsym(h_lib_instance, "NI_WriteData");
			#endif
			if (write_data_proc) {
				mtx.lock();
				int res = write_data_proc(value, length, address, (tR5560_Handle*)_handle, written_data);
				mtx.unlock();
				if (res==0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT5771:
		// write data to DT5771 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* WRITE_DATA_PROC_PTR)(uint32_t* data, uint32_t count, uint32_t address, tDT5771_Handle* handle, uint32_t* written_data);
			WRITE_DATA_PROC_PTR write_data_proc = (WRITE_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteData");
#else
			int (*write_data_proc)(uint32_t * data, uint32_t count, uint32_t address, tDT5771_Handle * handle, uint32_t * written_data);
			*(void**)(&write_data_proc) = dlsym(h_lib_instance, "NI_WriteData");
#endif
			if (write_data_proc) {
				mtx.lock();
				int res = write_data_proc(value, length, address, (tDT5771_Handle*)_handle, written_data);
				mtx.unlock();
				if (res==0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT4810:
		// write data to DT4810 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* WRITE_DATA_PROC_PTR)(uint32_t* data, uint32_t count, uint32_t address, tDT4810_Handle* handle, uint32_t* written_data);
			WRITE_DATA_PROC_PTR write_data_proc = (WRITE_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteData");
#else
			int (*write_data_proc)(uint32_t * data, uint32_t count, uint32_t address, tDT4810_Handle * handle, uint32_t * written_data);
			*(void**)(&write_data_proc) = dlsym(h_lib_instance, "NI_WriteData");
#endif
			if (write_data_proc) {
				mtx.lock();
				int res = write_data_proc(value, length, address, (tDT4810_Handle*)_handle, written_data);
				mtx.unlock();
				if (res==0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;


	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		for (int i = 0; i < length; i++) {
			WriteReg(value[i], address + i);
		}
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
	case BOARD_MODEL::FAKEBOARD:

		return NI_OK;
		break;

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
			else {
				return NI_INVALID_METHOD;
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
				*read_data = rd;
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT5771:
		// read data from DT5771 board
		if (h_lib_instance != NULL) {

#ifdef _MSC_VER
			typedef int(__cdecl* READ_DATA_PROC_PTR)(uint32_t* data, uint32_t count, uint32_t address, tDT5771_Handle* handle, uint32_t* read_data);
			READ_DATA_PROC_PTR read_data_proc = (READ_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadData");
#else
			int (*read_data_proc)(uint32_t * data, uint32_t count, uint32_t address, tDT5771_Handle * handle, uint32_t * read_data);
			*(void**)(&read_data_proc) = dlsym(h_lib_instance, "NI_ReadData");

#endif
			if (read_data_proc) {
				mtx.lock();
				int res = read_data_proc(value, length, address, (tDT5771_Handle*)_handle, &rd);
				mtx.unlock();
				*read_data = rd;
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

	}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT4810:
		// read data from DT5771 board
		if (h_lib_instance != NULL) {

#ifdef _MSC_VER
			typedef int(__cdecl* READ_DATA_PROC_PTR)(uint32_t* data, uint32_t count, uint32_t address, tDT4810_Handle* handle, uint32_t* read_data);
			READ_DATA_PROC_PTR read_data_proc = (READ_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadData");
#else
			int (*read_data_proc)(uint32_t * data, uint32_t count, uint32_t address, tDT4810_Handle * handle, uint32_t * read_data);
			*(void**)(&read_data_proc) = dlsym(h_lib_instance, "NI_ReadData");

#endif
			if (read_data_proc) {
				mtx.lock();
				int res = read_data_proc(value, length, address, (tDT4810_Handle*)_handle, &rd);
				mtx.unlock();
				*read_data = rd;
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:

		// read data from X274X board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* FEE_GET_HANDLE_PTR)(FEELibHandle handle, const char* path, uint64_t* pathHandle);
			FEE_GET_HANDLE_PTR CAEN_FELib_GetHandle = (FEE_GET_HANDLE_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetHandle");
			typedef int(__stdcall* FEE_SET_VALUE_PTR)(FEELibHandle handle, const char* path, const char* value);
			FEE_SET_VALUE_PTR CAEN_FELib_SetValue = (FEE_SET_VALUE_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SetValue");
			typedef int(__stdcall* FEE_SEND_CMD_PTR)(FEELibHandle handle, const char* path);
			FEE_SEND_CMD_PTR CAEN_FELib_SendCommand = (FEE_SEND_CMD_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SendCommand");
			typedef int(__cdecl* FEE_READ_DATA_PTR)(uint64_t handle, int timeout, char * data, uint32_t *size);
			FEE_READ_DATA_PTR CAEN_FELib_ReadData = (FEE_READ_DATA_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_ReadData");
#else
			int (*CAEN_FELib_GetHandle)(FEELibHandle handle, const char* path, uint64_t * pathHandle);
			*(void**)(&CAEN_FELib_GetHandle) = dlsym(h_lib_instance, "CAEN_FELib_GetHandle");
			int (*CAEN_FELib_SetValue)(FEELibHandle handle, const char* path, const char* value);
			*(void**)(&CAEN_FELib_SetValue) = dlsym(h_lib_instance, "CAEN_FELib_SetValue");
			int (*CAEN_FELib_SendCommand)(FEELibHandle handle, const char* path);
			*(void**)(&CAEN_FELib_SendCommand) = dlsym(h_lib_instance, "CAEN_FELib_SendCommand");
			int (*CAEN_FELib_ReadData)(FEELibHandle handle, int timeout, char* data, uint32_t *size);
			*(void**)(&CAEN_FELib_ReadData) = dlsym(h_lib_instance, "CAEN_FELib_ReadData");
#endif
			if ((CAEN_FELib_GetHandle) && (CAEN_FELib_SetValue) && (CAEN_FELib_SendCommand) && (CAEN_FELib_ReadData)) {
				uint64_t ep_od_handle;
				*read_data = 0;
				mtx.lock();
				int res = 0;
				uint32_t rd = 0;
				string size_transfer = std::to_string(length);
				string address_transfer = std::to_string(address*4);
				res += CAEN_FELib_GetHandle(*((FEELibHandle*)(_handle)), "/endpoint/opendata", &ep_od_handle);
				res += CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), "/par/opendataaddress", address_transfer.c_str());
				res += CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), "/par/opendatasize", size_transfer.c_str());
				res += CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), "/par/OpenDataMode", "RAM");
				res += CAEN_FELib_SendCommand(*((FEELibHandle*)(_handle)), "/cmd/armacquisition");
				res += CAEN_FELib_SendCommand(*((FEELibHandle*)(_handle)), "/cmd/opendataread");
				res += CAEN_FELib_ReadData(ep_od_handle, timeout_ms, (char*)value, &rd);
				res += CAEN_FELib_SendCommand(*((FEELibHandle*)(_handle)), "/cmd/disarmacquisition");
				*read_data = rd/4;
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}

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
	case BOARD_MODEL::FAKEBOARD:
		cout << "ReadFIFO  @" << address << " Size: " << length << endl;
		return NI_OK;
		break;

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
			else {
				return NI_INVALID_METHOD;
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
				READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadFifo");
			#else
				int (*read_data_proc)(uint32_t *data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tR5560_Handle *handle, uint32_t *read_data);
				*(void**)(&read_data_proc)  =  dlsym(h_lib_instance, "NI_ReadFifo");
			#endif

			if (read_data_proc) {
				mtx.lock();
				int r = read_data_proc(value, length, address, addressStatus, STREAMING, timeout_ms, (tR5560_Handle*)_handle, &rd);
				mtx.unlock();
				*read_data = rd;
				return r;
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT5771:
		// read FIFO from DT5771 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_FIFO_PROC_PTR)(uint32_t* data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tDT5771_Handle* handle, uint32_t* read_data);
			READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadFifo");
#else
			int (*read_data_proc)(uint32_t * data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tDT5771_Handle * handle, uint32_t * read_data);
			*(void**)(&read_data_proc) = dlsym(h_lib_instance, "NI_ReadFifo");
#endif

			if (read_data_proc) {
				mtx.lock();
				int r = read_data_proc(value, length, address, addressStatus, STREAMING, timeout_ms, (tDT5771_Handle*)_handle, &rd);
				mtx.unlock();
				*read_data = rd;
				return r;
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT4810:
		// read FIFO from DT4810 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_FIFO_PROC_PTR)(uint32_t* data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tDT4810_Handle* handle, uint32_t* read_data);
			READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadFifo");
#else
			int (*read_data_proc)(uint32_t * data, uint32_t count, uint32_t address, uint32_t fifo_status_address, uint32_t bus_mode, uint32_t timeout_ms, tDT4810_Handle * handle, uint32_t * read_data);
			*(void**)(&read_data_proc) = dlsym(h_lib_instance, "NI_ReadFifo");
#endif

			if (read_data_proc) {
				mtx.lock();
				int r = read_data_proc(value, length, address, addressStatus, STREAMING, timeout_ms, (tDT4810_Handle*)_handle, &rd);
				mtx.unlock();
				*read_data = rd;
				return r;
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:

		// read fifo from X274X board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* FEE_GET_HANDLE_PTR)(FEELibHandle handle, const char* path, uint64_t* pathHandle);
			FEE_GET_HANDLE_PTR CAEN_FELib_GetHandle = (FEE_GET_HANDLE_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetHandle");
			typedef int(__stdcall* FEE_SET_VALUE_PTR)(FEELibHandle handle, const char* path, const char* value);
			FEE_SET_VALUE_PTR CAEN_FELib_SetValue = (FEE_SET_VALUE_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SetValue");
			typedef int(__stdcall* FEE_SEND_CMD_PTR)(FEELibHandle handle, const char* path);
			FEE_SEND_CMD_PTR CAEN_FELib_SendCommand = (FEE_SEND_CMD_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SendCommand");
			typedef int(__cdecl* FEE_READ_DATA_PTR)(uint64_t handle, int timeout, char* data, uint32_t* size);
			FEE_READ_DATA_PTR CAEN_FELib_ReadData = (FEE_READ_DATA_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_ReadData");
#else
			int (*CAEN_FELib_GetHandle)(FEELibHandle handle, const char* path, uint64_t * pathHandle);
			*(void**)(&CAEN_FELib_GetHandle) = dlsym(h_lib_instance, "CAEN_FELib_GetHandle");
			int (*CAEN_FELib_SetValue)(FEELibHandle handle, const char* path, const char* value);
			*(void**)(&CAEN_FELib_SetValue) = dlsym(h_lib_instance, "CAEN_FELib_SetValue");
			int (*CAEN_FELib_SendCommand)(FEELibHandle handle, const char* path);
			*(void**)(&CAEN_FELib_SendCommand) = dlsym(h_lib_instance, "CAEN_FELib_SendCommand");
			int (*CAEN_FELib_ReadData)(FEELibHandle handle, int timeout, char* data, uint32_t * size);
			*(void**)(&CAEN_FELib_ReadData) = dlsym(h_lib_instance, "CAEN_FELib_ReadData");
#endif
			if ((CAEN_FELib_GetHandle) && (CAEN_FELib_SetValue) && (CAEN_FELib_SendCommand) && (CAEN_FELib_ReadData)) {
				uint32_t avail_word=0;
				uint64_t ep_od_handle;
				*read_data = 0;

				ReadReg(&avail_word, addressStatus);
				avail_word = avail_word >> 8;
				avail_word = avail_word > length ? length : avail_word;
				if (avail_word == 0) {
					*read_data = 0;
					return NI_OK;
				}

				mtx.lock();
				int res = 0;
				uint32_t rd = 0;
				string size_transfer = std::to_string(avail_word);
				string address_transfer = std::to_string(address * 4);
				res += CAEN_FELib_GetHandle(*((FEELibHandle*)(_handle)), "/endpoint/opendata", &ep_od_handle);
				res += CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), "/par/opendataaddress", address_transfer.c_str());
				res += CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), "/par/opendatasize", size_transfer.c_str());
				res += CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), "/par/OpenDataMode", "FIFO");
				res += CAEN_FELib_SendCommand(*((FEELibHandle*)(_handle)), "/cmd/armacquisition");
				res += CAEN_FELib_SendCommand(*((FEELibHandle*)(_handle)), "/cmd/opendataread");
				res += CAEN_FELib_ReadData(ep_od_handle, timeout_ms, (char*)value, &rd);
				res += CAEN_FELib_SendCommand(*((FEELibHandle*)(_handle)), "/cmd/disarmacquisition");
				*read_data = rd / 4;
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}

		break;
	default:
		break;
	}

	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadFIFODMA(
	uint32_t channel,
	uint32_t *data,
	uint32_t length,
	uint32_t *read_data) {
	uint32_t rd;
	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "ReadFIFO-DMA  @" << channel << " Size: " << length << endl;
		return NI_OK;
		break;

	case BOARD_MODEL::DT1260:
		return NI_NOT_IMPLEMENTED;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		//check if length is multiple of 2
		if (length % 2 != 0) {
		 	return NI_MISSALIGNED;
		}
		// read FIFO from X5560 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_FIFO_PROC_PTR)(uint32_t channel, uint8_t* data, uint32_t length, uint32_t* read_data,  tR5560_Handle* handle);
			READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_DMA_Read");
#else
			int (*read_data_proc)(uint32_t channel, uint8_t * data, uint32_t length, uint32_t * read_data, tR5560_Handle * handle);
			*(void**)(&read_data_proc) = dlsym(h_lib_instance, "NI_DMA_Read");
#endif

			if (read_data_proc) {
				mtx.lock();
				int r = read_data_proc(channel, (uint8_t*)data, length*4, &rd, (tR5560_Handle*)_handle);
				mtx.unlock();
				//cout << "ReadFIFO-DMA  @" << channel << " Size: " << length << " Read: " << rd/4 << endl;
				if (rd > length * 4) {
					*read_data = length;
				}
				else {
					*read_data = rd / 4;
				}

				return r;
			}
			else {
				return NI_INVALID_METHOD;
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



NI_RESULT SciSDK_HAL::DMAConfigure(
	uint32_t channel,
	bool blocking,
	uint32_t timeout,
	uint32_t buffer_size) {
	uint32_t rd;
	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "Configure-DMA  @" << channel << " Size: " << buffer_size <<  " Timeout: " << timeout << endl;
		return NI_OK;
		break;

	case BOARD_MODEL::DT1260:
		return NI_NOT_IMPLEMENTED;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		//check if length is multiple of 2
		if (buffer_size % 2 != 0) {
			return NI_MISSALIGNED;
		}
		// read FIFO from X5560 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* DMA_CONFIG_PTR)(uint32_t channel, uint32_t blocking, uint32_t timeout, uint32_t buffer_length, tR5560_Handle* handle);
			DMA_CONFIG_PTR config_proc = (DMA_CONFIG_PTR)GetProcAddress(h_lib_instance, "NI_DMA_SetOptions");
#else
			int (*config_proc)(uint32_t channel, uint32_t blocking, uint32_t timeout, uint32_t buffer_length, tR5560_Handle * handle);
			*(void**)(&config_proc) = dlsym(h_lib_instance, "NI_DMA_SetOptions");
#endif

			if (config_proc) {
				mtx.lock();
				int r = config_proc(channel, blocking?1:0, timeout, buffer_size, (tR5560_Handle*)_handle);
				mtx.unlock();
				return r;
			}
			else {
				return NI_INVALID_METHOD;
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



NI_RESULT SciSDK_HAL::DMAEnable(
	uint32_t channel,
	bool enable) {
	uint32_t rd;
	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "Configure-DMA  @" << channel << " Enable: " << enable << endl;
		return NI_OK;
		break;

	case BOARD_MODEL::DT1260:
		return NI_NOT_IMPLEMENTED;
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:

		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* INTERNAL_REGISTER_WRITE_PTR)(uint32_t data, uint32_t address, tR5560_Handle* handle);
			INTERNAL_REGISTER_WRITE_PTR internal_register_write= (INTERNAL_REGISTER_WRITE_PTR)GetProcAddress(h_lib_instance, "NI_InternalWriteReg");
#else
			int (*internal_register_write)(uint32_t data, uint32_t address, tR5560_Handle * handle);
			*(void**)(&internal_register_write) = dlsym(h_lib_instance, "NI_InternalWriteReg");
#endif

			if (internal_register_write) {
				mtx.lock();
				int r = internal_register_write(enable==true?1:0, 30 << 16, (tR5560_Handle*)_handle);
				mtx.unlock();
				return r;
			}
			else {
				return NI_INVALID_METHOD;
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


string SciSDK_HAL::GetFirmwareTree() {

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
		// gettree
		if (h_lib_instance != NULL) {
			char* jsonString =(char*) malloc(2000000);
			if (jsonString == NULL) return "";
#ifdef _MSC_VER
			typedef int(__stdcall* READ_REG_PROC_PTR)(FEELibHandle handle, char* jsonString, size_t size);
			READ_REG_PROC_PTR CAEN_FELib_GetDeviceTree = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetDeviceTree");
#else
			int (*CAEN_FELib_GetDeviceTree)(FEELibHandle handle, char* jsonString, size_t size);
			*(void**)(&CAEN_FELib_GetDeviceTree) = dlsym(h_lib_instance, "CAEN_FELib_GetDeviceTree");
#endif
			if (CAEN_FELib_GetDeviceTree) {
				mtx.lock();
				int res = CAEN_FELib_GetDeviceTree(*((FEELibHandle*)(_handle)), jsonString, 2000000);
				mtx.unlock();
				if (res != 0) {
					free(jsonString);
					return "";
				}
				string r(jsonString);
				free(jsonString);
				return r;
			}
			else {
				return "";
			}

		}

		break;
	default:
		break;
	}

	return NI_OK;
}



NI_RESULT SciSDK_HAL::SetBoardParamater(string path, string value){

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
		// gettree
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* READ_REG_PROC_PTR)(FEELibHandle handle, const char* path, const char* value);
			READ_REG_PROC_PTR CAEN_FELib_SetValue = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SetValue");
#else
			int (*CAEN_FELib_SetValue)(FEELibHandle handle, const char* path, const char* value);
			*(void**)(&CAEN_FELib_SetValue) = dlsym(h_lib_instance, "CAEN_FELib_SetValue");
#endif
			if (CAEN_FELib_SetValue) {
				mtx.lock();
				int res = CAEN_FELib_SetValue(*((FEELibHandle*)(_handle)), path.c_str(), value.c_str());
				mtx.unlock();
				if (res != 0) {
					return NI_FEELIB_INTERNAL_ERROR + abs(res);
				}
				return NI_OK;
			}
			else {
				return NI_INVALID_METHOD;
			}

		}

		break;
	default:
		break;
	}

	return NI_OK;
}


NI_RESULT SciSDK_HAL::GetBoardParamater(string path, string &value) {

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
		// gettree
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__stdcall* READ_REG_PROC_PTR)(FEELibHandle handle, const char* path, const char* value);
			READ_REG_PROC_PTR CAEN_FELib_GetValue = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetValue");
#else
			int (*CAEN_FELib_GetValue)(FEELibHandle handle, const char* path, const char* value);
			*(void**)(&CAEN_FELib_GetValue) = dlsym(h_lib_instance, "CAEN_FELib_GetValue");
#endif
			if (CAEN_FELib_GetValue) {
				char v[300];
				mtx.lock();
				int res = CAEN_FELib_GetValue(*((FEELibHandle*)(_handle)), path.c_str(),v);
				mtx.unlock();
				if (res != 0) {
					value = "";
					return NI_FEELIB_INTERNAL_ERROR + abs(res);
				}
				value = string(v);
				return NI_OK;
			}
			else {
				return NI_INVALID_METHOD;
			}

		}

		break;
	default:
		break;
	}

	return NI_OK;
}


NI_RESULT SciSDK_HAL::FELib_GetConnectionHandle(uint64_t *handle) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
		*handle =*((uint64_t*) _handle);
	}
	else {
		return NI_ERROR_INTERFACE;
	}
}


NI_RESULT SciSDK_HAL::FELib_GetHandle(uint64_t handle, const char* path, uint64_t* pathHandle) {

	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(FEELibHandle handle, const char* path, uint64_t* pathHandle);
		FN_PTR CAEN_FELib_GetHandle = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetHandle");
#else
		int (*CAEN_FELib_GetHandle)(FEELibHandle handle, const char* path, uint64_t * pathHandle);
		*(void**)(&CAEN_FELib_GetHandle) = dlsym(h_lib_instance, "CAEN_FELib_GetHandle");
#endif
		if (CAEN_FELib_GetHandle) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_GetHandle(handle, path, pathHandle);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}
	}
	else {
		return NI_ERROR_INTERFACE;
	}
}

NI_RESULT SciSDK_HAL::FELib_GetParentHandle(uint64_t handle, const char* path, uint64_t* pathHandle) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(FEELibHandle handle, const char* path, uint64_t* pathHandle);
		FN_PTR CAEN_FELib_GetParentHandle = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetParentHandle");
#else
		int (*CAEN_FELib_GetParentHandle)(FEELibHandle handle, const char* path, uint64_t * pathHandle);
		*(void**)(&CAEN_FELib_GetParentHandle) = dlsym(h_lib_instance, "CAEN_FELib_GetParentHandle");
#endif
		if (CAEN_FELib_GetParentHandle) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_GetParentHandle(handle, path, pathHandle);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}

NI_RESULT SciSDK_HAL::FELib_GetValue(uint64_t handle, const char* path, char value[256]) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(uint64_t handle, const char* path, char value[256]);
		FN_PTR CAEN_FELib_GetValue = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_GetValue");
#else
		int (*CAEN_FELib_GetValue)(uint64_t handle, const char* path, char value[256]);
		*(void**)(&CAEN_FELib_GetValue) = dlsym(h_lib_instance, "CAEN_FELib_GetValue");
#endif
		if (CAEN_FELib_GetValue) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_GetValue(handle, path, value);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}
NI_RESULT SciSDK_HAL::FELib_SetValue(uint64_t handle, const char* path, const char* value) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(uint64_t handle, const char* path, const char* value);
		FN_PTR CAEN_FELib_SetValue = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SetValue");
#else
		int (*CAEN_FELib_SetValue)(uint64_t handle, const char* path, const char* value);
		*(void**)(&CAEN_FELib_SetValue) = dlsym(h_lib_instance, "CAEN_FELib_SetValue");
#endif
		if (CAEN_FELib_SetValue) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_SetValue(handle, path, value);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}
NI_RESULT SciSDK_HAL::FELib_SendCommand(uint64_t handle, const char* path) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(uint64_t handle, const char* path);
		FN_PTR CAEN_FELib_SendCommand = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SendCommand");
#else
		int (*CAEN_FELib_SendCommand)(uint64_t handle, const char* path);
		*(void**)(&CAEN_FELib_SendCommand) = dlsym(h_lib_instance, "CAEN_FELib_SendCommand");
#endif
		if (CAEN_FELib_SendCommand) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_SendCommand(handle, path);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}
NI_RESULT SciSDK_HAL::FELib_SetReadDataFormat(uint64_t handle, const char* jsonString) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(uint64_t handle, const char* jsonString);
		FN_PTR CAEN_FELib_SetReadDataFormat = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_SetReadDataFormat");
#else
		int (*CAEN_FELib_SetReadDataFormat)(uint64_t handle, const char* jsonString);
		*(void**)(&CAEN_FELib_SetReadDataFormat) = dlsym(h_lib_instance, "CAEN_FELib_SetReadDataFormat");
#endif
		if (CAEN_FELib_SetReadDataFormat) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_SetReadDataFormat(handle, jsonString);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}


NI_RESULT SciSDK_HAL::FELib_ReadData(uint64_t handle, int timeout, ...) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(uint64_t handle, int timeout, va_list args);
		FN_PTR CAEN_FELib_ReadDataV = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_ReadDataV");
#else
		int (*CAEN_FELib_ReadDataV)(uint64_t handle, int timeout, va_list args);
		*(void**)(&CAEN_FELib_ReadDataV) = dlsym(h_lib_instance, "CAEN_FELib_ReadDataV");
#endif
		if (CAEN_FELib_ReadDataV) {
			va_list args;

			va_start(args, timeout);

			char v[300];
			mtx.lock();
			int res = CAEN_FELib_ReadDataV(handle, timeout, args);
			mtx.unlock();
			va_end(args);
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}
NI_RESULT SciSDK_HAL::FELib_HasData(uint64_t handle, int timeout) {
	if (_model != BOARD_MODEL::X2740) return NI_NOT_SUPPORTED;

	if (h_lib_instance != NULL) {
#ifdef _MSC_VER
		typedef int(__stdcall* FN_PTR)(uint64_t handle, int timeout);
		FN_PTR CAEN_FELib_HasData = (FN_PTR)GetProcAddress(h_lib_instance, "CAEN_FELib_HasData");
#else
		int (*CAEN_FELib_HasData)(uint64_t handle, int timeout);
		*(void**)(&CAEN_FELib_HasData) = dlsym(h_lib_instance, "CAEN_FELib_HasData");
#endif
		if (CAEN_FELib_HasData) {
			char v[300];
			mtx.lock();
			int res = CAEN_FELib_HasData(handle, timeout);
			mtx.unlock();
			return res;
		}
		else {
			return NI_INVALID_METHOD;
		}

	}
	else {
		return NI_ERROR_INTERFACE;
	}
}



NI_RESULT SciSDK_HAL::ConfigurationRegisterSet(uint32_t value,
	uint32_t address, uint32_t index) {

	switch (_model) {
	case BOARD_MODEL::DT1260:
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		return NI_ERROR;
		break;
	case BOARD_MODEL::X5560:
		// write register of X5560 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, tR5560_Handle* handle);
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_InternalWriteReg");
#else
			int (*write_reg)(uint32_t value, uint32_t address, tR5560_Handle * handle);
			*(void**)(&write_reg) = dlsym(h_lib_instance, "NI_InternalWriteReg");
#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(value, (address << 16) + index, (tR5560_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
		}
			else {
				return NI_INVALID_METHOD;
			}

	}
		break;

	case BOARD_MODEL::DT5771:
		// write register of DT5771 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, tDT5771_Handle* handle);
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_InternalWriteReg");
#else
			int (*write_reg)(uint32_t value, uint32_t address, tDT5771_Handle * handle);
			*(void**)(&write_reg) = dlsym(h_lib_instance, "NI_InternalWriteReg");
#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(value, (address << 16) + index, (tDT5771_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;

	case BOARD_MODEL::DT4810:
		// write register of DT4810 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, tDT4810_Handle* handle);
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_InternalWriteReg");
#else
			int (*write_reg)(uint32_t value, uint32_t address, tDT4810_Handle * handle);
			*(void**)(&write_reg) = dlsym(h_lib_instance, "NI_InternalWriteReg");
#endif
			if (write_reg) {
				mtx.lock();
				int res = write_reg(value, (address << 16) + index, (tDT4810_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		break;
	case BOARD_MODEL::X2495:
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2740:
		return NI_ERROR;
		break;
	default:
		return NI_ERROR;
		break;
	}

	return NI_OK;
}



NI_RESULT SciSDK_HAL::ConfigurationRegisterGet(uint32_t *value,
	uint32_t address, uint32_t index) {

	switch (_model) {
	case BOARD_MODEL::FAKEBOARD:
		cout << "Read reg  @" << address << endl;
		*value = 0;
		return NI_OK;
		break;
	case BOARD_MODEL::DT1260:
		return NI_ERROR;
		break;
	case BOARD_MODEL::DT5550X:
		return NI_ERROR;
		break;
	case BOARD_MODEL::X5560:
		// read register from X5560 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_REG_PROC_PTR)(uint32_t* value, uint32_t address, tR5560_Handle* handle);
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_InternalReadReg");
#else
			int (*read_reg)(uint32_t * value, uint32_t address,  tR5560_Handle * handle);
			*(void**)(&read_reg) = dlsym(h_lib_instance, "NI_InternalReadReg");
#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(value, (address << 16) + index, (tR5560_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT5771:
		// read register from DT5771 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_REG_PROC_PTR)(uint32_t* value, uint32_t address, tDT5771_Handle* handle);
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_InternalReadReg");
#else
			int (*read_reg)(uint32_t * value, uint32_t address, tDT5771_Handle * handle);
			*(void**)(&read_reg) = dlsym(h_lib_instance, "NI_InternalReadReg");
#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(value, (address << 16) + index, (tDT5771_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;

	case BOARD_MODEL::DT4810:
		// read register from DT4810 board
		if (h_lib_instance != NULL) {
#ifdef _MSC_VER
			typedef int(__cdecl* READ_REG_PROC_PTR)(uint32_t* value, uint32_t address, tDT4810_Handle* handle);
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_InternalReadReg");
#else
			int (*read_reg)(uint32_t * value, uint32_t address, tDT4810_Handle * handle);
			*(void**)(&read_reg) = dlsym(h_lib_instance, "NI_InternalReadReg");
#endif
			if (read_reg) {
				mtx.lock();
				int res = read_reg(value, (address << 16) + index, (tDT4810_Handle*)_handle);
				mtx.unlock();
				if (res == 0) {
					return NI_OK;
				}
			}
			else {
				return NI_INVALID_METHOD;
			}

		}
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2495:
		return NI_ERROR;
		break;
	case BOARD_MODEL::X2740:
		return NI_ERROR;
		break;
	default:
		return NI_ERROR;
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

