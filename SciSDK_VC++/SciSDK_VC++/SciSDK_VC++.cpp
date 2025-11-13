// SciSDK_VC++.cpp : Defines the entry point for the console application.
//

#include "../../src/scisdk_core.h"
#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>
SciSDK sdk;

void OpenDppDemo() {
	uint32_t v;
	string test;
	int res = 0;
	//int res = sdk.AddNewDevice("usb:10500", "FAKEBOARD", "C:/OpenHardware/UserProjects/SciSDKDev2740Wave/library/RegisterFile.json", "board0");
	std::ofstream out("c:/temp/output.csv");
	out << "ch,timestamp" << endl;

	//TEST DPP
	res = sdk.AddNewDevice("192.168.102.153", "V2740", "C:/OpenHardware/UserProject/v2740scisdkbug/library/RegisterFile.json", "board0");
	sdk.p_error(res);
	if (res) std::exit(res);
	sdk.p_error(sdk.ExecuteCommand("board0:/boardapi/felib/cmd/reset", ""));
	sdk.SetParameter("board0:/boardapi/readout.datatype", "dpp");
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/TrgOutMode", "UserTrgout"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/ch/0..63/par/ChEnable", "false"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/ch/0/par/ChEnable", "true"));
	sdk.p_error(sdk.GetParameter("board0:/boardapi/felib/ch/0/par/ChEnable", &test));
	std::cout << "test " << test << endl;
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/ch/1/par/ChEnable", "true"));
	sdk.p_error(sdk.GetParameter("board0:/boardapi/felib/ch/1/par/ChEnable", &test));
	std::cout << "test " << test << endl;
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/ch/2/par/ChEnable", "true"));
	sdk.p_error(sdk.GetParameter("board0:/boardapi/felib/ch/2/par/ChEnable", &test));
	std::cout << "test " << test << endl;

	// Data reduction
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/EnDataReduction", "true"));


	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/TestPulsePeriod", "100000"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/TestPulseWidth", "1000"));

	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/ITLAMask", "0x1"));

	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/ITLAPairLogic", "None"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/ITLAMajorityLev", "0"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/ITLAMainLogic", "OR"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/ITLAGateWidth", "800"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/ITLAPolarity", "Direct"));

	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/AcqTriggerSource", "ITLA"));

	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/startsource", "SWcmd"));
	sdk.p_error(sdk.ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", ""));
	sdk.p_error(sdk.ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", ""));

	string buffer_type;
	sdk.p_error(sdk.GetParameter("board0:/boardapi/readout.buffer_type", &buffer_type));

	cout << "buffer_type: " << buffer_type << endl;

	SCISDK_FE_OPENDPP_EVENT* dpp_evnt;

	sdk.p_error(sdk.AllocateBuffer("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&dpp_evnt));

	for (int i = 0; i < 1000; ++i) {
		sdk.p_error(sdk.ReadData("board0:/boardapi", dpp_evnt));
		out << (int)dpp_evnt->channel << "," << std::hex << dpp_evnt->timestamp << endl;
	}

	out.close();

	std::exit(0);
}

void OpenScopeDemo() {
	string test;
	int res = 0;
	
	//TEST SCOPE
	res = sdk.AddNewDevice("192.168.102.153", "V2740", "C:/OpenHardware/UserProject/v2740scisdkbug/library/RegisterFile.json", "board0");

	//TEST PARTE CAEN

	sdk.p_error(sdk.ExecuteCommand("board0:/boardapi/felib/cmd/reset", ""));
	sdk.SetParameter("board0:/boardapi/readout.datatype", "scope");

	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/ch/0..63/par/ChEnable", "true"));
	sdk.p_error(sdk.GetParameter("board0:/boardapi/felib/ch/0/par/ChEnable", &test));
	cout << "board0:/boardapi/felib/ch/0/par/ChEnable: " << test << endl;
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/RecordLengthS", "50"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/PreTriggerS", "4"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/AcqTriggerSource", "SwTrg | TestPulse"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/TestPulsePeriod", "100000"));
	sdk.p_error(sdk.SetParameter("board0:/boardapi/felib/par/TestPulseWidth", "1000"));
	sdk.p_error(sdk.ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", ""));
	sdk.p_error(sdk.ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", ""));

	string buffer_type;
	sdk.p_error(sdk.GetParameter("board0:/boardapi/buffer_type", &buffer_type));

	cout << "buffer_type: " << buffer_type << endl;


	SCISDK_FE_SCOPE_EVENT* evnt;
	sdk.p_error(sdk.AllocateBuffer("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&evnt, 50));
	sdk.ReadData("board0:/boardapi", evnt);

	for (int i = 0; i < evnt->n_channels; i++) {
		cout << "Channel " << i << endl;
		for (int j = 0; j < 50; ++j) {
			cout << evnt->waveform[i][j] << "  " << endl;
		}
	}
		
	
	std::exit(0);
}

void DemoSciCompilerRegister() {
	string test;
	int res = 0;
	uint32_t v;
	
	//TEST SCOPE
	res = sdk.AddNewDevice("10.105.250.18", "V2740", "SciSDKDev2740Wave.json", "board0");
	
	sdk.p_error(sdk.SetRegister("board0:/Registers/a", 1));
	sdk.p_error(sdk.SetRegister("board0:/Registers/b", 3));
	sdk.p_error(sdk.GetRegister("board0:/Registers/c", &v));
	cout << "Register access result: " << v << endl;
	
}

void DemoSciCompilerOscilloscope() {
	int res = 0;
	res = sdk.AddNewDevice("v2740.sw", "V2740", "C:/OpenHardware/UserProject/v2740scisdkbug/library/RegisterFile.json", "board0");
	
	//// OSCILLOSCOPE (decoded & raw)
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "free"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 33000));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 5000));
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER* ob;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", ""));

	std::ofstream out("c:/temp/output.txt");
	res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void*)ob);

	if (res == NI_OK) {
		cout << "Oscilloscope TS: " << ob->timecode << endl;
		for (int i = 0; i < ob->info.samples_analog; i++) {
			cout << ob->analog[i] << endl;
		}
	}
	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
	out.close();
}

void DemoSciCompilerList() {
	int res = 0;
	res = sdk.AddNewDevice("10.105.250.18", "V2740", "SciSDKDev2740Wave.json", "board0");
	
	SCISDK_LIST_RAW_BUFFER* ddb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/List_0", T_BUFFER_TYPE_RAW, (void**)&ddb, 1000));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/List_0.thread", "false"));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", ""));

	res = sdk.ReadData("board0:/MMCComponents/List_0", (void*)ddb);
	if (res == NI_OK) {
		uint32_t* p;
		p = (uint32_t*)ddb->data;
		cout << "----- " << ddb->info.valid_samples / 4 << " -----" << endl;
		for (int i = 0; i < ddb->info.valid_samples / 4; i++) {
			cout << std::hex << p[i] << endl;
		}
	}


	exit(0);
	
}

void DemoSciCompilerBoardApi() {
	int res = 0;
	res = sdk.AddNewDevice("usb:10500", "dt1260", "DT1260Frame.json", "board0");

	SCISDK_FRAME_DECODED_BUFFER* ddb;
	sdk.SetParameter("board0:/boardapi/analog.offset", 300);
}

void DemoSciCompilerFrame() {
	int res = 0;
	res = sdk.AddNewDevice("usb:10500", "dt1260", "DT1260Frame.json", "board0");

	SCISDK_FRAME_DECODED_BUFFER* ddb;
	sdk.SetParameter("board0:/boardapi/analog/CH1.offset", 300);
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.acq_mode", "blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.thread", "false"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.trigger_mode", "trig"));
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/ImageReadout_0", T_BUFFER_TYPE_DECODED, (void**)&ddb, 1000));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/ImageReadout_0.start", ""));

	while (1) {
		res = sdk.ReadData("board0:/MMCComponents/ImageReadout_0", (void*)ddb);
		if (res == NI_OK) {
			uint32_t* p;
			for (int i = 0; i < ddb->info.valid_data ; i++) {
				cout << setw(6) << ddb->data[i].pixel[0] << " " << setw(6) << ddb->data[i].pixel[1] << " " << setw(6) << ddb->data[i].pixel[2] << endl;
				cout << setw(6) << ddb->data[i].pixel[3] << " " << setw(6) << ddb->data[i].pixel[4] << " " << setw(6) << ddb->data[i].pixel[5] << endl;
				cout << setw(6) << ddb->data[i].pixel[6] << " " << setw(6) << ddb->data[i].pixel[7] << " " << setw(6) << ddb->data[i].pixel[8] << endl;
			}
			cout << "-----  -----" << endl;

		}
	}
	exit(0);

}

void DemoV2495_V4718_ETH() {
	string test;
	int res = 0;
	uint32_t v;

	//TEST SCOPE
	res = sdk.AddNewDevice("V4718ETH:v4718.sw:0x32100000", "V2495", "RegisterFileV2495.json", "board0");
	sdk.SetParameter("board0:/boardapi/gd[0].delay", 100);
	sdk.p_error(sdk.SetRegister("board0:/Registers/bb", 1));
	sdk.p_error(sdk.GetRegister("board0:/Registers/aa", &v));
	cout << "Register aa access result: " << v << endl;
	sdk.p_error(sdk.GetRegister("board0:/Registers/cc", &v));
	cout << "Register cc access result: " << v << endl;

}
void DemoV2495_USB_LogicAnalyser() {
	string test;
	int res = 0;
	uint32_t v;
	//TEST SCOPE
	res = sdk.AddNewDevice("usb:62702", "V2495", "E:/GIT/s800-fw/library/RegisterFile.json", "board0");
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software"));
	SCISDK_LOGICANALYSER_DECODED_BUFFER* logic_evnt;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&logic_evnt));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", ""));
	res = sdk.ReadData("board0:/MMCComponents/LogicAnalyser_0", (void*)logic_evnt);
	if (res == NI_OK) {
		cout << "Logic Analyser TS: " << logic_evnt->timecode << endl;

		// Apri file output
		ofstream outfile("logic_analyser_data.txt");

		// I dati sono organizzati così:
		// data[0] = sample 0, canali 0-31
		// data[1] = sample 0, canali 32-63 (se ntraces > 32)
		// data[2] = sample 1, canali 0-31
		// data[3] = sample 1, canali 32-63
		// ...

		// Scrivi i dati: ogni riga è un sample, ogni colonna è una trace
		for (int sample = 0; sample < logic_evnt->info.samples; sample++) {
			for (int trace = 0; trace < logic_evnt->info.ntraces; trace++) {
				// Calcola quale word contiene questa trace
				int word_offset = trace / 32;  // 0 per trace 0-31, 1 per trace 32-63, etc.
				int bit_idx = trace % 32;       // bit dentro la word

				// Indice nel buffer: sample * words_per_sample + word_offset
				int data_idx = sample * logic_evnt->info.words_per_sample + word_offset;

				// Estrai il bit
				uint32_t word = logic_evnt->data[data_idx];
				uint32_t bit = (word >> bit_idx) & 0x1;

				outfile << bit;
				if (trace < logic_evnt->info.ntraces - 1) {
					outfile << "\t";
				}
			}
			outfile << endl;
		}

		outfile.close();
		cout << "Dati salvati in logic_analyser_data.txt" << endl;
	}
}
int main()
{
	// Print actual executable path
	std::cout << "SciSDK Version: " << SCISDK_VERSION << std::endl;
	

	//Just run one demo at time!

	//OpenDppDemo();
	//OpenScopeDemo();
	//DemoSciCompilerOscilloscope();
	DemoV2495_USB_LogicAnalyser();

	return 0;

}






