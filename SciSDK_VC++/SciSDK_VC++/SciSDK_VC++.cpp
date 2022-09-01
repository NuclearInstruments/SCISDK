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

int main()
{
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data;
	//usb:10500
	//usb:13250
	//sdk.p_error(sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0"));

	//sdk.p_error(sdk.SetRegister("board0:/Registers/res", 1));
	//sdk.p_error(sdk.SetRegister("board0:/Registers/res", 0));


	//// OSCILLOSCOPE (decoded & raw)
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
	//	std::ofstream out("c:/temp/output.txt");
	//	sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob);
	//	for (int i = 0; i < ob->info.samples_analog; i++) {
	//	out << ob->analog[i] << endl;
	//	}
	//	out.close();
	////}
	//	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
	//	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);


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


	//// DIGITIZER decoded
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
	//SCISDK_RM_RAW_BUFFER *rmb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", T_BUFFER_TYPE_RAW, (void**)&rmb));
	//sdk.ReadData("board0:/MMCComponents/RateMeter_0", (void *)rmb);
	//for (int i = 0; i < rmb->info.nchannels; i++){
	//	cout << rmb->data[i] << endl;
	//}


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


	//// REGISTERS
	//uint32_t value;
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CREG_1.register_2", 12));
	//sdk.p_error(sdk.GetParameter("board0:/MMCComponents/CREG_0.register_2", &value));
	//cout << "test value " << value << endl;
	//for (int i = 0; i < 6; i++) {
	//	sdk.p_error(sdk.GetParameter("board0:/MMCComponents/REGFILE_0.register_" + to_string(i), &value));
	//	cout << "value " + to_string(i) + " " << value << endl;
	//}

	sdk.p_error(sdk.AddNewDevice("172.16.49.97:8888", "DT5560", "DT5560.json", "board0"));
	sdk.p_error(sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board1"));

	// DT5560
	uint32_t value;
	sdk.p_error(sdk.SetRegister("board0:/Registers/w1", 1));
	sdk.p_error(sdk.GetRegister("board0:/Registers/r1", &value));
	cout << "DT5560 register r1 value: " << value << endl;

	SCISDK_RM_RAW_BUFFER *rmb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", T_BUFFER_TYPE_RAW, (void**)&rmb));
	sdk.ReadData("board0:/MMCComponents/RateMeter_0", (void *)rmb);
	for (int i = 0; i < rmb->info.nchannels; i++) {
		cout << "DT5560 rate meter channel " << i << ": " << rmb->data[i] << endl;
	}

	// SciDK
	sdk.p_error(sdk.SetParameter("board1:/MMCComponents/CREG_1.register_2", 12));
	sdk.p_error(sdk.GetParameter("board1:/MMCComponents/CREG_0.register_2", &value));
	cout << "scidk test value " << value << endl;
	for (int i = 0; i < 6; i++) {
		sdk.p_error(sdk.GetParameter("board1:/MMCComponents/REGFILE_0.register_" + to_string(i), &value));
		cout << "scidk register " + to_string(i) + " value: " << value << endl;
	}

	return 0;

}

