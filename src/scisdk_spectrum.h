#ifndef HEADER_H_SCISDK_SPECTRUM
#define HEADER_H_SCISDK_SPECTRUM
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>

using json = nlohmann::json;

using namespace std;

class SciSDK_Spectrum : public  SciSDK_Node {
public:
	SciSDK_Spectrum(SciSDK_HAL *hal, json j, string path);


	NI_RESULT ISetParamU32(string name, uint32_t value);
	NI_RESULT ISetParamI32(string name, int32_t value);
	NI_RESULT ISetParamString(string name, string value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
	NI_RESULT IGetParamI32(string name, int32_t *value);
	NI_RESULT IGetParamString(string name, string *value);

	NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer);
	NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void **buffer);

	NI_RESULT ReadData(void *buffer);

	NI_RESULT CheckSpectrum(bool *running, bool *completed, uint32_t *limitcnt, 
							uint32_t *peak_max, uint32_t *total_counter, double *integration_time);
	NI_RESULT ExecuteCommand(string cmd, string param);
	NI_RESULT ReadStatus(void *buffer);

	NI_RESULT Detach();
private:

	enum class LIMIT_TYPE {
		FREE_RUNNING,
		TIME,
		TOTAL_CNT,
		PEAK_CNT
	} spectrum_limit;

	int32_t limit_value=0;
	int32_t rebin=0;
	int32_t emin;
	int32_t emax;
	int32_t current_cfg_reg = 0;


	NI_RESULT ConfigureSpectrum();
	NI_RESULT CmdStart();
	NI_RESULT CmdStop();
	NI_RESULT CmdReset();
	NI_RESULT CmdResetCounters();


	struct  {
		uint32_t base;
		uint32_t cfg;
		uint32_t cfg_limit;
		uint32_t cfg_rebin;
		uint32_t cfg_min;
		uint32_t cfg_max;
		uint32_t status;
		bool has_statustime;
		uint32_t status_time;
		uint32_t timestamp;
		bool has_timestamp;
	} address;

	struct {
		uint32_t nbins;
		uint32_t bitbin;
	} settings;



};
#endif 