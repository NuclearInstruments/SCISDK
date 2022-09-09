// SciSDK_DLL.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "SciSDK_DLL.h"

#include "scisdk_core.h"
#include <bitset>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>

SciSDK sdk;


SCISDK_DLL_API void * SCISDK_InitLib() {
	SciSDK * _sdk = new SciSDK;
	return (void *)_sdk;
}

SCISDK_DLL_API int SCISDK_FreeLib(void *handle) {
	if (handle == NULL) return NI_ERROR;
	SciSDK * _sdk = (SciSDK*)handle;
	delete _sdk;
}

SCISDK_DLL_API int SCISDK_AddNewDevice(char *DevicePath, char *DeviceModel, char *JSONFwFilePath, char *Name, void *handle) {

	if (handle == NULL) return NI_ERROR;
	if (DevicePath == NULL) return NI_ERROR;
	if (DeviceModel == NULL) return NI_ERROR;
	if (JSONFwFilePath == NULL)return NI_ERROR;
	if (Name == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _DevicePath(DevicePath);
	string _DeviceModel(DeviceModel);
	string _JSONFwFilePath(JSONFwFilePath);
	string _Name(Name);

	int res = _sdk->AddNewDevice(_DevicePath, _DeviceModel, _JSONFwFilePath, _Name);
	//_sdk->p_error(res);
	return res;
}

SCISDK_DLL_API int SCISDK_DetachDevice(char* name, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (name == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _name(name);
	int res = _sdk->DetachDevice(_name);
	return res;
}

SCISDK_DLL_API int SCISDK_SetParameterString(char* Path, char* value, void* handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	string _value(value);

	return _sdk->SetParameter(_Path, _value);
}

SCISDK_DLL_API int SCISDK_SetParameterInteger(char* Path, int value, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);

	return _sdk->SetParameter(_Path, value);
}

SCISDK_DLL_API int SCISDK_SetParameterDouble(char* Path, double value, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);

	return _sdk->SetParameter(_Path, value);
}

SCISDK_DLL_API int SCISDK_GetParameterString(char* Path, char** value, void* handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	string _value;

	int res = _sdk->GetParameter(_Path, &_value);
	char *v = (char*)_value.c_str();
	*value = (char*)malloc(strlen(v) * sizeof(char));
	strcpy(*value, v);
	return res;
}

SCISDK_DLL_API int SCISDK_GetParameterInteger(char* Path, int *value, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);

	int res = _sdk->GetParameter(_Path, value);
	return res;
}

SCISDK_DLL_API int SCISDK_GetParameterDouble(char* Path, double*value, void*handle) {
	if (handle == NULL)return NI_ERROR;
	SciSDK*_sdk = (SciSDK*)handle;
	string _Path(Path);

	int res = _sdk->GetParameter(_Path, value);
	return res;
}

SCISDK_DLL_API int SCISDK_DecodeData(char* Path, void*buffer_in, void*buffer_out, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (buffer_in == NULL)return NI_ERROR;
	if (buffer_out == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	_sdk->DecodeData(_Path, buffer_in, buffer_out);
}

SCISDK_DLL_API int SCISDK_AllocateBuffer(char* Path, int buffer_type, void **buffer, void *handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (buffer == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	T_BUFFER_TYPE _buffer_type;
	if (buffer_type == 0) {
		_buffer_type = T_BUFFER_TYPE_RAW;
	}
	else if (buffer_type == 1) {
		_buffer_type = T_BUFFER_TYPE_DECODED;
	}
	else {
		return NI_ERROR;
	}

	int res = _sdk->AllocateBuffer(_Path, _buffer_type, buffer);
	return res;
}

SCISDK_DLL_API int SCISDK_AllocateBufferSize(char * Path, int buffer_type, void ** buffer, void * handle, int size)
{
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (buffer == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	T_BUFFER_TYPE _buffer_type;
	if (buffer_type == 0) {
		_buffer_type = T_BUFFER_TYPE_RAW;
	}
	else if (buffer_type == 1) {
		_buffer_type = T_BUFFER_TYPE_DECODED;
	}
	else {
		return NI_ERROR;
	}

	int res = _sdk->AllocateBuffer(_Path, _buffer_type, buffer, size);
	return res;
}

SCISDK_DLL_API int SCISDK_SetRegister(char* Path, int value, void* handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;
	if (value < 0)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	uint32_t _value = (uint32_t)value;

	int res = _sdk->SetRegister(_Path, _value);
	return res;
}

SCISDK_DLL_API int SCISDK_GetRegister(char* Path, int*value, void*handle) {
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	uint32_t _value;
	int res = _sdk->GetRegister(_Path, &_value);
	*value = (int)_value;
	return res;
}

SCISDK_DLL_API int SCISDK_FreeBuffer(char* Path, int buffer_type, void **buffer, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (buffer_type == NULL)return NI_ERROR;
	if (buffer == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	T_BUFFER_TYPE _buffer_type;
	if (buffer_type == 0) {
		_buffer_type = T_BUFFER_TYPE_RAW;
	}
	else if (buffer_type == 1) {
		_buffer_type = T_BUFFER_TYPE_DECODED;
	}
	else {
		return NI_ERROR;
	}

	int res = _sdk->FreeBuffer(_Path, _buffer_type, buffer);
	return res;
}

SCISDK_DLL_API int SCISDK_ReadData(char *Path, void *buffer, void*handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (buffer == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string _Path(Path);
	int res = _sdk->ReadData(_Path, buffer);
	_sdk->p_error(res);
	return res;
}

SCISDK_DLL_API int SCISDK_s_error(int err_no, char** value, void* handle) {
	if (handle == NULL)return NI_ERROR;
	if (err_no == NULL)return NI_ERROR;

	SciSDK * _sdk = (SciSDK*)handle;
	string ret_string;

	ret_string = _sdk->s_error(err_no);
	char* str_tmp = (char*)ret_string.c_str();
	*value = (char*)malloc(strlen(str_tmp) * sizeof(char));

	strcpy(*value, str_tmp);
	return NI_OK;
}

SCISDK_DLL_API int SCISDK_ExecuteCommand(char* Path, char* value, void* handle) {
	if (handle == NULL)return NI_ERROR;
	if (Path == NULL)return NI_ERROR;
	if (value == NULL)return NI_ERROR;

	string _Path(Path);
	string _value(value);
	SciSDK* _sdk = (SciSDK*)handle;
	int res = _sdk->ExecuteCommand(_Path, _value);
	return res;
}

SCISDK_DLL_API int SCISDK_GetComponentList(char* name, char* Type, char** ret, bool return_json, void* handle)
{
	if (handle == NULL)return NI_ERROR;
	if (name == NULL)return NI_ERROR;
	if (Type == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;

	string _name(name);
	string _type(Type);
	string res_string;
	SciSDK* _sdk = (SciSDK*)handle;

	int res = _sdk->GetComponentList(_name, _type, &res_string, (bool)return_json);
	char* str_tmp = (char*)res_string.c_str();
	*ret = (char*)malloc(strlen(str_tmp) * sizeof(char));
	strcpy(*ret, str_tmp);

	return 0;
}

SCISDK_DLL_API int SCISDK_GetAllParameters(char * Path, char ** ret, void * handle)
{
	if (Path == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK* _handle = (SciSDK*)handle;
	string _path(Path);
	string _ret;

	*ret = "";
	int res = _handle->GetAllParameters(_path, &_ret);
	char* str_tmp = (char*)_ret.c_str();
	*ret = (char*)malloc(strlen(str_tmp) * sizeof(char));
	strcpy(*ret, str_tmp);
	return res;
}

SCISDK_DLL_API int SCISDK_GetParameterDescription(char * Path, char ** ret, void * handle)
{
	if (Path == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK* _handle = (SciSDK*)handle;
	string _path(Path);
	string _ret;
	*ret = "";

	int res = _handle->GetParameterDescription(_path, &_ret);
	char* str_tmp = (char*)_ret.c_str();
	*ret = (char*)malloc(strlen(str_tmp) * sizeof(char));
	strcpy(*ret, str_tmp);
	return res;
}

SCISDK_DLL_API int SCISDK_GetParameterListOfValues(char * Path, char ** ret, void * handle)
{
	if (Path == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK* _handle = (SciSDK*)handle;
	string _path(Path);
	string _ret;
	*ret = "";

	int res = _handle->GetParameterListOfValues(_path, &_ret);
	char* str_tmp = (char*)_ret.c_str();
	*ret = (char*)malloc(strlen(str_tmp) * sizeof(char));
	strcpy(*ret, str_tmp);
	return res;
}

SCISDK_DLL_API int SCISDK_GetParameterMinimumValue(char * Path, double * ret, void * handle)
{
	if (Path == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK* _handle = (SciSDK*)handle;
	string _path(Path);
	return _handle->GetParameterMinimumValue(_path, ret);
}

SCISDK_DLL_API int SCISDK_GetParameterMaximumValue(char * Path, double * ret, void * handle)
{
	if (Path == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK* _handle = (SciSDK*)handle;
	string _path(Path);
	return _handle->GetParameterMaximumValue(_path, ret);
}

SCISDK_DLL_API int SCISDK_GetParametersProperties(char * Path, char ** ret, void * handle)
{
	if (Path == NULL)return NI_ERROR;
	if (ret == NULL)return NI_ERROR;
	if (handle == NULL)return NI_ERROR;

	SciSDK* _handle = (SciSDK*)handle;
	string _path(Path);
	string _ret;
	int res = _handle->GetParametersProperties(_path, &_ret);

	char* str_tmp = (char*)_ret.c_str();
	*ret = (char*)malloc(sizeof(char) * strlen(str_tmp));
	strcpy(*ret, str_tmp);
	return res;
}
