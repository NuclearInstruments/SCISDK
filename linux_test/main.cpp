#include <iostream>
#include "SciSDK_DLL.h"
#include <scisdk_defines.h>
#include <unistd.h>


using namespace std;

int main(int argc, char* argv[]) {
	char* str_tmp = "";
	int int_tmp = -1;
  	void* _sdk = SCISDK_InitLib();
	char * res = "";
	int ret = 0;

	SCISDK_s_error(SCISDK_AddNewDevice("192.168.102.219:8888", "dt5560", "RegisterFile.json", "board0", _sdk), &res, _sdk);
	cout << res << endl << "-*-*-*-*-*-*-" << endl;

	SCISDK_CP_RAW_BUFFER *cprb;
	ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/CP_0"), 0, (void**)&cprb, _sdk, 1024);
	cout << ret << endl << "-*-*-*-1-*-*-" << endl;
	ret = SCISDK_SetParameterString((char*)("board0:/MMCComponents/CP_0.thread"), "false", _sdk);
	cout << ret << endl << "-*-*-*-2-*-*-" << endl;
	ret = SCISDK_SetParameterString((char*)("board0:/MMCComponents/CP_0.data_processing"), "raw", _sdk);
	cout << ret << endl << "-*-*-*-2b-*-*-" << endl;
	ret = SCISDK_SetParameterInteger((char*)("board0:/MMCComponents/CP_0.timeout"), 5000, _sdk);
	cout << ret << endl << "-*-*-*-3-*-*-" << endl;
	ret = SCISDK_SetParameterString((char*)("board0:/MMCComponents/CP_0.acq_mode"), "blocking", _sdk);
	cout << ret << endl << "-*-*-*-4-*-*-" << endl;
	ret = SCISDK_ExecuteCommand((char*)("board0:/MMCComponents/CP_0.start"), "", _sdk);
	cout << ret << endl << "-*-*-*-5-*-*-" << endl;
	ret = SCISDK_ReadData((char*)("board0:/MMCComponents/CP_0"), (void *)cprb, _sdk);
	cout << ret << endl << "-*-*-*-6-*-*-" << endl;	
	
	
	if (ret==0) {
		int s = cprb->info.valid_data/4;
		uint32_t *pp;
		pp = (uint32_t*) cprb->data;
		for (int i =0;i<255;i++) {
			cout << i << " " << pp[i] <<endl;
		}
	}

	cout  << endl << "-*-*-*-6b-*-*-" << endl;
	ret = SCISDK_ExecuteCommand((char*)("board0:/MMCComponents/CP_0.stop"), "", _sdk);
	cout << ret << endl << "-*-*-*-7-*-*-" << endl;	
	ret = SCISDK_FreeBuffer((char*)("board0:/MMCComponents/CP_0"), 0, (void**)&cprb, _sdk);
	cout << ret << endl << "-*-*-*-8-*-*-" << endl;	

	return 0;

	SCISDK_SPECTRUM_DECODED_BUFFER *spb;
	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Spectrum_0", 1, (void**)&spb, _sdk), &res, _sdk);
	
	SCISDK_s_error(SCISDK_ReadData("board0:/MMCComponents/Spectrum_0", (void *)spb, _sdk), &res, _sdk);
	//	for (int i=0;i<10;i++)
	//		cout << ob->analog[i] << " ";
	cout << spb->info.valid_bins;
	cout << endl;

	/////////////////////////////////////////////////////////////////
	cout << "*****************" <<endl;
	SCISDK_SPECTRUM_STATUS *spe_status;
	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Spectrum_0", 1, (void**)&spe_status, _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "", _sdk), &res, _sdk);	
	sleep(2);
	SCISDK_s_error(SCISDK_ReadStatus("board0:/MMCComponents/Spectrum_0", (void *)spe_status, _sdk), &res, _sdk);
	cout << spe_status->integration_time<< endl;
	cout << spe_status->progress<< endl;
	cout << spe_status->total_counter<< endl;
	cout << "*****************" <<endl;
	return 0;

	/////////////////////////////////////////////////////////////////




	SCISDK_LIST_RAW_BUFFER *lsb;

	SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/List_0.start", "", _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/List_0", 1000, (void**)&lsb, _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_ReadData("board0:/MMCComponents/List_0", (void *)lsb, _sdk), &res, _sdk);
	cout << "buffer_size " << lsb->info.buffer_size << endl;
	cout << "channels " << lsb->info.channels << endl;
	cout << "samples " << lsb->info.samples << endl;
	cout << "valid_samples " << lsb->info.valid_samples << endl;
	for (int i=0;i<10;i++){
			cout << lsb->data[i] << " ";
	}
	cout << endl;
	
	SCISDK_s_error(SCISDK_GetComponentList("board0", "ALL", &str_tmp, true, _sdk), &res, _sdk);
	cout << str_tmp << endl;
	cout << "******" << endl;	
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog", _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_GetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", &str_tmp, _sdk), &res, _sdk);
	cout << str_tmp << endl;
	SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_dtrack", 5, _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_dtrack", &int_tmp, _sdk), &res, _sdk);
	cout << to_string(int_tmp) << endl;
	cout << "******" << endl;

 	SCISDK_s_error(SCISDK_GetParameterListOfValues("board0:/MMCComponents/Oscilloscope_0.trigger_mode", &str_tmp, _sdk), &res, _sdk);
	cout << str_tmp << endl;
	SCISDK_s_error(SCISDK_GetParameterDescription("board0:/MMCComponents/Oscilloscope_0.trigger_mode", &str_tmp, _sdk), &res, _sdk);
	cout << str_tmp << endl;
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", &int_tmp, _sdk), &res, _sdk); 
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_GetAllParameters("board0:/MMCComponents/Oscilloscope_0", &str_tmp, _sdk), &res, _sdk);
	cout << str_tmp << endl;
	//SCISDK_s_error(SCISDK_GetParameterListOfValues("board0:/MMCComponents/Oscilloscope_0.data_processing", &str_tmp, _sdk), &res, _sdk);

	//SCISDK_s_error(SCISDK_GetParameterMaximumValue("board0:/MMCComponents/Oscilloscope_0.timeout", &value, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_GetComponentList("board0", "all", &str_tmp, true, _sdk), &res, _sdk);
	int_tmp = SCISDK_GetParameterString("board0:/MMCComponents/Oscilloscope_0.buffer_type", &str_tmp, _sdk);
	cout << "buffer type " << int_tmp << endl;
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	
	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", 1, (void**)&ob, _sdk), &res, _sdk);
	cout << ob->magic << endl;
	cout << ob->timecode << endl;
	cout << ob->trigger_position << endl;
	cout << ob->info.channels << endl;
	cout << ob->info.samples_analog << endl;
	cout << ob->info.samples_digital << endl;
	cout << ob->info.tracks_analog_per_channel << endl;
	cout << ob->info.tracks_digital_per_channel << endl;
	
	cout << "START REQ FROM OSCILLOSCOPE" << endl;
	while(1){
	SCISDK_s_error(SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk), &res, _sdk);
		for (int i=0;i<10;i++)
			cout << ob->analog[i] << " ";
		cout << endl;
	}
	

	SCISDK_FreeBuffer("board0:/MMCComponents/Oscilloscope_0", 1, (void**)&ob, _sdk);
	// cout << "command result: " << int_tmp << endl;
	// cout << "operation result " << str_tmp << endl;
	// cout << SCISDK_FreeLib(_sdk) << endl;
	return 0;
}