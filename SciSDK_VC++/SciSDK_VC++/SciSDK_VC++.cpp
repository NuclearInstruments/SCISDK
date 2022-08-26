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
	sdk.p_error(sdk.AddNewDevice("usb:10500", "dt1260", "fake_citiroc.json", "board0"));
	//sdk.p_error(sdk.SetRegister("board0:/Registers/res", 1));
	//sdk.p_error(sdk.SetRegister("board0:/Registers/res", 0));

	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "ext"));
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

	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Spectrum_0", ""));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0", ""));

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
/*
	SCISDK_LIST_RAW_BUFFER *ddb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/List_0", T_BUFFER_TYPE_RAW, (void**)&ddb, 1000));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "non-blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/List_0.thread", "true"));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", ""));

	while (1) {
		int ret = sdk.ReadData("board0:/MMCComponents/List_0", (void *)ddb);
		if (ret == NI_OK) {
			uint32_t *p;
			p = (uint32_t *)ddb->data;
			cout << "----- " << ddb->info.valid_samples / 4 << " -----" << endl;
			for (int i = 0; i < ddb->info.valid_samples/4; i++) {
				cout << std::hex  << p[i] << endl;
			}
		}
	}*/

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

	/*SCISDK_CP_RAW_BUFFER *cpb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/CP_0", T_BUFFER_TYPE_RAW, (void**)&cpb, 100));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.acq_mode", "blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.thread", "true"));
	sdk.SetParameter("board0:/MMCComponents/CP_0.timeout", 5000);
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.data_processing", "raw"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CP_0.check_align_word", "false"));


	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CP_0.start", ""));

	while (1) {
		int ret = sdk.ReadData("board0:/MMCComponents/CP_0", (void *)cpb);

			for (int i = 0; i < cpb->info.valid_data; i++) {
				std::cout << std::hex << cpb->data[i] << endl;
			}

	}*/
	/*SCISDK_RM_RAW_BUFFER *rmb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", T_BUFFER_TYPE_RAW, (void**)&rmb));

	sdk.ReadData("board0:/MMCComponents/RateMeter_0", (void *)rmb);
	for (int i = 0; i < rmb->info.nchannels; i++)
	{
		cout << rmb->data[i] << endl;
	}	*/

	/*sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CitirocCfg0.sc_enDiscri", 1));
	string value = "";
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CitirocCfg0.generate_bit_config", ""));
	sdk.p_error(sdk.GetParameter("board0:/MMCComponents/CitirocCfg0.bitstream", &value));
	cout << "Bitstream: " << value << endl;*/
	string bit_config_str = "1111111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110111011101110110111111111111111111111111111111111111111111011111100111111101111001111011100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001100000001000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000000100000000000111111110100101100010010110011111111111111011";
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/CitirocCfg0.bitstream", bit_config_str));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/CitirocCfg0.decode_bit_config", ""));
/*
	sdk.p_error(sdk.GetParameter("board0:/MMCComponents/CitirocCfg0.sc_enDiscri", value));*/

	return 0;
}

