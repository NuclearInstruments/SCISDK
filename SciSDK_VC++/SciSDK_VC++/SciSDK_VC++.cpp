// SciSDK_VC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/scisdk_core.h"

SciSDK sdk;

void dump_to_file(SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data) {

}

int main()
{
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *osc_data;

	sdk.AddNewDevice("usb:0004", "dt1260", "SCIDKTester.json", "board0");
	sdk.SetRegister("board0:/Registers/res", 1);
	sdk.SetRegister("board0:/Registers/res", 0);

	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog");
	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000);
	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 10);
	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
	//sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
	sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 5000);
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	SCISDK_OSCILLOSCOPE_RAW_BUFFER *rb;
	sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**) &ob);
	sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);
	sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "");
	
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
	return 0;
}

