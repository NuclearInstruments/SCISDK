// SciSDK_DLL_Test.cpp : Defines the entry point for the console application.
//

#include "../../src/SciSDK_DLL.h"

#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
using namespace std;



int main(int argc, char* argv[])
{
	void* _sdk = SCISDK_InitLib();
	char * res = "";

	SCISDK_s_error(SCISDK_AddNewDevice("192.168.102.220:8888", "DT5560", "C:/GIT/Tutorial/Transistor_reset_demo/RegisterFile.json", "board0", _sdk), &res, _sdk);
	int ret = SCISDK_SetRegister("board0:/Registers/RUNREG", 0, _sdk);

	SCISDK_s_error(SCISDK_AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0", _sdk), &res, _sdk);
	
	cout << res << endl;

	char* str_tmp = "";
	int int_tmp = 0;
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_dtrack", &int_tmp, _sdk), &res, _sdk);
	cout << endl << "********" << to_string(int_tmp) << endl;
	

		//// SPECTRUM
		SCISDK_SPECTRUM_DECODED_BUFFER *obSpectrum;
		
		SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, _sdk), &res, _sdk);
		cout << res << endl;
		SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.rebin", "0",_sdk), &res, _sdk);
		cout << res << endl;
		SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.limitmode", "freerun", _sdk), &res, _sdk);
		cout << res << endl;
		SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.limit", "100", _sdk), &res, _sdk);
		cout << res << endl;
		SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "", _sdk), &res, _sdk);
		cout << res << endl;
		SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/TOF_0.start", "", _sdk), &res, _sdk);
		cout << res << endl;
		SCISDK_s_error(SCISDK_SetRegister("board0:/Registers/DELAY", 100, _sdk), &res, _sdk);
		cout << res << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4000));
		//while (1) {
			//std::ofstream out("c:/tmp/output1.txt");
			SCISDK_ReadData("board0:/MMCComponents/TOF_0", (void *)obSpectrum, _sdk);
			for (int i = 0; i < obSpectrum->info.total_bins; i++) {
			cout << obSpectrum->data[i] << endl;
			}
			//out.close();
			SCISDK_FreeBuffer("board0:/MMCComponents/TOF_0", 1, (void**)&obSpectrum, _sdk);
		//}

		

	return 0;

}


/*SCISDK_RM_RAW_BUFFER *rmb;
SCISDK_AllocateBuffer("board0:/MMCComponents/RateMeter_0", 0, (void**)&rmb, _sdk);
SCISDK_ReadData("board0:/MMCComponents/RateMeter_0", (void *)rmb, _sdk);
for (int i = 0; i < rmb->info.nchannels; i++){
	cout << rmb->data[i] << endl;
}
string s = "";
getline(cin, s);
return 0;
*/
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
//return 0;
/*
	int value;
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.ndigital", &value, _sdk), res, _sdk);
	cout << res << endl;
	cout << "value: " << value << endl;*/
	/*int value;
	SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/CREG_1.register_2", 12, _sdk), res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/CREG_0.register_2", &value, _sdk), res, _sdk);
	cout << res << endl;
	cout << "value: " << value << endl;
	for (int i = 0; i < 6; i++) {
		string reg_path = "board0:/MMCComponents/REGFILE_0.register_" + to_string(i);
		SCISDK_s_error(SCISDK_GetParameterInteger((char*)reg_path.c_str(), &value, _sdk), res, _sdk);
		cout << "value " + to_string(i) + " " << value << endl;
	}*/

	//// OSCILLOSCOPE DUAL
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000, _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, _sdk), &res, _sdk);
	//cout << res << endl;

	//int val = 0;
	//SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.ndigital", &val, _sdk), &res, _sdk);
	//cout << "val: " << val << endl;

	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk), &res, _sdk);
	//cout << res << endl;
	////sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk), &res, _sdk);
	//cout << res << endl;
	///*SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 1000, _sdk), res, _sdk);
	//cout << res << endl;*/
	//SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	////SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER *rb;
	//SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", 1, (void**)&ob, _sdk), &res, _sdk);
	//cout << res << endl;
	///*SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb, _sdk), &res, _sdk);
	//cout << res << endl;*/
	//SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "", _sdk), &res, _sdk);
	//cout << res << endl;
	////while (1) {
	//SCISDK_s_error(SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk), &res, _sdk);
	//cout << res << endl;

	//SCISDK_FreeBuffer("board0:/MMCComponents/Oscilloscope_0", 1, (void**)&ob, _sdk);
	//SCISDK_FreeLib(_sdk);

