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
	uint32_t counter:32 ;
	
});

int main()
{
	void* _sdk = SCISDK_InitLib();
	char* res;
	int ret = 0;
	
	ret = SCISDK_AddNewDevice("192.168.102.119:8888", "R5560", "RegisterFileCPDMA.json", "board0", _sdk);

	if (ret == NI_OK) {
		cout << "Connected to the device." << endl;
	}
	else {
		SCISDK_s_error(ret, &res, _sdk);
		cout << res << endl;
		exit(-1);
	}


	SCISDK_CP_DECODED_BUFFER* lrb;


	SCISDK_SetParameterString("board0:/MMCComponents/CPXX_0.thread", "false", _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/CPXX_0.data_processing", "decode", _sdk);
	SCISDK_SetParameterInteger("board0:/MMCComponents/CPXX_0.timeout", 2000, _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/CPXX_0.acq_mode", "blocking", _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/CPXX_0.check_align_word", "true", _sdk);

	ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/CPXX_0"), T_BUFFER_TYPE_DECODED, (void**)&lrb, _sdk, 10);

	if (ret != NI_OK) {
		cout << "Error allocating buffer\n" << endl;
		return -1;
	}

	SCISDK_SetRegister("board0:/Registers/periodg", uint32_t(10*1e6), _sdk);
	
	ret = SCISDK_ExecuteCommand("board0:/MMCComponents/CPXX_0.start", "", _sdk);

	
	while (1) {
		ret = SCISDK_ReadData("board0:/MMCComponents/CPXX_0", (void*)lrb, _sdk);
		if (ret == NI_OK) {
			for (int i = 0; i < lrb->info.valid_data ; i++) {
				DATA_STRUCT* decoded_data = (DATA_STRUCT*)lrb->data[i].row;
				//for (int n = 0; n < lrb->data[i].n; n++) {
				//	//print rows in hexadecimal fixed width 8
				//	cout << hex << setw(10) << setfill(' ') << lrb->data[i].row[n] << " ";
				//}
				//printf("\n");
				cout << "Counter:  " << decoded_data->counter << endl;
			}
			cout << "-------------------------" << endl;
		}

	}
}

