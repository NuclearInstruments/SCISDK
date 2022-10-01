// SciSDK_VC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/scisdk_core.h"
#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>
SciSDK sdk;


void dump_to_file(SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data) {

}
//
//void * SCISDK_InitLib() {
//	SciSDK * _sdk = new SciSDK;
//	return (void *)_sdk;
//}
//
//int SCISDK_FreeLib(void *handle) {
//	if (handle == NULL) return NI_ERROR;
//	SciSDK * _sdk = (SciSDK*)handle;
//	delete _sdk;
//}
//
//int SCISDK_AddNewDevice(char *DevicePath, char *DeviceModel, char *JSONFwFilePath, char *Name, void *handle) {
//
//	if (handle == NULL) return NI_ERROR;
//	if (DevicePath == NULL) return NI_ERROR;
//	if (DeviceModel == NULL) return NI_ERROR;
//	if (JSONFwFilePath == NULL)return NI_ERROR;
//	if (Name == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _DevicePath(DevicePath);
//	string _DeviceModel(DeviceModel);
//	string _JSONFwFilePath(JSONFwFilePath);
//	string _Name(Name);
//
//	int res = _sdk->AddNewDevice(_DevicePath, _DeviceModel, _JSONFwFilePath, _Name);
//	_sdk->p_error(res);
//	return res;
//}
//
//int SCISDK_DetachDevice(char* name, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (name == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _name(name);
//	int res = _sdk->DetachDevice(_name);
//	return res;
//}
//
//int SCISDK_SetParameterString(char* Path, char* value, void* handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	string _value(value);
//
//	return _sdk->SetParameter(_Path, _value);
//}
//
//int SCISDK_SetParameterInteger(char* Path, int value, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//
//	return _sdk->SetParameter(_Path, value);
//}
//
//int SCISDK_SetParameterDouble(char* Path, double value, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//
//	return _sdk->SetParameter(_Path, value);
//}
//
//int SCISDK_GetParameterString(char* Path, char* value, void* handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	string _value;
//
//	int res = _sdk->GetParameter(_Path, &_value);
//	char *v = (char*)_value.c_str();
//	value = v;
//
//	return res;
//}
//
//int SCISDK_GetParameterInteger(char* Path, int *value, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//
//	int res = _sdk->GetParameter(_Path, value);
//	return res;
//}
//
//int SCISDK_GetParameterDouble(char* Path, double*value, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	SciSDK*_sdk = (SciSDK*)handle;
//	string _Path(Path);
//
//	int res = _sdk->GetParameter(_Path, value);
//	return res;
//}
//
//int SCISDK_DecodeData(char* Path, void*buffer_in, void*buffer_out, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (buffer_in == NULL)return NI_ERROR;
//	if (buffer_out == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	_sdk->DecodeData(_Path, buffer_in, buffer_out);
//}
//
//int SCISDK_AllocateBuffer(char* Path, int buffer_type, void **buffer, void *handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (buffer_type == NULL)return NI_ERROR;
//	if (buffer == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	T_BUFFER_TYPE _buffer_type;
//	if (buffer_type == 0) {
//		_buffer_type = T_BUFFER_TYPE_RAW;
//	}
//	else if (buffer_type == 1) {
//		_buffer_type = T_BUFFER_TYPE_DECODED;
//	}
//	else {
//		return NI_ERROR;
//	}
//
//	int res = _sdk->AllocateBuffer(_Path, _buffer_type, buffer);
//	return res;
//}
//
//int SCISDK_SetRegister(char* Path, int value, void* handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//	if (value < 0)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	uint32_t _value = (uint32_t)value;
//
//	int res = _sdk->SetRegister(_Path, _value);
//	return res;
//}
//
//int SCISDK_GetRegister(char* Path, int*value, void*handle) {
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//	if (handle == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	uint32_t _value;
//	int res = _sdk->GetRegister(_Path, &_value);
//	*value = (int)_value;
//	return res;
//}
//
//int SCISDK_FreeBuffer(char* Path, int buffer_type, void **buffer, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (buffer_type == NULL)return NI_ERROR;
//	if (buffer == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	T_BUFFER_TYPE _buffer_type;
//	if (buffer_type == 0) {
//		_buffer_type = T_BUFFER_TYPE_RAW;
//	}
//	else if (buffer_type == 1) {
//		_buffer_type = T_BUFFER_TYPE_DECODED;
//	}
//	else {
//		return NI_ERROR;
//	}
//
//	int res = _sdk->FreeBuffer(_Path, _buffer_type, buffer);
//	return res;
//}
//
//int SCISDK_ReadData(char *Path, void *buffer, void*handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (buffer == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string _Path(Path);
//	int res = _sdk->ReadData(_Path, buffer);
//	_sdk->p_error(res);
//	return res;
//}
//
//int SCISDK_s_error(int err_no, char* value, void* handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (err_no == NULL)return NI_ERROR;
//
//	SciSDK * _sdk = (SciSDK*)handle;
//	string ret_string;
//
//	ret_string = _sdk->s_error(err_no);
//	value = (char*)ret_string.c_str();
//	return NI_OK;
//}
//
//int SCISDK_ExecuteCommand(char* Path, char* value, void* handle) {
//	if (handle == NULL)return NI_ERROR;
//	if (Path == NULL)return NI_ERROR;
//	if (value == NULL)return NI_ERROR;
//
//	string _Path(Path);
//	string _value(value);
//	SciSDK* _sdk = (SciSDK*)handle;
//	int res = _sdk->ExecuteCommand(_Path, _value);
//	return res;
//}


int main()
{
	SciSDK sdk;
	//DAVIDE 13250
	//ANDREA 10500
	int res = sdk.AddNewDevice("usb:10500", "dt1260", "Frame.json", "board0");
	//void* _sdk = SCISDK_InitLib();

	// REGISTRI
	//char * res = "";
	//SCISDK_s_error(SCISDK_AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0", _sdk), res, _sdk);

	//int value;
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/CREG_1.register_2", 12, _sdk), res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/CREG_0.register_2", &value, _sdk), res, _sdk);
	//cout << res << endl;
	//cout << "value: " << value << endl;
	//for (int i = 0; i < 6; i++) {
	//	string reg_path = "board0:/MMCComponents/REGFILE_0.register_" + to_string(i);
	//	SCISDK_s_error(SCISDK_GetParameterInteger((char*)reg_path.c_str(), &value, _sdk), res, _sdk);
	//	cout << "value " + to_string(i) + " " << value << endl;
	//}

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
	//SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 1000, _sdk), res, _sdk);
	//cout << res << endl;
	//SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER *ob;
	//SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER *rb;
	//SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk), res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb, _sdk), res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "", _sdk), res, _sdk);
	//cout << res << endl;
	////while (1) {
	//std::ofstream out1("c:/tmp/output_dual_channel1.txt");
	//std::ofstream out2("c:/tmp/output_dual_channel2.txt");
	//SCISDK_s_error(SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk), res, _sdk);
	//cout << res << endl;
	//for (int i = 0; i < ob->info.samples_analog; i++) {
	//	out1 << ob->analog[i] << endl;
	//	out2 << ob->analog[i + 1 * ob->info.samples_analog] << endl;
	//}
	//out1.close();
	//out2.close();
	////}
	//SCISDK_FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);
	//SCISDK_FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb, _sdk);
	//SCISDK_FreeLib(_sdk);

	//return 0;


	//SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data;
	//usb:10500
	//usb:13250
	//sdk.p_error(sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0"));

	//sdk.p_error(sdk.SetRegister("board0:/Registers/res", 1));
	//sdk.p_error(sdk.SetRegister("board0:/Registers/res", 0));


	////// OSCILLOSCOPE (decoded & raw)
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode"));
	////sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 5000));
	//SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	//SCISDK_OSCILLOSCOPE_RAW_BUFFER *rb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**) &ob));
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", ""));
	////while (1) {
	//	//std::ofstream out("c:/temp/output.txt");
	//	sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob);
	//	cout << ob->timecode << endl;
	//	/*for (int i = 0; i < ob->info.samples_analog; i++) {
	//	out << ob->analog[i] << endl;
	//	}
	//	out.close();*/
	////}
	//	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
	//	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);


	//// OSCILLOSCOPE DUAL (decoded & raw)
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 1));

	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode"));
	////sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000));
	//SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER *ob;
	//SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER *rb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob));
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", ""));
	////while (1) {
	//std::ofstream out("c:/tmp/output_dual.txt");
	//sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob);
	//for (int i = 0; i < ob->info.samples_analog; i++) {
	//	out << ob->analog[i + 1 * ob->info.samples_analog] << endl;
	//}
	//out.close();
	////}
	//sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
	//sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);


	//// SPECTRUM
	//SCISDK_SPECTRUM_DECODED_BUFFER *obSpectrum;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limit", "100"));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", ""));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", ""));
	//sdk.p_error(sdk.SetRegister("board0:/Registers/spe", 100));
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	////while (1) {
	//	std::ofstream out("c:/tmp/output1.txt");
	//	sdk.ReadData("board0:/MMCComponents/Spectrum_0", (void *)obSpectrum);
	//	for (int i = 0; i < obSpectrum->info.total_bins; i++) {
	//	out << obSpectrum->data[i] << endl;
	//	}
	//	out.close();
	//	sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum);
	////}
	//sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
	//sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);


	// DIGITIZER decoded
	//SCISDK_DIGITIZER_DECODED_BUFFER *ddb;
	//sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0", T_BUFFER_TYPE_DECODED, (void**)&ddb);
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.data_processing", "decode");
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.enabledch", 1);
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_len", 1000);
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.timeout", 100);
	//sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", "");
	//while (1) {
	//	int ret = sdk.ReadData("board0:/MMCComponents/Digitizer_0", (void *)ddb);
	//	if (ret == NI_OK) {
	//		std::ofstream out("c:/temp/output.txt");
	//		for (int i = 0; i < ddb->info.valid_samples; i++) {
	//			out << ddb->analog[i] << endl;
	//		}
	//		out.close();
	//	}
	//}


	//// DIGITIZER raw
	//SCISDK_DIGITIZER_RAW_BUFFER *ddb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0", T_BUFFER_TYPE_RAW, (void**)&ddb, 100));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.data_processing", "raw"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.enabledch", 1));
	//sdk.p_error(__LINE__, sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_len", 1000));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_mode", "non-blocking"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.timeout", 10000));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", ""));
	//while (1) {
	//	int ret = sdk.ReadData("board0:/MMCComponents/Digitizer_0", (void *)ddb);
	//	if (ret == NI_OK) {
	//		for (int i = 0; i < ddb->info.valid_samples; i++) {
	//			cout << std::hex  << ddb->data[i] << endl;
	//		}	
	//	}
	//}


	//// LIST raw
	//SCISDK_LIST_RAW_BUFFER *ddb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/List_0", T_BUFFER_TYPE_RAW, (void**)&ddb, 1000));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "non-blocking"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/List_0.thread", "true"));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", ""));
	//while (1) {
	//	int ret = sdk.ReadData("board0:/MMCComponents/List_0", (void *)ddb);
	//	if (ret == NI_OK) {
	//		uint32_t *p;
	//		p = (uint32_t *)ddb->data;
	//		cout << "----- " << ddb->info.valid_samples / 4 << " -----" << endl;
	//		for (int i = 0; i < ddb->info.valid_samples/4; i++) {
	//			cout << std::hex  << p[i] << endl;
	//		}
	//	}
	//}


	//// CUSTOM PACKET decoded
	//SCISDK_CP_DECODED_BUFFER *cpb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/CP_0", T_BUFFER_TYPE_DECODED, (void**)&cpb, 100));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.acq_mode", "blocking"));
	//sdk.SetParameter("board0:/MMCComponents/CP_0.timeout", 5000);
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.thread", "true"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.data_processing", "decode"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.check_align_word", "true"));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CP_0.start", ""));
	//while (1) {
	//	int ret = sdk.ReadData("board0:/MMCComponents/CP_0", (void *)cpb);
	//	if (ret == NI_OK) {
	//		SCISDK_CP_PACKET *p;
	//		p = (SCISDK_CP_PACKET *)cpb->data;
	//		cout << "----- " << cpb->info.valid_data  << " -----" << endl;
	//		for (int i = 0; i < cpb->info.valid_data; i++) {
	//			cout << "  "  << std::hex << p[i].row[0] << endl;
	//			cout << "  " << std::hex << p[i].row[1] << endl;
	//			cout << "  " << std::hex << p[i].row[2] << endl;
	//			cout << " ~~~ " << endl;
	//		}
	//	}
	//}


	//// CUSTOM PACKET raw
	//SCISDK_CP_RAW_BUFFER *cpb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/CP_0", T_BUFFER_TYPE_RAW, (void**)&cpb, 100));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.acq_mode", "blocking"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.thread", "true"));
	//sdk.SetParameter("board0:/MMCComponents/CP_0.timeout", 5000);
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.data_processing", "raw"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.check_align_word", "false"));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CP_0.start", ""));
	//while (1) {
	//	int ret = sdk.ReadData("board0:/MMCComponents/CP_0", (void *)cpb);
	//		for (int i = 0; i < cpb->info.valid_data; i++) {
	//			std::cout << std::hex << cpb->data[i] << endl;
	//		}
	//}


	//// RATEMETER
	/*SCISDK_RM_RAW_BUFFER *rmb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", T_BUFFER_TYPE_RAW, (void**)&rmb));
	sdk.ReadData("board0:/MMCComponents/RateMeter_0", (void *)rmb);
	for (int i = 0; i < rmb->info.nchannels; i++){
		cout << rmb->data[i] << endl;
	}*/


	//// DT5550W-CITIROC
	//string bit_config_str = "1111111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110110111111111111111111111111111111111111111111011111100111111101111001111011100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000111111110100101100010010110011111111111111011";
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CitirocCfg0.bitstream", bit_config_str));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CitirocCfg0.write_bitstream",""));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CitirocCfg0.read_bitstream", ""));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CitirocCfg0.decode_bit_config", ""));
	//uint32_t *value = (uint32_t*)malloc(sizeof(uint32_t));
	//sdk.p_error(sdk.GetParameter("board0:/MMCComponents/CitirocCfg0.sc_enDiscri", value));
	//cout << "sc_enDiscri " << *value << endl;
	//free(value);


	// REGISTERS
	//uint32_t value;
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CREG_1.register_2", 12));
	//sdk.p_error(sdk.GetParameter("board0:/MMCComponents/CREG_0.register_2", &value));
	//cout << "test value " << value << endl;
	//for (int i = 0; i < 6; i++) {
	//	sdk.p_error(sdk.GetParameter("board0:/MMCComponents/REGFILE_0.register_" + to_string(i), &value));
	//	cout << "value " + to_string(i) + " " << value << endl;
	//}

	//sdk.p_error(sdk.AddNewDevice("172.16.49.97:8888", "DT5560", "DT5560.json", "board0"));
	//sdk.p_error(sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board1"));

	//// DT5560
	//uint32_t value;
	//sdk.p_error(sdk.SetRegister("board0:/Registers/w1", 1));
	//sdk.p_error(sdk.GetRegister("board0:/Registers/r1", &value));
	//cout << "DT5560 register r1 value: " << value << endl;

	//SCISDK_RM_RAW_BUFFER *rmb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", T_BUFFER_TYPE_RAW, (void**)&rmb));
	//sdk.ReadData("board0:/MMCComponents/RateMeter_0", (void *)rmb);
	//for (int i = 0; i < rmb->info.nchannels; i++) {
	//	cout << "DT5560 rate meter channel " << i << ": " << rmb->data[i] << endl;
	//}

	//// SciDK
	//sdk.p_error(sdk.SetParameter("board1:/MMCComponents/CREG_1.register_2", 12));
	//sdk.p_error(sdk.GetParameter("board1:/MMCComponents/CREG_0.register_2", &value));
	//cout << "scidk test value " << value << endl;
	//for (int i = 0; i < 6; i++) {
	//	sdk.p_error(sdk.GetParameter("board1:/MMCComponents/REGFILE_0.register_" + to_string(i), &value));
	//	cout << "scidk register " + to_string(i) + " value: " << value << endl;
	//}

	//sdk.p_error(sdk.DetachDevice("board0"));
	//sdk.p_error(sdk.DetachDevice("board1"));

	//FRAME TEST
	SCISDK_FRAME_DECODED_BUFFER *frameD;
	sdk.p_error(sdk.SetRegister("board0:/Registers/period", 100000));
	int q = 0;
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.acq_mode", "non-blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.data_processing", "decode"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.thread", "true"));
	int ret = sdk.AllocateBuffer("board0:/MMCComponents/ImageReadout_0", T_BUFFER_TYPE_DECODED, (void**)&frameD,100);

	
	if (ret == NI_OK)
	{
		sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/ImageReadout_0.start", ""));
		//for (int i = 0;i < 100000; i++)
		//	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CREG_0.register_0", q++));
		while (1) {
			int ret = sdk.ReadData("board0:/MMCComponents/ImageReadout_0", (void *)frameD);
			if (ret == NI_OK) {
				

				for (int i = 0;i < frameD->info.valid_data; i++) {
					cout << "*TIMESTAMP:" << frameD->data[i].timestamp << "   TRG: " << frameD->data[i].trigger_count << endl;
					cout << "    ";
					for (int j = 0;j < frameD->data[i].n;j++) {
						cout << frameD->data[i].pixel[j] << " ";
					}
					cout << endl;
				}
			}
		}
	}
	else {
		cout << "unable to allocate buffer" << endl;
		sdk.p_error(ret);
		return -1;
	}


	return 0;

}

