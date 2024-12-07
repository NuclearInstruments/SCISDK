#ifndef HEADER_H_SCISDK_HAL
#define HEADER_H_SCISDK_HAL
#ifdef _MSC_VER 
	#include <windows.h> 
#else
	#define HINSTANCE void *
#endif
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include "NIErrorCode.h"
#include "scisdk_SciElement.h"
#include <mutex>




using namespace std;
#include "json.hpp"
enum class BOARD_MODEL {
	FAKEBOARD,
	DT1260,
	DT5550X,
	X5560,
	DT5771,
	DT4810,
	X2495,
	X2740
};



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

	NI_RESULT SciSDK_HAL::ReadDataDMA(uint8_t* buffer,
		uint32_t length,
		uint32_t dma_channel,
		uint32_t timeout_ms,
		uint32_t* read_data);

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


	NI_RESULT ReadFIFODMA(
		uint32_t channel,
		uint32_t* data,
		uint32_t length,
		uint32_t* read_data);


	NI_RESULT DMAConfigure(
		uint32_t channel,
		bool blocking,
		uint32_t timeout,
		uint32_t buffer_size);

	NI_RESULT DMAEnable(
		uint32_t channel,
		bool enable);

	string ReadFirmwareInformationApp();
	string GetFirmwareTree();

	NI_RESULT SetBoardParamater(string path, string value);
	NI_RESULT GetBoardParamater(string path, string &value);
	NI_RESULT FELib_GetConnectionHandle(uint64_t* handle);
	NI_RESULT FELib_GetHandle(uint64_t handle, const char* path, uint64_t* pathHandle);
	NI_RESULT FELib_GetParentHandle(uint64_t handle, const char* path, uint64_t* pathHandle);
	NI_RESULT FELib_GetValue(uint64_t handle, const char* path, char value[256]);
	NI_RESULT FELib_SetValue(uint64_t handle, const char* path, const char* value);
	NI_RESULT FELib_SendCommand(uint64_t handle, const char* path);
	NI_RESULT FELib_SetReadDataFormat(uint64_t handle, const char* jsonString);

	NI_RESULT FELib_ReadData(uint64_t handle, int timeout,  ... );
	NI_RESULT FELib_HasData(uint64_t handle, int timeout);


	NI_RESULT ConfigurationRegisterSet(uint32_t value, uint32_t address, uint32_t index);
	NI_RESULT ConfigurationRegisterGet(uint32_t *value, uint32_t address, uint32_t index);
private:
	std::vector<std::string> SplitPath(string path, char separator);
	void *_handle;
	BOARD_MODEL _model;
	HINSTANCE h_lib_instance;
	mutex mtx;
};

// DT1260
#define REG_ACCESS  0
#define STREAMING 1
typedef int NI_HANDLE;

// DT5560
typedef enum {
	LOW_LEVEL_TCP = 0
} SOCKET_TYPE;

typedef struct {
	void *zmq_context;
	void *zmq_pullsocket;
	int zmq_connected;
	int recv_blocking;
}tZMQEndpoint;

typedef struct {
	int Csocket;
	int connected;
	uint32_t __IICBASEADDRESS;
	uint32_t __IICBASEADDRESS_STATUS;
	SOCKET_TYPE socketType;
	tZMQEndpoint *zmq;
} tR5560_Handle;

typedef struct {
	int Csocket;
	int connected;
	uint32_t __IICBASEADDRESS;
	uint32_t __IICBASEADDRESS_STATUS;
	SOCKET_TYPE socketType;
	tZMQEndpoint* zmq;
} tDT5771_Handle;

typedef struct {
	int Csocket;
	int connected;
} tDT4810_Handle;

typedef uint64_t FEELibHandle;

#endif