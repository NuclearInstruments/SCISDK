// digitizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <SciSDK_DLL.h>
#include <NIErrorCode.h>
using namespace std;
int main()
{
	void* _sdk = SCISDK_InitLib();
	char* res;
	int ret = 0;
	
	ret = SCISDK_AddNewDevice("192.168.90.2:8888", "dt5560", "DigitizerDT5560.json", "board0", _sdk);
	
	if (ret == NI_OK) {
		cout << "Connected to the device." << endl;
	}
	else {
		SCISDK_s_error(ret, &res, _sdk);
		exit(-1);
	}

	int enabled_channels = 2;

	// DIGITIZER decoded
	SCISDK_DIGITIZER_DECODED_BUFFER *ddb;
	SCISDK_AllocateBuffer("board0:/MMCComponents/Digitizer_0", T_BUFFER_TYPE_DECODED, (void**)&ddb, _sdk);
	SCISDK_SetParameterString("board0:/MMCComponents/Digitizer_0.data_processing", "decode", _sdk);
	SCISDK_SetParameterInteger("board0:/MMCComponents/Digitizer_0.enabledch", enabled_channels, _sdk);
	SCISDK_SetParameterInteger("board0:/MMCComponents/Digitizer_0.acq_len", 2000, _sdk);
	SCISDK_SetParameterInteger("board0:/MMCComponents/Digitizer_0.timeout", 1000, _sdk);
	SCISDK_ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", "", _sdk);
	while (1) {
		int ret = SCISDK_ReadData("board0:/MMCComponents/Digitizer_0", (void *)ddb, _sdk);
		if (ret == NI_OK) {
			std::ofstream out("output.txt");
			for (int i = 0; i < ddb->info.valid_samples; i++) {
				for (int ch = 0; ch < enabled_channels; ch++) {
					out << std::setfill(' ') << std::setw(16) << ddb->analog[i + ddb->info.valid_samples*ch];
				}	
				out << endl;
			}
			out.close();
			cout << "Acquisition completed" << endl;
			exit(0);
		}
		else {
			cout << "An error occoured in reading data " << endl;
		}
	}
}

