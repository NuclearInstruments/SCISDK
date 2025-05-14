// SciSDK_DLL_Test.cpp : Defines the entry point for the console application.
//

#include "../../src/SciSDK_DLL.h"

#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void TestDT4810();
void TestDT1260();
void TestAFEDT5771();
void TestR5560();
void TestAFEDT55560();
void TestAFEDT55560_reset();
void TestDigitizerDT5771();
int main(int argc, char* argv[])
{
	//TestAFEDT55560();
	//TestAFEDT55560_reset();
	//TestDT4810();
	//TestAFEDT5771();
	//TestR5560();
	TestDigitizerDT5771();
	return 0;

}

void TestR5560() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";
	uint32_t revalue=0;

	int op_res = SCISDK_AddNewDevice("130.246.55.76:8888", "R5560", "RegisterFile.json", "board0", _sdk);
	//op_res = SCISDK_GetRegister("board0:/Registers/INTGAIN", &revalue, _sdk);
	//cout <<" op res " << op_res << " v: " << revalue << endl;


	SCISDK_2DHISTOGRAM_DECODED_BUFFER* obHistrogram;

	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Hist2D_0", T_BUFFER_TYPE_DECODED, (void**)&obHistrogram, _sdk), &res, _sdk);
	cout << res << endl;
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun", _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Spectrum_0.limit", "100", _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "", _sdk), &res, _sdk);
	//cout << res << endl;
	//SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "", _sdk), &res, _sdk);
	//cout << res << endl;
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	//while (1) {
	std::ofstream out("c:/temp/output1.txt");
	SCISDK_ReadData("board0:/MMCComponents/Hist2D_0", (void*)obHistrogram, _sdk);
	for (int i = 0; i < obHistrogram->info.total_bins; i++) {
		out << obHistrogram->data[i] << endl;
	}
	out.close();
	SCISDK_FreeBuffer("board0:/MMCComponents/Hist2D_0", 1, (void**)&obHistrogram, _sdk);


}

void TestDT4810() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	int op_res = SCISDK_AddNewDevice("192.168.102.120:8888", "dt4810", "dt4810-firmware.json", "board0", _sdk);

	if (op_res == 0) {
		cout << "Device added" << endl;
	}
	else {
		cout << "Device not added" << endl;
		exit(-1);
	}
	char *fwversion;
	char *hwversion;
	op_res = SCISDK_GetParameterString("board0:/boardapi/fwver", &fwversion, _sdk);
	op_res = SCISDK_GetParameterString("board0:/boardapi/hwver", &hwversion, _sdk);
	cout << "Firmware version: " << fwversion << endl;
	cout << "Hardware version: " << hwversion << endl;
	
	op_res = SCISDK_ExecuteCommand("board0:/boardapi/initialize", "", _sdk);
	op_res = SCISDK_SetParameterUInteger("board0:/boardapi/amplitude.fixed", 8000, _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/amplitude.mode", "fixed", _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/timebase.rate", 100, _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/timebase.mode", "fixed", _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.decay", 200, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 50, _sdk);

	op_res = SCISDK_SetParameterString("board0:/boardapi/noise.gauss.enable", "false", _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/noise.gauss.gain", 0.70, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/main.gain", 1, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/main.offset", 000, _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/main.output.enable", "false", _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/main.output.enable", "true", _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/main.output.gate", "true", _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/main.invert", "false", _sdk);

	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 20, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 50, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 100, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 250, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 500, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 1000, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 2000, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 4000, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 6000, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 8000, _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/shape.drc.risetime", 10000, _sdk);
	/* "mon.icr",
	 "mon.ocr",
	 "mon.ic", "
	 "mon.oc", "
	 "mon.deadp"
	 "mon.dead",
	 "mon.rt", "
	 "mon.lt", "
	 "mon.los",
	 "mon.ovl",
	 */
	cout << endl;
	double icr, ocr, ic, rt, lt, los, ovl, deadp, dead, oc, sc;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.icr", &icr, _sdk);
	cout << "ICR: " << icr << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.ocr", &ocr, _sdk);
	cout << "OCR: " << ocr << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.ic", &ic, _sdk);
	cout << "IC: " << ic << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.oc", &oc, _sdk);
	cout << "OC: " << oc << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.sc", &sc, _sdk);
	cout << "SC: " << sc << endl;

	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.rt", &rt, _sdk);
	cout << "RT: " << rt << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.lt", &lt, _sdk);
	cout << "LT: " << lt << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.los", &los, _sdk);
	cout << "LOS: " << los << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.ovl", &ovl, _sdk);
	cout << "OVL: " << ovl << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.deadp", &deadp, _sdk);
	cout << "DEADP: " << deadp << endl;
	op_res = SCISDK_GetParameterDouble("board0:/boardapi/mon.dead", &dead, _sdk);
	cout << "DEAD: " << dead << endl;
	

	SCISDK_EMULATOR_ENERGY_SPECTRUM* energy_spectrum;

	
	op_res = SCISDK_AllocateBufferSize("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&energy_spectrum, _sdk, 16384);
	
	for (int i = 0; i < 16384; i++) {
		energy_spectrum->data[i] = i/4;
	}

	//energy_spectrum->data[1000] = 20000;
	//energy_spectrum->data[2000] = 5000;
	//energy_spectrum->data[400] = 5000;

	/*for (int i = 5000; i < 10000; i++) {
		energy_spectrum->data[i] = 30000;
	}*/
	energy_spectrum->info.valid_bins = 16384;

	op_res = SCISDK_WriteData("board0:/boardapi", energy_spectrum, _sdk);


	SCISDK_OSCILLOSCOPE_DECODED_BUFFER* obOsc;
	
	op_res = SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "ext", _sdk);
	op_res = SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk);
	op_res = SCISDK_SetParameterUInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level",5000, _sdk);
	op_res = SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
	op_res = SCISDK_SetParameterUInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 5000, _sdk);
	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&obOsc, _sdk), &res, _sdk);

	cout << "-----" << endl;
	while (1) {
		SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void*)obOsc, _sdk);
		for (int i = 0; i < 250; i++) {
			cout << (double)obOsc->analog[i] << endl;
		}
	}


	SCISDK_SPECTRUM_DECODED_BUFFER* obSpectrum;

	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/Spectrum_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, _sdk), &res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0", _sdk), &res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun", _sdk), &res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/Spectrum_0.limit", "100", _sdk), &res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "", _sdk), &res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "", _sdk), &res, _sdk);
	cout << res << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	//while (1) {
		//std::ofstream out("c:/tmp/output1.txt");
	SCISDK_ReadData("board0:/MMCComponents/Spectrum_0", (void*)obSpectrum, _sdk);
	for (int i = 0; i < obSpectrum->info.total_bins; i++) {
		cout << obSpectrum->data[i] << endl;
	}
	//out.close();
	SCISDK_FreeBuffer("board0:/MMCComponents/Spectrum_0", 1, (void**)&obSpectrum, _sdk);
	
}

void TestDT1260() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	/*SCISDK_s_error(SCISDK_AddNewDevice("192.168.102.220:8888", "DT5560", "C:/GIT/Tutorial/Transistor_reset_demo/RegisterFile.json", "board0", _sdk), &res, _sdk);
	int ret = SCISDK_SetRegister("board0:/Registers/RUNREG", 0, _sdk);*/

	int op_res = SCISDK_AddNewDevice("usb:28645", "dt1260", "C:/git/scisdk/examples/components/LabView/Registers/DT1260RegisterFile.json", "board0", _sdk);
	op_res = SCISDK_SetRegister("board0:/Registers/doesnotexist", 10, _sdk);

	char* str_tmp = "";
	int int_tmp = 0;
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_dtrack", &int_tmp, _sdk), &res, _sdk);
	cout << endl << "********" << to_string(int_tmp) << endl;


	//// SPECTRUM
	SCISDK_SPECTRUM_DECODED_BUFFER* obSpectrum;

	SCISDK_s_error(SCISDK_AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, _sdk), &res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.rebin", "0", _sdk), &res, _sdk);
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
	SCISDK_ReadData("board0:/MMCComponents/TOF_0", (void*)obSpectrum, _sdk);
	for (int i = 0; i < obSpectrum->info.total_bins; i++) {
		cout << obSpectrum->data[i] << endl;
	}
	//out.close();
	SCISDK_FreeBuffer("board0:/MMCComponents/TOF_0", 1, (void**)&obSpectrum, _sdk);
	//}


}

void TestAFEDT5771() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	int op_res = SCISDK_AddNewDevice("192.168.102.229:8888", "dt5771", "dt5771RegisterFile.json", "board0", _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/analog/CH0.range", "2v", _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/analog/CH0.impedance", "1k", _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/analog/CH0.offset", 101, _sdk);
}


void TestDigitizerDT5771() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	int op_res = SCISDK_AddNewDevice("192.168.102.139:8888", "dt5771", "RegisterFileDT5771-SmartMCA.json", "board0", _sdk);

	SCISDK_FE_SCOPE_EVENT* evnt;
	op_res = SCISDK_AllocateBufferSize("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&evnt,  _sdk, 1024 * 1024*8);
	op_res = SCISDK_ReadData("board0:/boardapi", evnt, _sdk);

	for (int i = 0; i < evnt->n_channels; i++) {
		cout << "Channel " << i << endl;
		for (int j = 0; j < 50; ++j) {
			cout << evnt->waveform[i][j] << "  " << endl;
		}
	}

	SCISDK_FreeBuffer("board0:/boardapi", T_BUFFER_TYPE_DECODED,(void**)&evnt, _sdk);
}

void TestAFEDT55560() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	int op_res = SCISDK_AddNewDevice("192.168.102.219:8888", "dt5560", "OscilloscopeDT5560.json", "board0", _sdk);
	for (int i = 0; i < 16; i++) {
		string ch = "board0:/boardapi/analog/CH" + to_string(i*2) + "_" + to_string(i*2+1) + ".gain";
		op_res = SCISDK_SetParameterInteger((char*)ch.c_str(), i, _sdk);
		ch = "board0:/boardapi/analog/CH" + to_string(i*2) + "_" + to_string(i*2 + 1) + ".50r";
		op_res = SCISDK_SetParameterString((char*)ch.c_str(), i&0x1 ? "true" : "false", _sdk);
		ch = "board0:/boardapi/analog/CH" + to_string(i * 2) + "_" + to_string(i * 2 + 1) + ".div";
		op_res = SCISDK_SetParameterString((char*)ch.c_str(), i & 0x1 ? "false" : "true", _sdk);
}
	for (int i = 0; i < 32; i++) {
		string ch = "board0:/boardapi/analog/CH" + to_string(i) + ".offset";
		op_res = SCISDK_SetParameterInteger((char*)ch.c_str(), i, _sdk);
	}
	op_res = SCISDK_ExecuteCommand("board0:/boardapi/configure", "", _sdk);

}


void TestAFEDT55560_reset() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	int op_res = SCISDK_AddNewDevice("192.168.102.219:8888", "dt5560", "OscilloscopeDT5560.json", "board0", _sdk);
	for (int i = 0; i < 16; i++) {
		string ch = "board0:/boardapi/analog/CH" + to_string(i * 2) + "_" + to_string(i * 2 + 1) + ".gain";
		op_res = SCISDK_SetParameterInteger((char*)ch.c_str(), 0, _sdk);
		ch = "board0:/boardapi/analog/CH" + to_string(i * 2) + "_" + to_string(i * 2 + 1) + ".50r";
		op_res = SCISDK_SetParameterString((char*)ch.c_str(), "true", _sdk);
		ch = "board0:/boardapi/analog/CH" + to_string(i * 2) + "_" + to_string(i * 2 + 1) + ".div";
		op_res = SCISDK_SetParameterString((char*)ch.c_str(), "true", _sdk);
	}
	for (int i = 0; i < 32; i++) {
		string ch = "board0:/boardapi/analog/CH" + to_string(i) + ".offset";
		op_res = SCISDK_SetParameterInteger((char*)ch.c_str(), 0, _sdk);
	}
	op_res = SCISDK_SetParameterString("board0:/boardapi/analog.shaper", "dc", _sdk);
	op_res = SCISDK_ExecuteCommand("board0:/boardapi/configure", "", _sdk);

}


void TestDT1260List() {
	void* _sdk = SCISDK_InitLib();
	char* res = "";

	int op_res = SCISDK_AddNewDevice("usb:10500", "dt1260", "RegisterFileList.json", "board0", _sdk);
	op_res = SCISDK_SetParameterString("board0:/MMCComponents/List_0.thread", "false", _sdk);
	op_res = SCISDK_SetParameterString("board0:/boardapi/analog/CH0.impedance", "1k", _sdk);
	op_res = SCISDK_SetParameterDouble("board0:/boardapi/analog/CH0.offset", 101, _sdk);
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

