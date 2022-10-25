#include<iostream>
#include <SciSDK_DLL.h>
using namespace std;

int main(int argc, char* argv[]) {
  void* _sdk = SCISDK_InitLib();
	char * res = "";
	SCISDK_s_error(SCISDK_AddNewDevice("tcp:127.0.0.1", "dt5560se", "RegisterFile.json", "board0", _sdk), &res, _sdk);
	cout << res << endl;


	//SCISDK_s_error(SCISDK_GetComponentList("board0", "oscilloscope", &str_tmp, true, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", &int_tmp, _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_GetAllParameters("board0:/MMCComponents/Oscilloscope_0", &str_tmp, _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_GetParameterListOfValues("board0:/MMCComponents/Oscilloscope_0.data_processing", &str_tmp, _sdk), &res, _sdk);
	//double value;
	////SCISDK_s_error(SCISDK_GetParameterMaximumValue("board0:/MMCComponents/Oscilloscope_0.timeout", &value, _sdk), &res, _sdk);
	////SCISDK_s_error(SCISDK_GetComponentList("board0", "all", &str_tmp, true, _sdk), &res, _sdk);
	//int_tmp = SCISDK_GetParameterString("board0:/MMCComponents/Oscilloscope_0.buffer_type", &str_tmp, _sdk);
	//cout << "command result: " << int_tmp << endl;
	//cout << "operation result " << str_tmp << endl;
	//cout << SCISDK_FreeLib(_sdk) << endl;
	return 0;
}