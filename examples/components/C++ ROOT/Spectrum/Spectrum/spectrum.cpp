
#include <SciSDK_DLL.h>
#include <NIErrorCode.h>
#include <iostream>

// import root dependencies
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMarker.h>
#include <TApplication.h>

using namespace std;


int main(int argc, char** argv) {
	void* sdk = SCISDK_InitLib();

	char* error_description = (char*)"";

	// connect to device
	int ret = SCISDK_AddNewDevice((char*)"usb:28645", (char*)"dt1260", (char*)".\\json\\DT1260RegisterFile.json", (char*)"board0", sdk);
	if (ret) {
		char* error_description = (char*)"";
		SCISDK_s_error(ret, &error_description, sdk);
		cout << "ERROR Program exit due connection error: " << error_description << endl;
		SCISDK_FreeLib(sdk);
		return 1;
	}

	// set digitizer parameter
	ret = SCISDK_SetParameterString((char*)"board0:/MMCComponents/Spectrum_0.rebin", (char*)"0", sdk);
	ret = SCISDK_SetParameterString((char*)"board0:/MMCComponents/Spectrum_0.limitmode", (char*)"freerun", sdk);
	ret = SCISDK_SetParameterString((char*)"board0:/MMCComponents/Spectrum_0.limit", (char*)"100", sdk);

	// execute reset command
	ret = SCISDK_ExecuteCommand((char*)"board0:/MMCComponents/Spectrum_0.reset", (char*)"", sdk);
	// execute start command
	ret = SCISDK_ExecuteCommand((char*)"board0:/MMCComponents/Spectrum_0.start", (char*)"", sdk);

	SCISDK_SPECTRUM_DECODED_BUFFER* sdb;
	ret = SCISDK_AllocateBuffer((char*)"board0:/MMCComponents/Spectrum_0", T_BUFFER_TYPE_DECODED, (void**)&sdb, sdk);
	if (ret == NI_OK) {

		// read data and plot graph with ROOT
		ret = SCISDK_ReadData((char*)"board0:/MMCComponents/Spectrum_0", (void*)sdb, sdk);

		if (ret == NI_OK) {
			TApplication theApp("Spectrum", NULL, NULL);
			Int_t n = sdb->info.valid_bins;
			Double_t* x, * y;

			x = (Double_t*)malloc(sdb->info.valid_bins * sizeof(Double_t));
			y = (Double_t*)malloc(sdb->info.valid_bins * sizeof(Double_t));

			for (int i = 0; i < sdb->info.valid_bins; i++)
			{
				x[i] = i;
				y[i] = sdb->data[i];
			}

			// create graphs
			TGraph* gr3 = new TGraph(sdb->info.valid_bins, x, y);
			gr3->SetTitle("Spectrum");
			TCanvas* c1 = new TCanvas("c1", "Graph Draw Options", 200, 10, 600, 400);

			c1->cd(4);
			gr3->Draw("APL");
			c1->Update();

			theApp.Run();

			free(x);
			free(y);
		}
		else {
			char* error_description = (char*)"";
			SCISDK_s_error(ret, &error_description, sdk);
			cout << "ERROR, cannot read data: " << error_description << endl;
		}

	}
	else {
		char* error_description = (char*)"";
		SCISDK_s_error(ret, &error_description, sdk);
		cout << "ERROR, cannot allocate spectrum buffer: " << error_description << endl;
	}

	// detach device
	ret = SCISDK_DetachDevice((char*)"board0", sdk);
	SCISDK_FreeLib(sdk);

	return 0;
}