// digitizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <SciSDK_DLL.h>
#include <NIErrorCode.h>
using namespace std;



#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#else
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

PACK(typedef struct DATA_STRUCT {
	uint16_t ch1 : 16;
	uint16_t ch0 : 16;
	uint32_t counter : 32;
});

int main()
{
	void* _sdk = SCISDK_InitLib();
	char* res;
	int ret = 0;
	
	ret = SCISDK_AddNewDevice("usb:10500", "dt1260", "ListDT1260.json", "board0", _sdk);
	//ret = SCISDK_AddNewDevice("192.168.90.2:8888", "dt5560", "ListDT5560.json", "board0", _sdk);
	
	if (ret == NI_OK) {
		cout << "Connected to the device." << endl;
	}
	else {
		SCISDK_s_error(ret, &res, _sdk);
		exit(-1);
	}


	SCISDK_LIST_RAW_BUFFER* lrb;
	ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/List_0"), T_BUFFER_TYPE_RAW, (void**)&lrb, _sdk, 100);

	if (ret != NI_OK) {
		printf("Error allocating buffer\n");
		return -1;
	}

	SCISDK_SetParameterString("board0:/MMCComponents/List_0.thread", "false", _sdk);
	SCISDK_SetParameterInteger("board0:/MMCComponents/List_0.timeout", 1000, _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/List_0.acq_mode", "non-blocking", _sdk);

	ret = SCISDK_ExecuteCommand("board0:/MMCComponents/List_0.start", "", _sdk);

	DATA_STRUCT* decoded_data = (DATA_STRUCT*)lrb->data;
	while (1) {
		ret = SCISDK_ReadData("board0:/MMCComponents/List_0", (void*)lrb, _sdk);
		if (ret == NI_OK) {
			for (int i = 0; i < lrb->info.valid_samples; i++) {
				printf("%x ", (uint8_t) lrb->data[i]);
				if (i % 16 == 15) {
					printf("\n");
				}
			}
			printf("\n");
			for (int i = 0; i < lrb->info.valid_samples / sizeof(DATA_STRUCT); i++) {
				printf("%d %d %d\n", decoded_data[i].counter, decoded_data[i].ch0, decoded_data[i].ch1);
			}
			printf("------------------------\n");
		}
	}
}

