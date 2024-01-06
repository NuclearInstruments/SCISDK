// SciSDK_DLL_Info.cpp : Defines the entry point for the console application.
//

//#include "../../src/SciSDK_DLL.h"

#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
using namespace std;

#ifdef _MSC_VER 

#else
#define __cdecl 
#include <dlfcn.h>
#endif


#ifdef _MSC_VER 
#include <windows.h> 
#else
#define HINSTANCE void *
#endif

typedef void* SCISDK_HANDLE;

int GetSciSDKVersion() {
	HINSTANCE h_lib_instance;
#ifdef _MSC_VER 
	h_lib_instance = LoadLibrary(L"SciSDK_DLL.dll");
#else
	h_lib_instance = dlopen("libSCISDK.so", RTLD_LAZY);
#endif

	if (h_lib_instance == NULL) {
		cout << "SciSDK library can not be loaded ..." << endl;
		return -1;
	}
	else {
		cout << "SciSDK library loaded correclty ..." << endl;

#ifdef _MSC_VER 
		wchar_t path[1024];
		if (GetModuleFileName(h_lib_instance, path, sizeof(path) / sizeof(wchar_t))) {
			std::wcout << L"DLL Path: " << path << std::endl;
		}
		else {
			std::wcout << L"Unable to locate the DLL path." << std::endl;
		}
#else
		void* sym = dlsym(handle, "SCISDK_InitLib");

		// Utilizza dladdr per ottenere informazioni sulla libreria
		if (dladdr(sym, &dl_info)) {
			std::cout << L"SO Path: " <<  dl_info.dli_fname << std::endl;
		}
		else {
			std::cout << L"Unable to locate the SO path." << std::endl;
		}

#endif
#ifdef _MSC_VER 
		typedef SCISDK_HANDLE(__cdecl* CONNECT_PROC_PTR)();
		CONNECT_PROC_PTR _SCISDK_InitLib = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "SCISDK_InitLib");
#else
		SCISDK_HANDLE(*_SCISDK_InitLib)(char*, NI_HANDLE*);
		*(void**)(&_SCISDK_InitLib) = dlsym(h_lib_instance, "SCISDK_InitLib");
#endif
		if (_SCISDK_InitLib) {

			SCISDK_HANDLE scisdk_handle = _SCISDK_InitLib();
			if (scisdk_handle == NULL) {
				cout << "SCISDK_InitLib failed ..." << endl;
				return -2;
			}
			else {
				cout << "SCISDK_InitLib succeeded ..." << endl;
#ifdef _MSC_VER 
				typedef int(__cdecl* CONNECT_PROC_PTR)(char** ret, SCISDK_HANDLE handle);
				CONNECT_PROC_PTR _SCISDK_GetLibraryVersion = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "SCISDK_GetLibraryVersion");
#else
				int(*_SCISDK_GetLibraryVersion)(char** ret, SCISDK_HANDLE handle);
				*(void**)(&_SCISDK_GetLibraryVersion) = dlsym(h_lib_instance, "SCISDK_GetLibraryVersion");
#endif
				if (_SCISDK_GetLibraryVersion) {
					char* version;
					version = (char*)malloc(100);
					int r = _SCISDK_GetLibraryVersion(&version, scisdk_handle);
					if (r != 0) {
						cout << "SCISDK_GetLibraryVersion failed ..." << endl;
						return -3;
					}
					else {
						cout << "SCISDK_GetLibraryVersion succeeded ..." << endl;
						cout << "SCISDK_GetLibraryVersion version: " << version << endl;
					}
				}
				else {
					cout << "SCISDK_GetLibraryVersion not found ..." << endl;
					return -4;
				}
			}
		}
		FreeLibrary(h_lib_instance);
	}
	return 0;
}

void DependancyLibraryInfo(string lib) {
	std::wstring stemp = std::wstring(lib.begin(), lib.end());
	LPCWSTR sw = stemp.c_str();
	HINSTANCE h_lib_instance;
#ifdef _MSC_VER 
	h_lib_instance = LoadLibrary(sw);
#else
	h_lib_instance = dlopen(lib.c_str(), RTLD_LAZY);
#endif

	if (h_lib_instance == NULL) {
		cout << "[" << lib << "] \t\t NOT FOUND" << endl;
		return ;
	}
	else {
		wchar_t path[1024];
		path[0] = 0;
#ifdef _MSC_VER 
		if (GetModuleFileName(h_lib_instance, path, sizeof(path) / sizeof(wchar_t))) {
		}
		else {
			std::wcout << L"Unable to locate the DLL path." << std::endl;

	}
#else


#endif
		cout << "[" << lib << "] \t\t AVAILABLE" << "\t"; std::wcout << path << endl;
	}
}

int main(int argc, char* argv[])
{
	int r = GetSciSDKVersion();
	if (r != 0) {
		return r;
	}
	cout << "----------------------------------------" << endl;
	cout << "Status of the board driver libraries:" << endl;
#ifdef _MSC_VER 
	DependancyLibraryInfo("SCIDK_Lib.dll");
	DependancyLibraryInfo("R5560_SDKLib.dll");
	DependancyLibraryInfo("DT5771_SDKLib.dll");
	DependancyLibraryInfo("DT4810_SDKLib.dll");
	DependancyLibraryInfo("CAEN_FELib.dll");
#else
	DependancyLibraryInfo("libSCIDK.so");
	DependancyLibraryInfo("libr5560");
	DependancyLibraryInfo("libdt5771.so");
	DependancyLibraryInfo("libdt4810.so");
	DependancyLibraryInfo("caen_feelib.so");
#endif
	
	return 0;

}

