// SciSDK_VC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/scisdk_core.h"
#include <bitset>
SciSDK sdk;


void dump_to_file(SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data) {

}

int main()
{
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data;

	sdk.p_error(sdk.AddNewDevice("usb:0004", "dt1260", "SCIDKTester.json", "board0"));
	sdk.p_error(sdk.SetRegister("board0:/Registers/res", 1));
	sdk.p_error(sdk.SetRegister("board0:/Registers/res", 0));

	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 10));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode"));
	//sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 5000));
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	SCISDK_OSCILLOSCOPE_RAW_BUFFER *rb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**) &ob));
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", ""));
	
	//while (1) {
		std::ofstream out("c:/temp/output.txt");
		sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob);

		for (int i = 0; i < ob->info.samples_analog; i++) {
		out << ob->analog[i] << endl;
		}
		out.close();
	//}
		sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
		sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);

	//SCISDK_DIGITIZER_DECODED_BUFFER *ddb;
	//sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0", T_BUFFER_TYPE_DECODED, (void**)&ddb);
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.data_processing", "decode");
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.enabledch", 1);
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_len", 10);
	//sdk.SetParameter("board0:/MMCComponents/Digitizer_0.timeout", 100);
	//sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", "");
	//while (1) {
	//	int ret = sdk.ReadData("board0:/MMCComponents/Digitizer_0", (void *)ddb);
	//	if (ret == NI_OK) {
	//		for (int i = 0; i < ddb->info.valid_samples; i++) {
	//			cout << ddb->analog[i] << endl;
	//		}
	//		/*for (int i = 0; i < ddb->info.valid_samples; i++) {
	//			cout << ddb->analog[i + ddb->info.samples] << endl;
	//		}*/
	//	}
	//}

	SCISDK_DIGITIZER_RAW_BUFFER *ddb;
	sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0", T_BUFFER_TYPE_RAW, (void**)&ddb, 100));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.data_processing", "raw"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.enabledch", 1));
	sdk.p_error(__LINE__, sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_len", 1000));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_mode", "non-blocking"));
	sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Digitizer_0.timeout", 10000));
	sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", ""));
	while (1) {
		/*int ret = sdk.ReadData("board0:/MMCComponents/Digitizer_0", (void *)ddb);
		if (ret == NI_OK) {
			for (int i = 0; i < ddb->info.valid_samples; i++) {
				cout << std::hex  << ddb->data[i] << endl;
			}	
		}*/
	}
	return 0;
}

