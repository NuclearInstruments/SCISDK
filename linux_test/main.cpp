#include<iostream>
#include <SciSDK_DLL.h>
#include <scisdk_defines.h>
using namespace std;

int main(int argc, char* argv[]) {
		char* str_tmp = "";
	int int_tmp = -1;
  void* _sdk = SCISDK_InitLib();
	char * res = "";
	SCISDK_s_error(SCISDK_AddNewDevice("192.168.102.219:8888", "dt5560", "RegisterFile.json", "board0", _sdk), &res, _sdk);
	cout << res << endl;


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
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_GetAllParameters("board0:/MMCComponents/Oscilloscope_0", &str_tmp, _sdk), &res, _sdk);
	cout << str_tmp << endl;
	//SCISDK_s_error(SCISDK_GetParameterListOfValues("board0:/MMCComponents/Oscilloscope_0.data_processing", &str_tmp, _sdk), &res, _sdk);
	double value;
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