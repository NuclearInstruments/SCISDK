#ifndef HEADER_H_SCISDK_FRAME
#define HEADER_H_SCISDK_FRAME
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>
#include <thread>  
#include <queue>
#include <atomic> 
using json = nlohmann::json;

using namespace std;

class SciSDK_Frame : public  SciSDK_Node {
public:
	SciSDK_Frame(SciSDK_HAL *hal, json j, string path);


	NI_RESULT ISetParamU32(string name, uint32_t value);
	NI_RESULT ISetParamI32(string name, int32_t value);
	NI_RESULT ISetParamString(string name, string value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
	NI_RESULT IGetParamI32(string name, int32_t *value);
	NI_RESULT IGetParamString(string name, string *value);

	NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer, int size);
	NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void **buffer);

	NI_RESULT ReadData(void *buffer);

	NI_RESULT ExecuteCommand(string cmd, string param);
	NI_RESULT ReadStatus(void *buffer);
private:

	int32_t timeout;
	uint32_t threaded_buffer_size;
	uint32_t transfer_size;
	enum class ACQ_MODE {
		BLOCKING,
		NON_BLOCKING
	} acq_mode;

	enum class DATA_PROCESSING {
		RAW,
		DECODE
	} data_processing;

	enum class TRIGGER_MODE {
		OR,
		AND,
		TRIG,
		SYNCTRIG
	} trigger_mode;

	uint32_t t0mask;

	uint32_t wait_coincidence;

	enum class SYNC_MODE {
		SLAVE,
		MASTER
	} sync_mode;

	enum class DECODE_SM {
		HEADER_1,
		HEADER_2,
		TIMESTAMP_1,
		TIMESTAMP_2,
		COUNT_IN_1,
		COUNT_IN_2,
		COUNT_OUT_1,
		COUNT_OUT_2,
		HITS_1,
		HITS_2,
		PIXELS
	} ;

	struct {
		uint32_t base;
		uint32_t status;
		uint32_t config;
		uint32_t config_trigger_mode;
		uint32_t config_t0mask;
		uint32_t config_wait;
		uint32_t config_sync;
	} address;

	struct {
		int32_t acq_len;
		uint32_t channels;
		uint32_t packet_size;
		bool usedma;
	} settings;

	uint32_t *__buffer;
	bool threaded;
	bool high_performance;
	bool isRunning;
	struct {
		std::thread *t;
		std::atomic<bool> canRun;
		std::atomic<bool> isRunning;
	} producer;

	NI_RESULT Configure();
	NI_RESULT CmdStart();
	NI_RESULT CmdStop();

	void producer_thread();

	std::mutex h_mutex;
	std::queue<uint32_t> pQ;

	uint32_t header_size;
};
#endif 