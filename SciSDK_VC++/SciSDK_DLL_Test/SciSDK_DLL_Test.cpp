// SciSDK_DLL_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/SciSDK_DLL.h"

#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;
int main()
{
	void* _sdk = SCISDK_InitLib();

	char * res = "";
	SCISDK_s_error(SCISDK_AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0", _sdk), res, _sdk);

	int value;
	SCISDK_s_error(SCISDK_SetParameterInteger("board0:/MMCComponents/CREG_1.register_2", 12, _sdk), res, _sdk);
	cout << res << endl;
	SCISDK_s_error(SCISDK_GetParameterInteger("board0:/MMCComponents/CREG_0.register_2", &value, _sdk), res, _sdk);
	cout << res << endl;
	cout << "value: " << value << endl;
	for (int i = 0; i < 6; i++) {
		string reg_path = "board0:/MMCComponents/REGFILE_0.register_" + to_string(i);
		SCISDK_s_error(SCISDK_GetParameterInteger((char*)reg_path.c_str(), &value, _sdk), res, _sdk);
		cout << "value " + to_string(i) + " " << value << endl;
	}
    return 0;
}

