#ifndef HEADER_H_SCISDK_HAL
#define HEADER_H_SCISDK_HAL

#include <windows.h> 
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
	DT1260,
	DT5550X,
	X5560,
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

#endif