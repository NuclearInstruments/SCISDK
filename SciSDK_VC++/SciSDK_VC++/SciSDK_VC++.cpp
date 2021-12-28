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
	sdk.StrobeRegister("Registers/res", "pos");
	
	sdk.SetParameter("Oscilloscope_0/decimator", 10);
	sdk.SetParameter("Oscilloscope_0/trigger", "ext");
	sdk.SetParameter("Oscilloscope_0/acq_mode", "blocking");
	sdk.SetParameter("Oscilloscope_0/data_processing", "decode");

	sdk.ExecuteCommand("Oscilloscope_0", "start");

	sdk.AllocateBuffer("Oscilloscope_0", "decoded_buffer", (void **) &osc_data);
	
	for (int i = 0; i < 10; i++) {
		sdk.ReadData("Oscilloscope_0", osc_data);
		dump_to_file(osc_data);
	}
    
	return 0;
}

