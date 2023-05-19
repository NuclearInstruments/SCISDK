
#include <SciSDK_DLL.h>
#include <NIErrorCode.h>
#include <iostream>

// import root dependencies
#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMarker.h>
#include <TApplication.h>
#include <TMultiGraph.h>

using namespace std;


int main(int argc, char** argv) {
	void* sdk = SCISDK_InitLib();

	char* error_description = (char*)"";

	// connect to device
	int ret = SCISDK_AddNewDevice((char*)"usb:28645", (char*)"dt1260", (char*)"./json/DT1260RegisterFile.json", (char*)"board0", sdk);
	if (ret) {
		SCISDK_FreeLib(sdk);
		return 1;
	}

	// set digitizer parameter
	ret = SCISDK_SetParameterString((char*)"board0:/MMCComponents/Oscilloscope_0.data_processing", (char*)"decoded", sdk);
	ret = SCISDK_SetParameterInteger((char*)"board0:/MMCComponents/Digitizer_0.trigger_level", 1500, sdk);
	ret = SCISDK_SetParameterString((char*)"board0:/MMCComponents/Oscilloscope_0.trigger_mode", (char*)"self", sdk);
	ret = SCISDK_SetParameterInteger((char*)"board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, sdk);
	ret = SCISDK_SetParameterInteger((char*)"board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, sdk);
	ret = SCISDK_SetParameterInteger((char*)"board0:/MMCComponents/Oscilloscope_0.decimator", 0, sdk);
	ret = SCISDK_SetParameterString((char*)"board0:/MMCComponents/Oscilloscope_0.acq_mode", (char*)"blocking", sdk);
	ret = SCISDK_SetParameterInteger((char*)"board0:/MMCComponents/Oscilloscope_0.timeout", 1000, sdk);

	SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER* odb;
	ret = SCISDK_AllocateBuffer((char*)"board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&odb, sdk);
	if (ret == NI_OK) {

		// read data and plot graph with ROOT
		ret = SCISDK_ReadData((char*)"board0:/MMCComponents/Oscilloscope_0", (void*)odb, sdk);

		if (ret == NI_OK) {
			TApplication theApp("Oscilloscope", NULL, NULL);
			Int_t n = odb->info.samples_analog;
			
			TCanvas* c1 = new TCanvas("c1", "Graph Draw Options", 200, 10, 600, 400);
			TGraph** graphs = (TGraph**)malloc(sizeof(TGraph) * odb->info.channels);
			TMultiGraph* mg = new TMultiGraph();

			for (int i = 0;i < odb->info.channels;i++) {
				Double_t* x, * y;
				x = (Double_t*)malloc(odb->info.samples_analog * sizeof(Double_t));
				y = (Double_t*)malloc(odb->info.samples_analog * sizeof(Double_t));

				for (int j = 0; j < odb->info.samples_analog; j++)
				{
					x[j] = j;
					y[j] = odb->analog[j + i * odb->info.samples_analog];
				}
				// add graph to multi graph
				graphs[i] = new TGraph(odb->info.samples_analog, x, y);
				graphs[i]->SetLineColor(41 + i);
				string graph_name = "Channel " + to_string(i);
				graphs[i]->SetName(graph_name.c_str());
				graphs[i]->SetTitle(graph_name.c_str());
				mg->Add(graphs[i]);

				free(x);
				free(y);
			}
			
			mg->Draw("AL");
			c1->BuildLegend();
			c1->cd(4);
			c1->Update();
			theApp.Run();
			free(graphs);
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
		cout << "ERROR, cannot allocate oscilloscope buffer: " << error_description << endl;
	}

	// detach device
	ret = SCISDK_DetachDevice((char*)"board0", sdk);
	SCISDK_FreeLib(sdk);

	return 0;
}