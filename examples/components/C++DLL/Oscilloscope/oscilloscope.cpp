
#include <iostream>
#include <fstream>
#include <iomanip>
#include <SciSDK_DLL.h>
#include <NIErrorCode.h>
using namespace std;

// Endpoint CP_0 USE DMA
#define ENDPOINT "Oscilloscope_0"
// Endpoint CPXX_0 DO NOT USE DMA
//#define ENDPOINT "CPXX_0"

std::string buildEndpointPath(char* endpoint, const std::string& suffix) {
	return ("board0:/MMCComponents/" + std::string(endpoint) + suffix);
}

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#else
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

PACK(typedef struct DATA_STRUCT {
	uint32_t allign_word : 32;
	uint32_t counter : 32;

});

int main()
{
	void* _sdk = SCISDK_InitLib();
	char* res;
	int ret = 0;

	//ret = SCISDK_AddNewDevice("192.168.102.119:8888", "R5560", "OscilloscopeDT5560.json", "board0", _sdk);
	ret = SCISDK_AddNewDevice("192.168.102.219:8888", "DT5560", "OscilloscopeDT5560.json", "board0", _sdk);

	if (ret == NI_OK) {
		cout << "Connected to the device." << endl;
	}
	else {
		SCISDK_s_error(ret, &res, _sdk);
		cout << res << endl;
		exit(-1);
	}


	SCISDK_OSCILLOSCOPE_DECODED_BUFFER* odb;


	SCISDK_SetParameterString((char*)buildEndpointPath(ENDPOINT, ".data_processing").c_str(), "decode", _sdk);
	SCISDK_SetParameterInteger((char*)buildEndpointPath(ENDPOINT, ".trigger_level").c_str(), 500, _sdk);
	SCISDK_SetParameterString((char*)buildEndpointPath(ENDPOINT, ".trigger_mode").c_str(), "analog", _sdk);
	SCISDK_SetParameterInteger((char*)buildEndpointPath(ENDPOINT, ".trigger_channel").c_str(), 0, _sdk);
	SCISDK_SetParameterInteger((char*)buildEndpointPath(ENDPOINT, ".pretrigger").c_str(), 233, _sdk);
	SCISDK_SetParameterInteger((char*)buildEndpointPath(ENDPOINT, ".decimator").c_str(), 0, _sdk);
	SCISDK_SetParameterString((char*)buildEndpointPath(ENDPOINT, ".acq_mode").c_str(), "blocking", _sdk);

	// When using the DMA list the allocator will allocate a minimum of 2048 word buffer elements
	ret = SCISDK_AllocateBuffer((char*)buildEndpointPath(ENDPOINT, "").c_str(), T_BUFFER_TYPE_DECODED, (void**)&odb, _sdk);

	if (ret != NI_OK) {
		cout << "Error allocating buffer\n" << endl;
		return -1;
	}

	SCISDK_SetRegister("board0:/Registers/HI", uint32_t(30), _sdk);
	SCISDK_SetRegister("board0:/Registers/PERIOD", uint32_t(50), _sdk);




	while (1) {
		ret = SCISDK_ReadData((char*)buildEndpointPath(ENDPOINT, "").c_str(), (void*)odb, _sdk);
		if (ret == NI_OK) {
			for (int i = 0; i < odb->info.samples_analog; i++) {
				cout << odb->analog[i] << endl;
			}
			cout << "-------------------------" << endl;
		}
		else {
			//cout << ".";
		}

	}
}

