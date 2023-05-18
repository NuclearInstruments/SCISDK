#ifndef HEADER_H_SCISDK_DIGITIZER
#define HEADER_H_SCISDK_DIGITIZER
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

class SciSDK_Digitizer : public  SciSDK_Node {
public:
	SciSDK_Digitizer(SciSDK_HAL *hal, json j, string path);


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

	NI_RESULT Detach();
private:

	uint32_t enabledch;
	uint32_t acq_len;
	int32_t timeout;
	uint32_t threaded_buffer_size;
	uint32_t transfer_size;
	bool threaded;
	bool high_performance;
	enum class ACQ_MODE {
		BLOCKING,
		NON_BLOCKING
	} acq_mode;

	enum class DATA_PROCESSING {
		RAW,
		DECODE
	} data_processing;

	struct {
		uint32_t base;
		uint32_t cfg_acqlen;
		uint32_t cfg_global;
		uint32_t status;
		uint32_t info;
	} address;

	struct {
		uint32_t nsamples;
		uint32_t nchannels;
		uint32_t wordsize;
		bool usedma;
	} settings;

	uint32_t *__buffer;

	struct {	
		std::thread *t;
		std::atomic<bool> canRun;
		std::atomic<bool> isRunning;
	} producer;

	NI_RESULT ConfigureDigitizer();
	NI_RESULT CmdStart();
	NI_RESULT CmdStop();

	void producer_thread();

	std::mutex h_mutex;
	std::queue<uint32_t> pQ;
};
#endif 