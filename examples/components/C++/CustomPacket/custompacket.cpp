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
	uint32_t allign_word : 32;
	uint64_t timestamp : 64;
	uint32_t counter[4] ;
	
});

int main()
{
	void* _sdk = SCISDK_InitLib();
	char* res;
	int ret = 0;
	
	ret = SCISDK_AddNewDevice("usb:10500", "dt1260", "DT1260CustomPacket.json", "board0", _sdk);
	//ret = SCISDK_AddNewDevice("192.168.90.2:8888", "dt5560", "ListDT5560.json", "board0", _sdk);
	
	if (ret == NI_OK) {
		cout << "Connected to the device." << endl;
	}
	else {
		SCISDK_s_error(ret, &res, _sdk);
		cout << res << endl;
		exit(-1);
	}


	SCISDK_CP_DECODED_BUFFER* lrb;


	SCISDK_SetParameterString("board0:/MMCComponents/CP_0.thread", "false", _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/CP_0.data_processing", "decode", _sdk);
	SCISDK_SetParameterInteger("board0:/MMCComponents/CP_0.timeout", 1000, _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/CP_0.acq_mode", "non-blocking", _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/CP_0.check_align_word", "true", _sdk);

	ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/CP_0"), T_BUFFER_TYPE_DECODED, (void**)&lrb, _sdk, 100);

	if (ret != NI_OK) {
		cout << "Error allocating buffer\n" << endl;
		return -1;
	}
	
	ret = SCISDK_ExecuteCommand("board0:/MMCComponents/CP_0.start", "", _sdk);

	
	while (1) {
		ret = SCISDK_ReadData("board0:/MMCComponents/CP_0", (void*)lrb, _sdk);
		if (ret == NI_OK) {
			for (int i = 0; i < lrb->info.valid_data ; i++) {
				DATA_STRUCT* decoded_data = (DATA_STRUCT*)lrb->data[i].row;
				for (int n = 0; n < lrb->data[i].n; n++) {
					//print rows in hexadecimal fixed width 8
					cout << hex << setw(10) << setfill(' ') << lrb->data[i].row[n] << " ";
				}
				printf("\n");
				cout << "Timestamp:   " << decoded_data->timestamp << endl;
				cout << "Counter 0:   " << decoded_data->counter[0] << endl;
				cout << "Counter 1:   " << decoded_data->counter[1] << endl;
				cout << "Counter 2:   " << decoded_data->counter[2] << endl;
				cout << "Counter 3:   " << decoded_data->counter[3] << endl;
				cout << "--------------------------------------------" << endl;
			}
	
		}
	}
}

