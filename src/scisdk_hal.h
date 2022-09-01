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

	//typedef int(__cdecl *ENUMERATE_PROC_PTR)();// Pointer to enumerate function in DLL
	//typedef int(__cdecl *READ_FIFO_DMA_PROC_PTR)(LPCWSTR);// Pointer to read FIFO DMA function in DLL
	//typedef int(__cdecl *READ_FIRMWARE_INFO_PROC_PTR)(LPCWSTR);// Pointer to read firmware information app function in DLL
};

#endif