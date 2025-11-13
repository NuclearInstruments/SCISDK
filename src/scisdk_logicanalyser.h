#ifndef HEADER_H_SCISDK_LOGICANALYSER
#define HEADER_H_SCISDK_LOGICANALYSER
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>

using json = nlohmann::json;

using namespace std;

class SciSDK_LogicAnalyser : public  SciSDK_Node {
public:
	SciSDK_LogicAnalyser(SciSDK_HAL *hal, json j, string path);

	NI_RESULT ISetParamU32(string name, uint32_t value);
	NI_RESULT ISetParamI32(string name, int32_t value);
	NI_RESULT ISetParamString(string name, string value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
	NI_RESULT IGetParamI32(string name, int32_t *value);
	NI_RESULT IGetParamString(string name, string *value);

	NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void **buffer);

	NI_RESULT ReadData(void *buffer);

	NI_RESULT ExecuteCommand(string cmd, string param);
	NI_RESULT ReadStatus(void *buffer);

	NI_RESULT Detach();
private:
	int32_t timeout = 5000;

	enum class TRIGGER_MODE {
		SOFTWARE,
		SIGNAL
	} trigger_mode = TRIGGER_MODE::SOFTWARE;

	enum class ACQ_MODE {
		BLOCKING,
		NON_BLOCKING
	} acq_mode = ACQ_MODE::BLOCKING;

	NI_RESULT ConfigureLogicAnalyser();
	NI_RESULT CmdStart();
	NI_RESULT CmdReset();
	NI_RESULT CheckLogicAnalyserStatus(uint32_t *status);

	struct {
		uint32_t base;
		uint32_t config;
		uint32_t status;
	} address;

	struct {
		uint32_t nsamples;
		uint32_t ntraces;
		uint32_t words_per_sample;
	} settings;

	uint32_t *__buffer;
	bool isRunning;
};
#endif
