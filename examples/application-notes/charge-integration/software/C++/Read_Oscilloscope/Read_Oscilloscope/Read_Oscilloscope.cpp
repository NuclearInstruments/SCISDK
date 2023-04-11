#include <iostream>
#include <fstream>
#include <iomanip>
#include "scisdk.hpp"

using namespace std;

int main()
{
    // Initialize a scisdk library
    SciSDK sdk;

    // Add new device
    int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

    // Print error message
    if (res != NI_OK) {
        cout << "Connection error " << res << endl;
        exit(-1);
    }

    // Set registers
    res = sdk.SetRegister("board0:/Registers/pol", 1);
    res = sdk.SetRegister("board0:/Registers/trgthrs", 2000);
    res = sdk.SetRegister("board0:/Registers/Nsample", 7);
    res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100);
    res = sdk.SetRegister("board0:/Registers/time_integr", 76);
    res = sdk.SetRegister("board0:/Registers/pre_integr", 5);
    res = sdk.SetRegister("board0:/Registers/gain", 30000);
    res = sdk.SetRegister("board0:/Registers/Offset_int", 0);

    if (res == 0) {
        cout << "Register value has been successully set" << endl;
    }
    else {
        cout << "Error while trying to write register value" << endl;
    }

    // // OSCILLOSCOPE
    // Set parameters oscilloscope
    sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "ext");
    sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
    sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 1);
    sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
    sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
    sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000);

    string directoryPath = "C:\\GIT\\SCISDK\\examples\\application-notes\\charge-integration\\software\\C++\\Read_Oscilloscope";

    SCISDK_OSCILLOSCOPE_DECODED_BUFFER* ob;
    res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
    
    if (res != NI_OK) {
        cout << "Error allocating buffer" << endl;
        return -1;
    }

    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "");
    
    sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void*)ob);
    int offs_dig = (int)ob->info.samples_digital * (int)ob->info.tracks_digital_per_channel;
    int offs_an = (int)ob->info.samples_analog;
    cout << offs_dig << endl;
    cout << offs_an << endl;
    

    if (res == 0) {
        //========================================================
        string str_to_wr = string("Analog Channels") + "\n";
        str_to_wr += string("0    ") + "\t    " + "1    " + "\t    " + "2    " + "\t    " + "3    " + "\n";
        str_to_wr += string("Signal") + "\t    " + "Baseline" + "\t    " + "Trig data" + "\t    " + "Energy" + "\n";
        for (int i = 0; i < ob->info.samples_analog; i++) {
            str_to_wr += to_string(ob->analog[i]) + "\t    "
                + to_string(ob->analog[i + offs_an * 1]) + "\t    "
                + to_string(ob->analog[i + offs_an * 2]) + "\t    "
                + to_string(ob->analog[i + offs_an * 3]) + "\n";
        }
        string fileName = "Analog.txt";
        string filePath = directoryPath + "\\" + fileName;
        ofstream outAnalog(filePath);
        outAnalog << str_to_wr;
        outAnalog.close();
        cout << "Write spectrum data on " + filePath << endl;

        //========================================================
        str_to_wr = string("Digital0") + "\n" + "analog channels" + "\n";
        str_to_wr += string("0    ") + "\t     " + "1    " + "\t" + "2    " + "\t     " + "3    " + "\n";
        str_to_wr += string("No data") + "\t     " + "Valid BL" + "\t" + "Trigger" + "\t     " + "Valid charge" + "\n";
        for (int i = 0; i < ob->info.samples_analog; i++) {
            str_to_wr += to_string(ob->digital[i]) + "\t     "
                + to_string(ob->digital[i + offs_dig * 1]) + "\t" + "\t" 
                + to_string(ob->digital[i + offs_dig * 2]) + "\t     "
                + to_string(ob->digital[i + offs_dig * 3]) + "\n";
        }
        fileName = "Digital0.txt";
        filePath = directoryPath + "\\" + fileName;
        ofstream outDig0(filePath);
        outDig0 << str_to_wr;
        outDig0.close();
        cout << "Write spectrum data on " + filePath << endl;

        //========================================================
        str_to_wr = string("Digital1") + "\n" + "analog channels" + "\n";
        str_to_wr += string("0    ") + "\t     " + "1    " + "\t  " + "2    " + "\t     " + "3    " + "\n";
        str_to_wr += string("No data") + "\t     " + "Run/hold BL" + "  " + "No data" + "    " + "Integration Gate" + "\n";
        for (int i = 0; i < ob->info.samples_analog; i++) {
            str_to_wr += to_string(ob->digital[i + offs_an]) + "\t     "
                + to_string(ob->digital[i + offs_dig * 1 + offs_an]) + "\t" + "\t  "
                + to_string(ob->digital[i + offs_dig * 2 + offs_an]) + "\t     "
                + to_string(ob->digital[i + offs_dig * 3 + offs_an]) + "\n";
        }
        fileName = "Digital1.txt";
        filePath = directoryPath + "\\" + fileName;
        ofstream outDig1(filePath);
        outDig1 << str_to_wr;
        outDig1.close();
        cout << "Write spectrum data on " + filePath << endl;

        //========================================================
        str_to_wr = string("Digital2") + "\n" + "analog channels" + "\n";
        str_to_wr += string("0    ") + "\t  " + "1    " + "\t   " + "2" + "\t    " + "3    " + "\n";
        str_to_wr += string("No data") + "\t  " + "No data" + "  " + "No data" + "  " + "Pileup events" + "\n";
        for (int i = 0; i < ob->info.samples_analog; i++) {
            str_to_wr += to_string(ob->digital[i + offs_an * 2]) + "\t  "
                + to_string(ob->digital[i + offs_dig * 1 + offs_an * 2]) + "\t   " 
                + to_string(ob->digital[i + offs_dig * 2 + offs_an * 2]) + "\t    "
                + to_string(ob->digital[i + offs_dig * 3 + offs_an * 2]) + "\n";
        }
        fileName = "Digital2.txt";
        filePath = directoryPath + "\\" + fileName;
        ofstream outDig2(filePath);
        outDig2 << str_to_wr;
        outDig2.close();
        cout << "Write spectrum data on " + filePath << endl;

    }

    else {
        cout << "Error while trying to read oscilloscope value" << endl;
    }
}



