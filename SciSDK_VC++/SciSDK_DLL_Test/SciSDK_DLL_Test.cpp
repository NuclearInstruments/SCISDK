// SciSDK_DLL_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/SciSDK_DLL.h"

#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;


typedef struct {
	uint32_t magic;
	int32_t *analog;
	uint8_t *digital;
	uint32_t trigger_position;
	uint64_t timecode;
	struct {
		uint32_t samples_analog;
		uint32_t samples_digital;
		uint32_t tracks_analog_per_channel;
		uint32_t tracks_digital_per_channel;
		uint32_t channels;
	} info;
}SCISDK_OSCILLOSCOPE_DECODED_BUFFER;

typedef struct {
	uint32_t magic;
	uint32_t *data;
	uint32_t zero_position;
	uint32_t trigger_position;
	uint64_t timecode;
	struct {
		uint32_t buffer_size;
		uint32_t samples_analog;
		uint32_t samples_digital;
		uint32_t tracks_analog_per_channel;
		uint32_t tracks_digital_per_channel;
		uint32_t channels;
	} info;
}SCISDK_OSCILLOSCOPE_RAW_BUFFER;

int main()
{
	void* _sdk = SCISDK_InitLib();

	char * res = "";
	SCISDK_s_error(SCISDK_AddNewDevice("usb:13250", "dt1260", "oscilloscope.json", "board0", _sdk), &res, _sdk);
	cout << res << endl;

	char* str_tmp = "";
	//SCISDK_s_error(SCISDK_GetComponentList("board0", "oscilloscope", &str_tmp, true, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_GetAllParameters("board0:/MMCComponents/Oscilloscope_0", &str_tmp, _sdk), &res, _sdk);
	//SCISDK_s_error(SCISDK_GetParameterListOfValues("board0:/MMCComponents/Oscilloscope_0.data_processing", &str_tmp, _sdk), &res, _sdk);
	double value;
	//SCISDK_s_error(SCISDK_GetParameterMaximumValue("board0:/MMCComponents/Oscilloscope_0.timeout", &value, _sdk), &res, _sdk);
	SCISDK_s_error(SCISDK_GetParametersProperties("board0:/MMCComponents/Oscilloscope_0", &str_tmp, _sdk), &res, _sdk);
	cout << "command result: " << res << endl;
	cout << str_tmp << endl;
	return 0;
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
		//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk), res, _sdk);
		//cout << res << endl;
		//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000, _sdk), res, _sdk);
		//cout << res << endl;
		//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, _sdk), res, _sdk);
		//cout << res << endl;

		//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk), res, _sdk);
		//cout << res << endl;
		//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk), res, _sdk);
		//cout << res << endl;
		//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk), res, _sdk);
		//cout << res << endl;
		////sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
		//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk), res, _sdk);
		//cout << res << endl;
		///*SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 1000, _sdk), res, _sdk);
		//cout << res << endl;*/
		//SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
		////SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER *rb;
		//SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", 1, (void**)&ob, _sdk), res, _sdk);
		//cout << res << endl;
		///*SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb, _sdk), res, _sdk);
		//cout << res << endl;*/
		//SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "", _sdk), res, _sdk);
		//cout << res << endl;
		////while (1) {
		//SCISDK_s_error(SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk), res, _sdk);
		//cout << res << endl;

		//cout << ob->info.channels << endl;

		////}
		//SCISDK_FreeBuffer("board0:/MMCComponents/Oscilloscope_0", 1, (void**)&ob, _sdk);
		//SCISDK_FreeLib(_sdk);

	return 0;

}

