#include "SciSDK_Labview_DLL.h"

#include <stdio.h>
#include <string>
#include "../../src/SciSDK_DLL.h"
#include "extcode.h"

#include "../../src/json.hpp"
//
//
using json = nlohmann::json;

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
	if (res)return res;
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

	buffer->magic = obSpectrum->magic;
	buffer->timecode = obSpectrum->timecode;
	buffer->inttime = obSpectrum->inttime;
	buffer->buffer_size = obSpectrum->info.buffer_size;
	buffer->total_bins = obSpectrum->info.total_bins;
	buffer->valid_bins = obSpectrum->info.valid_bins;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, handle);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadTOFSpectrum(char* Path, TD_SPECTRUM* buffer, void* handle) {
	SCISDK_SPECTRUM_DECODED_BUFFER* obSpectrum;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, handle);
	if (res) return res;

	// read data
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

	buffer->magic = obSpectrum->magic;
	buffer->timecode = obSpectrum->timecode;
	buffer->inttime = obSpectrum->inttime;
	buffer->buffer_size = obSpectrum->info.buffer_size;
	buffer->total_bins = obSpectrum->info.total_bins;
	buffer->valid_bins = obSpectrum->info.valid_bins;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, handle);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadDigitizer(char* Path, TD_DIGITIZER* buffer, void* handle)
{
	SCISDK_DIGITIZER_DECODED_BUFFER* ddb;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)(&ddb), handle);
	if (res) return res;

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

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFFT(char* Path, TD_FFT* buffer, void* handle)
{
	SCISDK_FFT_DECODED_BUFFER* fb;
	int res = SCISDK_AllocateBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fb, handle);
	if (res) return res;

	// read data
	res = SCISDK_ReadData(Path, fb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fb, handle);
		return res;
	}

	// magnitude
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->mag), fb->info.channels * fb->info.samples);
	for (int i = 0; i < fb->info.channels * fb->info.samples; i++)
	{
		(*(buffer->mag))->Numeric[i] = fb->mag[i];
	}
	(*(buffer->mag))->dimSize = fb->info.channels * fb->info.samples;

	// phase
	NumericArrayResize(iQ, 1, (UHandle*)&(buffer->ph), fb->info.channels * fb->info.samples);
	for (int i = 0; i < fb->info.channels * fb->info.samples; i++)
	{
		(*(buffer->ph))->Numeric[i] = fb->ph[i];
	}
	(*(buffer->ph))->dimSize = fb->info.channels * fb->info.samples;

	buffer->magic = fb->magic;
	buffer->channels = fb->info.channels;
	buffer->samples = fb->info.samples;
	buffer->timecode = fb->timecode;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fb, handle);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadList(char* Path, TD_LIST* buffer, void* handle, int buffer_size)
{
	SCISDK_LIST_RAW_BUFFER* lrb;
	int res = SCISDK_AllocateBufferSize(Path, T_BUFFER_TYPE_RAW, (void**)&lrb, handle, buffer_size);
	if (res) return res;

	res = SCISDK_ReadData(Path, lrb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_RAW, (void**)&lrb, handle);
		return res;
	}

	NumericArrayResize(iB, 1, (UHandle*)&(buffer->data), lrb->info.valid_samples);
	for (int i = 0; i < lrb->info.valid_samples; i++)
	{
		(*(buffer->data))->Numeric[i] = lrb->data[i];
	}
	(*(buffer->data))->dimSize = lrb->info.valid_samples;

	buffer->magic = lrb->magic;
	buffer->samples = lrb->info.samples;
	buffer->valid_samples = lrb->info.valid_samples;
	buffer->buffer_size = lrb->info.buffer_size;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_RAW, (void**)&lrb, handle);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscopeDual(char* Path, TD_OSCILLOSCOPE_DUAL* buffer, void* handle)
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

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadRatemeter(char* Path, TD_RATEMETER* buffer, void* handle)
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
		(*(buffer->data))->Numeric[i] = rb->data[i];
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

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadCustomPacketMultiple(char* Path, TD_CUSTOMPACKETMULTIPLE* buffer, void* handle, int buffer_size)
{
	SCISDK_CP_DECODED_BUFFER* lrb;
	int res = SCISDK_AllocateBufferSize(Path, T_BUFFER_TYPE_DECODED, (void**)&lrb, handle, buffer_size);
	if (res) return res;

	res = SCISDK_ReadData(Path, lrb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&lrb, handle);
		return res;
	}

	if (lrb->info.valid_data) {
		NumericArrayResize(uL, 1, (UHandle*)&(buffer->data), lrb->info.valid_data * lrb->data[0].n);
		int pos = 0;
		for (int i = 0; i < lrb->info.valid_data; i++)
		{
			for (int j = 0; j < lrb->data[i].n; j++)
			{
				(*(buffer->data))->Numeric[pos] = lrb->data[i].row[j];
				pos++;
			}
		}
		(*(buffer->data))->dimSizes[0] = lrb->info.valid_data; // rows
		(*(buffer->data))->dimSizes[1] = lrb->data[0].n; // columns
	}

	buffer->magic = lrb->magic;
	buffer->buffer_size = lrb->info.buffer_size;
	buffer->packet_size = lrb->info.packet_size;
	buffer->valid_data = lrb->info.valid_data;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&lrb, handle);
	return res;
}

int LV_SCISDK_ReadCustomPacketSingle(char* Path, TD_CUSTOMPACKETSINGLE* buffer, void* handle)
{
	SCISDK_CP_DECODED_BUFFER* lrb;
	int res = SCISDK_AllocateBufferSize(Path, T_BUFFER_TYPE_DECODED, (void**)&lrb, handle, 1);
	if (res) return res;

	res = SCISDK_ReadData(Path, lrb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&lrb, handle);
		return res;
	}

	if (lrb->info.valid_data) {
		NumericArrayResize(uL, 1, (UHandle*)&(buffer->data), lrb->data[0].n);
		for (int j = 0; j < lrb->data[0].n; j++)
		{
			(*(buffer->data))->Numeric[j] = lrb->data[0].row[j];
		}
		(*(buffer->data))->dimSize = lrb->data[0].n;
	}

	buffer->magic = lrb->magic;
	buffer->buffer_size = lrb->info.buffer_size;
	buffer->packet_size = lrb->info.packet_size;
	buffer->valid_data = lrb->info.valid_data;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&lrb, handle);
	return res;
}

int LV_SCISDK_ReadFrameSingle(char* Path, TD_FRAME_SINGLE* buffer, void* handle)
{
	SCISDK_FRAME_DECODED_BUFFER* fdb;
	int res = SCISDK_AllocateBufferSize(Path, T_BUFFER_TYPE_DECODED, (void**)&fdb, handle, 1);
	if (res) return res;

	// read data
	res = SCISDK_ReadData(Path, (void*)fdb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fdb, handle);
		return res;
	}

	if (fdb->info.valid_data) {
		NumericArrayResize(uL, 1, (UHandle*)&(buffer->pixel), fdb->data[0].n);
		for (int i = 0; i < fdb->data[0].n; i++)
		{
			(*(buffer->pixel))->Numeric[i] = fdb->data[0].pixel[i];
		}
		(*(buffer->pixel))->dimSize = fdb->data[0].n;
		buffer->magic = fdb->magic;
		buffer->buffer_size = fdb->info.buffer_size;
		buffer->event_count = fdb->data[0].info.event_count;
		buffer->hits = fdb->data[0].info.hits;
		buffer->n = fdb->data[0].n;
		buffer->timestamp = fdb->data[0].info.timestamp;
		buffer->trigger_count = fdb->data[0].info.trigger_count;
	}

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fdb, handle);
	return res;
}

int LV_SCISDK_ReadFrameMultiple(char* Path, TD_FRAME_MULTIPLE* buffer, void* handle, int buffer_size)
{
	SCISDK_FRAME_DECODED_BUFFER* fdb;
	int res = SCISDK_AllocateBufferSize(Path, T_BUFFER_TYPE_DECODED, (void**)&fdb, handle, buffer_size);
	if (res) return res;

	// read data
	res = SCISDK_ReadData(Path, (void*)fdb, handle);
	if (res) {
		SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fdb, handle);
		return res;
	}

	NumericArrayResize(uQ, 1, (UHandle*)&(buffer->data), fdb->info.valid_data * sizeof(FRAME_PACKET) / 8);
	for (int i = 0; i < fdb->info.valid_data;i++) {

		NumericArrayResize(uL, 1, (UHandle*)&((*buffer->data)->packets[i].pixel), fdb->data[i].n);
		for (int j = 0; j < fdb->data[i].n; j++)
		{
			(*(*buffer->data)->packets[i].pixel)->Numeric[j] = fdb->data[i].pixel[j];
		}
		(*(*buffer->data)->packets[i].pixel)->dimSize = fdb->data[i].n;

		// other info
		(*(buffer->data))->packets[i].n = fdb->data[i].n;
		(*(buffer->data))->packets[i].timestamp = fdb->data[i].info.timestamp;
		(*(buffer->data))->packets[i].trigger_count = fdb->data[i].info.trigger_count;
		(*(buffer->data))->packets[i].event_count = fdb->data[i].info.event_count;
		(*(buffer->data))->packets[i].hits = fdb->data[i].info.hits;
	}
	(*buffer->data)->dimSize = fdb->info.valid_data;
	buffer->magic = fdb->magic;
	buffer->valid_data = fdb->info.valid_data;
	buffer->buffer_size = fdb->info.buffer_size;

	SCISDK_FreeBuffer(Path, T_BUFFER_TYPE_DECODED, (void**)&fdb, handle);
	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetRegister(char* Path, uint32_t value, void* handle)
{
	return SCISDK_SetRegister(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetRegister(char* Path, uint32_t* value, void* handle)
{
	return SCISDK_GetRegister(Path, value, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ExecuteCommand(char* Path, char* parameter, void* handle)
{
	return SCISDK_ExecuteCommand(Path, parameter, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscopeStatus(char* Path, TD_OSCILLOSCOPE_STATUS* buffer, void* handle)
{
	return SCISDK_ReadStatus(Path, buffer, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadSpectrumStatus(char* Path, TD_SPECTRUM_STATUS* buffer, void* handle)
{
	return SCISDK_ReadStatus(Path, buffer, handle);
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFFTStatus(char* Path, TD_FFT_STATUS* buffer, void* handle)
{
	return SCISDK_ReadStatus(Path, buffer, handle);
}

int LV_SCISDK_GetComponentList(char* name, char* type, TD_COMPONENTARRAY ret, void* handle)
{
	char* ret_json_string = (char*)"";
	int res = SCISDK_GetComponentList(name, type, &ret_json_string, true, handle);
	if (res) return res;

	if (ret_json_string == "")return -1;
	json ret_json = json::parse(ret_json_string);

	NumericArrayResize(uQ, 1, (UHandle*)&(ret), 3 * ret_json.size());
	for (int i = 0; i < ret_json.size();i++) {
		char* path_str = (char*)((std::string)ret_json.at(0)["path"]).c_str();
		char* name_str = (char*)((std::string)ret_json.at(0)["name"]).c_str();
		char* type_str = (char*)((std::string)ret_json.at(0)["type"]).c_str();
		
		ret.array[i].path = path_str;
		ret.array[i].name = name_str;
		ret.array[i].type = type_str;
	}
	ret.dimSize = ret_json.size();
	return res;
}

int LV_SCISDK_GetAllParameters(char* path, char* ret, void* handle)
{
	char* ret_json_string = (char*)"";
	int res = SCISDK_GetAllParameters(path, &ret_json_string, handle);
	if (res) return res;

	return res;
}

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterDescription(char* path, char* ret, void* handle)
{
	char* value_ptr = (char*)"";
	int res = SCISDK_GetParameterString(path, &value_ptr, handle);
	if (res)return res;
	strcpy_s(ret, strlen(value_ptr) + 1, value_ptr);
	SCISDK_free_string(value_ptr);
	return res;
}

int LV_SCISDK_GetParameterListOfValues(char* path, char* ret, void* handle)
{
	char* ret_json_string = (char*)"";
	int res = SCISDK_GetParameterListOfValues(path, &ret_json_string, handle);
	if (res) return res;

	return res;
}

int LV_SCISDK_GetParameterMinimumValue(char* path, double* ret, void* handle)
{
	return SCISDK_GetParameterMinimumValue(path, ret, handle);
}

int LV_SCISDK_GetParameterMaximumValue(char* path, double* ret, void* handle)
{
	return SCISDK_GetParameterMaximumValue(path, ret, handle);
}

int LV_SCISDK_GetParametersProperties(char* path, char* ret, void* handle)
{
	char* ret_json_string = (char*)"";
	int res = SCISDK_GetParametersProperties(path, &ret_json_string, handle);
	if (res) return res;

	return res;
}
