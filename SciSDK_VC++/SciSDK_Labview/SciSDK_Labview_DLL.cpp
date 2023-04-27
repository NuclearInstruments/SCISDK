#include "SciSDK_Labview_DLL.h"

#include <stdio.h>
#include <string>

#include "../../src/SciSDK_DLL.h"
#include "extcode.h"
SCISDKLABVIEW_DLL_API void* LV_SCISDK_InitLib() {
	return SCISDK_InitLib();
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_FreeLib(void* handle) {
	void* handle_ptr = handle;
	return SCISDK_FreeLib(handle_ptr);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_AddNewDevice(char* DevicePath, char* DeviceModel, char* JSONFwFilePath, char* Name, void* handle) {
	return SCISDK_AddNewDevice(DevicePath, DeviceModel, JSONFwFilePath, Name, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_DetachDevice(char* name, void* handle) {
	return SCISDK_DetachDevice(name, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterString(char* Path, char* value, void* handle) {
	return SCISDK_SetParameterString(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterInteger(char* Path, int value, void* handle) {
	return SCISDK_SetParameterInteger(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterUInteger(char* Path, unsigned int value, void* handle) {
	return SCISDK_SetParameterUInteger(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterDouble(char* Path, double value, void* handle) {
	return SCISDK_SetParameterDouble(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterString(char* Path, char* value, void* handle) {
	char* value_ptr = (char*)"";
	int res = SCISDK_GetParameterString(Path, &value_ptr, handle);
	strcpy_s(value, strlen(value_ptr) + 1, value_ptr);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterInteger(char* Path, int* value, void* handle) {
	return  SCISDK_GetParameterInteger(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterUInteger(char* Path, unsigned int* value, void* handle) {
	return SCISDK_GetParameterUInteger(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterDouble(char* Path, double* value, void* handle) {
	return SCISDK_GetParameterDouble(Path, value, handle);
}
//
//SCISDKLABVIEW_DLL_API int LV_SCISDK_AllocateBuffer_Oscilloscope(char* Path, int buffer_type, LV_OSCILLOSCOPE_DECODED_BUFFER* buffer, void* handle)
//{
//	void** ptr = (void**)malloc(sizeof(void*));
//	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, ptr, handle);
//	memcpy(buffer, *ptr, sizeof(LV_OSCILLOSCOPE_DECODED_BUFFER));
//	free(ptr);
//	return res;
//}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscope(char* Path, TD_OSCILLOSCOPE* buffer, void* handle)
{
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&ob, handle);
	if (res) return res;
	res = SCISDK_ReadData(Path, ob, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&ob, handle);
		return res;
	}
	NumericArrayResize(iQ, 1, ( UHandle *)&(buffer->analog), ob->info.channels * ob->info.samples_analog * ob->info.tracks_analog_per_channel);
	for (int i = 0; i < ob->info.channels * ob->info.samples_analog; i++)
	{
 		(*(buffer->analog))->Numeric[i] = ob->analog[i];
	}
	(*(buffer->analog))->dimSize = ob->info.channels * ob->info.samples_analog;

	return res;
}