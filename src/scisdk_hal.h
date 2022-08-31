#ifndef HEADER_H_SCISDK_HAL
#define HEADER_H_SCISDK_HAL

#include <windows.h> 
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include "NIErrorCode.h"
#include "scisdk_SciElement.h"

using namespace std;
#include "json.hpp"
enum class BOARD_MODEL {
	DT1260,
	DT5550X,
	X5560,
	X2495,
	X2740
};

#define REG_ACCESS  0
#define STREAMING 1
typedef int NI_HANDLE;

class SciSDK_HAL {

public:

	NI_RESULT Connect(string Path, string model);
	
	NI_RESULT CloseConnection();

	NI_RESULT Enumerate(string board_model, vector<string> devices);

	NI_RESULT WriteReg(uint32_t value,
		uint32_t address);

	NI_RESULT ReadReg(uint32_t *value,
		uint32_t address);

	NI_RESULT WriteData(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t timeout_ms,
		uint32_t *written_data);

	NI_RESULT ReadData(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t timeout_ms,
		uint32_t *read_data);

	NI_RESULT ReadFIFO(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t addressStatus,
		uint32_t timeout_ms,
		uint32_t *read_data);

	NI_RESULT ReadFIFODMA(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t addressStatus,
		uint32_t timeout_ms,
		uint32_t *read_data);

	string ReadFirmwareInformationApp();

private:
	std::vector<std::string> SplitPath(string path, char separator);
	void *_handle;
	BOARD_MODEL _model;
	HINSTANCE h_lib_instance;
	typedef int(__cdecl *CONNECT_PROC_PTR)(char*, NI_HANDLE*);// Pointer to connect function in DLL
	typedef int(__cdecl *CLOSE_CONNECTION_PROC_PTR)(NI_HANDLE*);// Pointer to close connection function in DLL
	typedef int(__cdecl *ENUMERATE_PROC_PTR)();// Pointer to enumerate function in DLL
	typedef int(__cdecl *WRITE_REG_PROC_PTR)(uint32_t value, uint32_t address, NI_HANDLE* handle);// Pointer to write reg function in DLL
	typedef int(__cdecl *READ_REG_PROC_PTR)(uint32_t *value,uint32_t address, NI_HANDLE * handle);// Pointer to read reg function in DLL
	typedef int(__cdecl *WRITE_DATA_PROC_PTR)(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t BusMode,
		uint32_t timeout_ms,
		NI_HANDLE * handle,
		uint32_t *written_data);// Pointer to write data function in DLL
	typedef int(__cdecl *READ_DATA_PROC_PTR)(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t BusMode,
		uint32_t timeout_ms,
		NI_HANDLE * handle,
		uint32_t *read_data,
		uint32_t *valid_data);// Pointer to read data function in DLL
	typedef int(__cdecl *READ_FIFO_PROC_PTR)(uint32_t *value,
		uint32_t length,
		uint32_t address,
		uint32_t BusMode,
		uint32_t timeout_ms,
		NI_HANDLE * handle,
		uint32_t *read_data,
		uint32_t *valid_data);// Pointer to read FIFO function in DLL
	typedef int(__cdecl *READ_FIFO_DMA_PROC_PTR)(LPCWSTR);// Pointer to read FIFO DMA function in DLL
	typedef int(__cdecl *READ_FIRMWARE_INFO_PROC_PTR)(LPCWSTR);// Pointer to read firmware information app function in DLL
};

#endif