#ifndef HEADER_H_SCISDK_OSCILLOSCOPE
#define HEADER_H_SCISDK_OSCILLOSCOPE
#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>

using json = nlohmann::json;

using namespace std;

class SciSDK_Oscilloscope : public  SciSDK_Node {
public:
	SciSDK_Oscilloscope(SciSDK_HAL *hal, json j, string path);


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
private:
	uint32_t decimator=0;
	uint32_t pretrigger=0;
	uint32_t trigger_channel = 0;
	uint32_t trigger_dtrack = 0;
	uint32_t trigger_level=0;
	int32_t timeout=5000;

bool auto_arm;

enum class TRIGGER_TYPE {
	DISABLED,
	SELF,
	EXT,
	ANALOG,
	DIGITAL_TRACK
} trigger = TRIGGER_TYPE::SELF;




enum class ACQ_MODE {
	BLOCKING,
	NON_BLOCKING
} acq_mode = ACQ_MODE::BLOCKING;



enum class DATA_PROCESSING {
	RAW,
	DECODE
} data_processing=DATA_PROCESSING::DECODE;



enum class TRIGGER_POLARITY {
	POSITIVE,
	NEGATIVE
} trigger_polarity =TRIGGER_POLARITY::POSITIVE;



   NI_RESULT ConfigureOscilloscope();
    NI_RESULT CmdArm();
    NI_RESULT CmdResetReadValidFlag();
    NI_RESULT CheckOscilloscopeStatus(bool *ready, bool *armed, bool *running);



   struct  {
        uint32_t base;
        uint32_t cfg_decimator;
        uint32_t cfg_trigger_mode;
        uint32_t cfg_trigger_level;
        uint32_t cfg_pretrigger;
        uint32_t cmd_arm;
        uint32_t status_read;
        uint32_t status_position;
        uint32_t timestamp;
        bool has_timestamp;
    } address;



   struct {
        uint32_t nsamples;
        uint32_t nchannels;
        uint32_t nanalog;
        uint32_t ndigital;
    } settings;



   uint32_t *__buffer;


};
#endif 