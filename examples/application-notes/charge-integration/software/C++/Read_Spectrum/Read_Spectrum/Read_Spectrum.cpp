#include <iostream>
#include <fstream>
#include <iomanip>
#include "scisdk.hpp"
#include <chrono>
#include <thread>
#include <Windows.h>

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

    // // SPECTRUM
    // Set parameters spectrum
    res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0");
    res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");

    // Execute command reset
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");

    // Execute command start
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");

    // Waiting for data
    for (int i = 0; i < 50; i++)
    {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Waiting for data " << i << " s" << endl;
    }

    

    SCISDK_SPECTRUM_DECODED_BUFFER* obSpectrum;

    res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum);
    if (res != NI_OK) {
        cout << "Error allocating buffer" << endl;
        return -1;
    }

    //Read a single time the data after waiting 50s
    string directoryPath = "C:\\GIT\\SCISDK\\examples\\application-notes\\charge-integration\\software\\C++\\Read_Spectrum";
    string fileName = "Spectrum_one_time.txt";
    string filePath = directoryPath + "\\" + fileName;
    res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", (void*)obSpectrum);
    string str_to_wr = "";
    if (res == NI_OK) {
        for (int i = 0; i < obSpectrum->info.valid_bins; i++) {
            str_to_wr += to_string(i) + "\t" + to_string(obSpectrum->data[i]) + "\n";
            //printf("[%5d] -- %9d\n", i, obSpectrum->data[i]);
        }
    }
    ofstream outFile(filePath);
    outFile << str_to_wr;
    outFile.close();
    cout << "Write spectrum data on " + filePath << endl;


    int counts = 0;
    fileName = "Spectrum.txt";
    filePath = directoryPath + "\\" + fileName;

    bool go = true;
    cout << endl;
    cout << "Press 'Q' to end the loop" << endl;
    while (go) {
        //Just leave in your code of of two of this sleep functions
        this_thread::sleep_for(chrono::seconds(1));        // WINDOWS: sleep for 1s 
        res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", (void*)obSpectrum);
        counts++;
        string str_to_wr = "";

        if (res == NI_OK) {
            for (int i = 0; i < obSpectrum->info.valid_bins; i++) {
                str_to_wr += to_string(i) + "\t" + to_string(obSpectrum->data[i]) + "\n";
                //printf("[%5d] -- %9d\n", i, obSpectrum->data[i]);
            }
        }
        ofstream outFile(filePath);
        outFile << str_to_wr;
        outFile.close();

        cout << counts << " s"<< endl;

        if (GetAsyncKeyState('Q') & 0x8000)
        {
            go = false;
            cout << "Write spectrum data on " + filePath << endl;
        }
    }

    sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum);
    return 0;

}
