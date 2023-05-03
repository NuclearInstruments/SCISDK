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

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscope(char* Path, TD_OSCILLOSCOPE* buffer, void* handle)
{
	SCISDK_OSCILLOSCOPE_DECODED_BUFFER* ob;
	// allocate buffer
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&ob, handle);
	if (res) return res;
	// read data
	res = SCISDK_ReadData(Path, ob, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&ob, handle);
		return res;
	}
	// analog data
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->analog), ob->info.channels * ob->info.samples_analog * ob->info.tracks_analog_per_channel);

	for (int i = 0; i < ob->info.channels * ob->info.samples_analog * ob->info.tracks_analog_per_channel; i++)
	{
		(*(buffer->analog))->Numeric[i] = ob->analog[i];
	}

	(*(buffer->analog))->dimSizes[0] = ob->info.channels * ob->info.tracks_analog_per_channel;// rows
	(*(buffer->analog))->dimSizes[1] = ob->info.samples_analog;// columns

	// digital data
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->digital_data), ob->info.channels * ob->info.samples_digital * ob->info.tracks_digital_per_channel);
	for (int i = 0; i < ob->info.channels * ob->info.samples_digital * ob->info.tracks_digital_per_channel; i++)
	{
		(*(buffer->digital_data))->Numeric[i] = ob->digital[i];
	}
	(*(buffer->digital_data))->dimSizes[0] = ob->info.channels * ob->info.tracks_digital_per_channel;// rows
	(*(buffer->digital_data))->dimSizes[1] = ob->info.samples_digital;// columns

	// copy other informations
	buffer->magic = ob->magic;
	buffer->trigger_position = ob->trigger_position;
	buffer->timecode = ob->timecode;
	buffer->samples_analog = ob->info.samples_analog;
	buffer->samples_digital = ob->info.samples_digital;
	buffer->tracks_analog_per_channel = ob->info.tracks_analog_per_channel;
	buffer->tracks_digital_per_channel = ob->info.tracks_digital_per_channel;
	buffer->channels = ob->info.channels;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ob), handle);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadSpectrum(char* Path, TD_SPECTRUM* buffer, void* handle)
{
	SCISDK_SPECTRUM_DECODED_BUFFER* obSpectrum;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, handle);
	if (res) return res;
	// read data
	std::string path_tmp(Path);
	path_tmp += ".start";
	SCISDK_ExecuteCommand((char*)path_tmp.c_str(), (char*)"", handle);
	res = SCISDK_ReadData(Path, obSpectrum, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, handle);
		return res;
	}

	// copy data
	NumericArrayResize(iQ, 1, (UHandle*)(&(buffer->data)), obSpectrum->info.valid_bins);
	for (int i = 0; i < obSpectrum->info.valid_bins; i++)
	{
		(*(buffer->data))->Numeric[i] = obSpectrum->data[i];
	}
	(*(buffer->data))->dimSize = obSpectrum->info.valid_bins;

	// copy other informations
	buffer->magic = obSpectrum->magic;
	buffer->timecode = obSpectrum->timecode;
	buffer->inttime = obSpectrum->inttime;
	buffer->buffer_size = obSpectrum->info.buffer_size;
	buffer->total_bins = obSpectrum->info.total_bins;
	buffer->valid_bins = obSpectrum->info.valid_bins;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, handle);
	return 0;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadDigitizer(char* Path, TD_DIGITIZER* buffer, void* handle)
{
	SCISDK_DIGITIZER_DECODED_BUFFER* ddb;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ddb), handle);
	if (res) return res;

	std::string path_tmp(Path);
	path_tmp += ".start";
	SCISDK_ExecuteCommand((char*)path_tmp.c_str(), (char*)"", handle);

	// read data
	res = SCISDK_ReadData(Path, ddb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ddb), handle);
		return res;
	}

	NumericArrayResize(iQ, 1, (UHandle*)(&(buffer->analog)), ddb->info.valid_samples * ddb->info.enabled_channels);

	for (int i = 0; i < ddb->info.valid_samples * ddb->info.enabled_channels; i++)
	{
		(*(buffer->analog))->Numeric[i] = ddb->analog[i];
	}
	(*(buffer->analog))->dimSizes[0] = ddb->info.enabled_channels; // rows
	(*(buffer->analog))->dimSizes[1] = ddb->info.valid_samples; // columns

	// copy other data
	buffer->magic = ddb->magic;
	buffer->hits = ddb->hits;
	buffer->timecode = ddb->timecode;
	buffer->counter = ddb->counter;
	buffer->user = ddb->user;
	buffer->samples = ddb->info.samples;
	buffer->valid_samples = ddb->info.valid_samples;
	buffer->channels = ddb->info.channels;
	buffer->enabled_channels = ddb->info.enabled_channels;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ddb), handle);
	return res;
}

int LV_SCISDK_ReadFFT(char* Path, TD_FFT* buffer, void* handle)
{
	return 0;
}

int LV_SCISDK_ReadList(char* Path, TD_LIST* buffer, void* handle)
{
	return 0;
}

int LV_SCISDK_ReadOscilloscopeDual(char* Path, TD_OSCILLOSCOPE_DUAL* buffer, void* handle)
{
	SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER* ob;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ob), handle);
	if (res) return res;

	// read data
	res = SCISDK_ReadData(Path, ob, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ob), handle);
		return res;
	}

	// analog data
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->analog), ob->info.channels * ob->info.samples_analog * ob->info.tracks_analog_per_channel);

	for (int i = 0; i < ob->info.channels * ob->info.samples_analog * ob->info.tracks_analog_per_channel; i++)
	{
		(*(buffer->analog))->Numeric[i] = ob->analog[i];
	}

	(*(buffer->analog))->dimSizes[0] = ob->info.channels * ob->info.tracks_analog_per_channel;// rows
	(*(buffer->analog))->dimSizes[1] = ob->info.samples_analog;// columns

	// digital data
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->digital_data), ob->info.channels * ob->info.samples_digital * ob->info.tracks_digital_per_channel);
	for (int i = 0; i < ob->info.channels * ob->info.samples_digital * ob->info.tracks_digital_per_channel; i++)
	{
		(*(buffer->digital_data))->Numeric[i] = ob->digital[i];
	}
	(*(buffer->digital_data))->dimSizes[0] = ob->info.channels * ob->info.tracks_digital_per_channel;// rows
	(*(buffer->digital_data))->dimSizes[1] = ob->info.samples_digital;// columns

	// copy other informations
	buffer->magic = ob->magic;
	buffer->trigger_position = ob->trigger_position;
	buffer->timecode = ob->timecode;
	buffer->samples_analog = ob->info.samples_analog;
	buffer->samples_digital = ob->info.samples_digital;
	buffer->tracks_analog_per_channel = ob->info.tracks_analog_per_channel;
	buffer->tracks_digital_per_channel = ob->info.tracks_digital_per_channel;
	buffer->channels = ob->info.channels;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ob), handle);
	return res;
}

int LV_SCISDK_ReadRatemeter(char* Path, TD_RATEMETER* buffer, void* handle)
{
	SCISDK_RM_RAW_BUFFER* rb;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_RAW, (void**)&rb, handle);
	if (res) return res;

	// read data
	res = SCISDK_ReadData(Path, rb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_RAW, (void**)&rb, handle);
	}
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->data), rb->info.nchannels);

	for (int i = 0; i < rb->info.nchannels; i++)
	{
		(*(buffer->data))->Numeric[i] = (float)rb->data[i];
	}
	(*(buffer->data))->dimSize = rb->info.nchannels;

	// copy other informations
	buffer->magic = rb->magic;
	buffer->nchannels = rb->info.nchannels;
	buffer->buffer_size = rb->info.buffer_size;
	buffer->valid_data = rb->info.valid_data;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_RAW, (void**)&rb, handle);
	return res;
}

int LV_SCISDK_SetRegister(char* Path, uint32_t value, void* handle)
{
	return SCISDK_SetRegister(Path, value, handle);
}

int LV_SCISDK_GetRegister(char* Path, uint32_t* value, void* handle)
{
	return SCISDK_GetRegister(Path, value, handle);
}
