#include <iostream>
#include <fstream>
#include <iomanip>
#include "scisdk.hpp"
using namespace std;

int main()
{
    SciSDK sdk;

    // Add new device
    int res= sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");
    
    // Print error message
    if (res != NI_OK) {
        cout << "Connection error " << std::hex << res << endl;
        exit(-1);
    }

    
    // Write register
    res = sdk.SetRegister("board0:/Registers/trgthrs", 2500);
    res = sdk.SetRegister("board0:/Registers/pol", 1);

    if (res == 0) {
        cout << "Register value has been successully set" << endl;
    }
    else {
        cout << "Error while trying to write register value" << endl;
    }

    // Read register
        
    uint32_t value;
    for (int i = 0; i <= 100; i++) {
        res = sdk.GetRegister("board0:/Registers/trgcnt", &value);
        printf("Register value = %u\n", value);

    }

    if (res == 0) {
        cout << "Register value is read" << endl;
    }
    else {
        cout << "Error while trying to read register value" << endl;
    }
    // Detach device
    sdk.DetachDevice("board0");
    
 
}

