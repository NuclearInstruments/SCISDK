// SciSDK_VC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/scisdk_core.h"

SciSDK sdk;

int main()
{
	sdk.AddNewDevice("usb:0006", "dt1260", "SCIDKTester.json", "board0");
	
    return 0;
}

