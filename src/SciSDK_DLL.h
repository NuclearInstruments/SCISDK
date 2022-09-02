// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCISDK_DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCISDK_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifdef SCISDK_DLL_EXPORTS
#define SCISDK_DLL_API extern "C" __declspec(dllexport)
#else
#define SCISDK_DLL_API extern "C" __declspec(dllimport)
#endif


SCISDK_DLL_API void * SCISDK_InitLib();
SCISDK_DLL_API int SCISDK_FreeLib(void *handle);
SCISDK_DLL_API int SCISDK_AddNewDevice(char *DevicePath, char *DeviceModel, char *JSONFwFilePath, char *Name, void *handle);
SCISDK_DLL_API int SCISDK_DetachDevice(char* name, void*handle);
SCISDK_DLL_API int SCISDK_SetParameterString(char* Path, char* value, void* handle);
SCISDK_DLL_API int SCISDK_SetParameterInteger(char* Path, int value, void*handle);
SCISDK_DLL_API int SCISDK_SetParameterDouble(char* Path, double value, void*handle);
SCISDK_DLL_API int SCISDK_GetParameterString(char* Path, char* value, void* handle);
SCISDK_DLL_API int SCISDK_GetParameterInteger(char* Path, int *value, void*handle);
SCISDK_DLL_API int SCISDK_GetParameterDouble(char* Path, double*value, void*handle);
SCISDK_DLL_API int SCISDK_DecodeData(char* Path, void*buffer_in, void*buffer_out, void*handle);
SCISDK_DLL_API int SCISDK_AllocateBuffer(char* Path, int buffer_type, void **buffer, void *handle);
SCISDK_DLL_API int SCISDK_SetRegister(char* Path, int value, void* handle);
SCISDK_DLL_API int SCISDK_GetRegister(char* Path, int*value, void*handle);
SCISDK_DLL_API int SCISDK_FreeBuffer(char* Path, int buffer_type, void **buffer, void*handle);
SCISDK_DLL_API int SCISDK_ReadData(char *Path, void *buffer, void*handle);
SCISDK_DLL_API int SCISDK_s_error(int err_no, char* value, void* handle);
SCISDK_DLL_API int SCISDK_ExecuteCommand(char* Path, char* value, void* handle);
