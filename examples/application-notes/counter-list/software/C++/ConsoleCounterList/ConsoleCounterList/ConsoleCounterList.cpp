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
        cout << "Connection error " << std::hex << res << endl;
        exit(-1);
    }

    // Set registers
    res = sdk.SetRegister("board0:/Registers/trgthrs", 2000);
    res = sdk.SetRegister("board0:/Registers/pol", 1);
    res = sdk.SetRegister("board0:/Registers/PulsePeriod", 6250000);

    if (res == 0) {
        cout << "Register value has been successully set" << endl;
    }
    else {
        cout << "Error while trying to write register value" << endl;
    }

    // Allocate buffer raw, size 1024
    SCISDK_LIST_RAW_BUFFER* lrb;
    res = sdk.AllocateBuffer("board0:/MMCComponents/List_0", T_BUFFER_TYPE_RAW, (void**)&lrb, 1024);
    
    // Set parameters List
    res = sdk.SetParameter("board0:/MMCComponents/List_0.thread", "false");
    res = sdk.SetParameter("board0:/MMCComponents/List_0.timeout", 500);
    res = sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "blocking");

    res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "");

    // Read data buffered
    res = sdk.ReadData("board0:/MMCComponents/List_0", (void*)lrb);

    for (int j = 0; j < 10; j++)
    {
        res = sdk.ReadData("board0:/MMCComponents/List_0", (void*)lrb);
        cout <<"valid sample"<< lrb->info.valid_samples << endl;
        for (int i = 0; i < lrb->info.valid_samples; i += 4)
        {
            uint8_t value1 = static_cast<uint8_t>(lrb->data[i]);
            uint8_t value2 = static_cast<uint8_t>(lrb->data[i+1]);
            uint8_t value3 = static_cast<uint8_t>(lrb->data[i+2]);
            uint8_t value4 = static_cast<uint8_t>(lrb->data[i+3]);
            uint32_t value = static_cast<uint32_t>((value4 << 24) | (value3 << 16) | (value2 << 8) | value1);
            cout << value << endl;
        }
    }

    if (res != NI_OK) {
        printf("Error reading data\n");
        return -1;
    }

    // Detach device
    sdk.DetachDevice("board0");

}
