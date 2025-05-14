#include "bd_feelib.h"
#include <functional>
#include <chrono>
#include <thread>

#define DPP_MAX_USER_INFO 4
#define DPP_MAX_NWAVE 4
#define DPP_MAX_SAMPLE_WAVE 4096
#define SCOPE_DATA_FORMAT " \
	[ \
		{ \"name\" : \"TIMESTAMP\", \"type\" : \"U64\" }, \
		{ \"name\" : \"TRIGGER_ID\", \"type\" : \"U32\" }, \
		{ \"name\" : \"WAVEFORM\", \"type\" : \"U16\", \"dim\" : 2 }, \
		{ \"name\" : \"WAVEFORM_SIZE\", \"type\" : \"SIZE_T\", \"dim\" : 1 }, \
		{ \"name\" : \"EVENT_SIZE\", \"type\" : \"SIZE_T\" } \
	] \
"

#define OPENDPP_DATA_FORMAT " \
	[ \
		{ \"name\" : \"CHANNEL\", \"type\" : \"U8\" }, \
		{ \"name\" : \"TIMESTAMP\", \"type\" : \"U64\" }, \
		{ \"name\" : \"TIMESTAMP_NS\", \"type\" : \"U64\" }, \
		{ \"name\" : \"FINE_TIMESTAMP\", \"type\" : \"U16\" }, \
		{ \"name\" : \"ENERGY\", \"type\" : \"U16\" }, \
		{ \"name\" : \"FLAGS_B\", \"type\" : \"U16\" }, \
		{ \"name\" : \"FLAGS_A\", \"type\" : \"U8\" }, \
		{ \"name\" : \"PSD\", \"type\" : \"U16\" }, \
		{ \"name\" : \"USER_INFO\", \"type\" : \"U64\", \"dim\" : 1 }, \
		{ \"name\" : \"USER_INFO_SIZE\", \"type\" : \"SIZE_T\" }, \
		{ \"name\" : \"TRUNCATED\", \"type\" : \"BOOL\" }, \
		{ \"name\" : \"WAVEFORM\", \"type\" : \"U16\", \"dim\" : 1 }, \
		{ \"name\" : \"WAVEFORM_SIZE\", \"type\" : \"SIZE_T\" }, \
		{ \"name\" : \"BOARD_FAIL\", \"type\" : \"BOOL\" }, \
		{ \"name\" : \"SPECIAL_EVENT\", \"type\" : \"BOOL\" }, \
		{ \"name\" : \"EVENT_SIZE\", \"type\" : \"SIZE_T\" }, \
		{ \"name\" : \"FLUSH\", \"type\" : \"BOOL\" }, \
		{ \"name\" : \"AGGREGATE_COUNTER\", \"type\" : \"U32\" } \
	] \
"
static unsigned long long value_to_ull(const char* value);

/*
BOARD DEVICE DRIVER FOR FEE-LIB COMPATIBLE DEVICES


Framework Compatible version: 1.0

*/


typedef enum {
    CAEN_FELib_Success                      = 0,    //!< Operation completed successfully  --> NI_OK
    CAEN_FELib_GenericError                 = -1,   //!< Unspecified error --> NI_ERROR_GENERIC
    CAEN_FELib_InvalidParam                 = -2,   //!< Invalid parameter --> NI_INVALID_PARAMETER
    CAEN_FELib_DeviceAlreadyOpen            = -3,   //!< Returned by CAEN_FELib_Open() in case of device already opened --> NI_ALREADY_CONNECTED
    CAEN_FELib_DeviceNotFound               = -4,   //!< Returned by CAEN_FELib_Open() in case of device not found --> NI_NOTCONNECTED
    CAEN_FELib_MaxDevicesError              = -5,   //!< Returned by CAEN_FELib_Open() in case of maximum number of devices exceeded --> NI_TOO_MANY_DEVICES_CONNECTED
    CAEN_FELib_CommandError                 = -6,   //!< Command error --> NI_INVALID_COMMAND
    CAEN_FELib_InternalError                = -7,   //!< Internal error --> NI_FEELIB_INTERNAL_ERROR
    CAEN_FELib_NotImplemented               = -8,   //!< Not yet implemented function --> NI_NOT_SUPPORTED
    CAEN_FELib_InvalidHandle                = -9,   //!< Invalid handle --> NI_INVALID_HANDLE
    CAEN_FELib_DeviceLibraryNotAvailable    = -10,  //!< Returned by CAEN_FELib_Open() in case of invalid prefix --> NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY
    CAEN_FELib_Timeout                      = -11,  //!< Returned by CAEN_FELib_ReadData() and CAEN_FELib_HasData() in case of timeout -->NI_TIMEOUT
    CAEN_FELib_Stop                         = -12,  //!< Returned by CAEN_FELib_ReadData() and CAEN_FELib_HasData() once after the last event of a run --> NI_LAST
    CAEN_FELib_Disabled                     = -13,  //!< Disabled function --> NI_DISABLED
    CAEN_FELib_BadLibraryVersion            = -14,  //!< Returned by CAEN_FELib_Open() in case of library version not supported by the server --> NI_INVALID_LIBRARY_VERSION
    CAEN_FELib_CommunicationError           = -15,  //!< Communication error --> NI_ERROR_INTERFACE
} CAEN_FELib_ErrorCode;




static NI_RESULT CAEN_TO_SCISDK_ErrorTranslate(int caen_error){ 
    switch(caen_error) {
        case 0:   return NI_OK;
        case -1:  return NI_ERROR_GENERIC;
        case -2:  return NI_INVALID_PARAMETER;
        case -3:  return NI_ALREADY_CONNECTED;
        case -4:  return NI_NOTCONNECTED;
        case -5:  return NI_TOO_MANY_DEVICES_CONNECTED;
        case -6:  return NI_INVALID_COMMAND;
        case -7:  return NI_FEELIB_INTERNAL_ERROR;
        case -8:  return NI_NOT_SUPPORTED;
        case -9:  return NI_INVALID_HANDLE;
        case -10: return NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY;
        case -11: return NI_TIMEOUT;
        case -12: return NI_LAST;
        case -13: return NI_DISABLED;
        case -14: return NI_INVALID_LIBRARY_VERSION;
        case -15: return NI_ERROR_INTERFACE;
        default:  return NI_ERROR_GENERIC; // fallback in case of unknown error code
    }
}

bd_feelib::bd_feelib(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {
    RegisterParameter("boardapi/felib/**", "router all board api to string access", SciSDK_Paramcb::Type::str, this);
	const std::list<std::string> listOfDataFormat = { "scope", "dpp"};
	RegisterParameter("boardapi/readout.datatype", "select data format", SciSDK_Paramcb::Type::str, listOfDataFormat, this);
	RegisterParameter("boardapi/buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	ConfigureEndpoint();
}


NI_RESULT bd_feelib::ISetParamI32(string name, uint32_t value)
{
	
    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_feelib::IGetParamI32(string name, uint32_t *value)
{

    return NI_INVALID_PARAMETER;
}


NI_RESULT bd_feelib::ISetParamString(string name, string value)
{
	if (name == "readout.datatype") {
		if (value == "scope") {
			_datatype = FEELIB_DATATYPE::SCOPE;
			ConfigureEndpoint();
			return NI_OK;
		}	else if (value == "dpp") {
			_datatype = FEELIB_DATATYPE::DPP;
			ConfigureEndpoint();
			return NI_OK;
		}
		else {
			return NI_INVALID_PARAMETER;
		}
	}
	else {
		string rootpath;
		string board_path;
		int ret = ExtractRootPath(name, rootpath, board_path);
		if (ret) {
			return NI_INVALID_PARAMETER_PATH;
		}
		if (rootpath == "felib") {
			cout << "boardapi: set string " << name << " to " << value << endl;
			return _hal->SetBoardParamater("/" + board_path, value);
		}
	}
	return NI_THIS_CODE_SHOULDNOT_BE_EXECUTED;
}

NI_RESULT bd_feelib::IGetParamString(string name, string* value)
{
	if (name == "readout.datatype") {
		switch (_datatype) {
			case FEELIB_DATATYPE::SCOPE:
				*value = "scope";
				return NI_OK;
			case FEELIB_DATATYPE::DPP:
				*value = "dpp";
				return NI_OK;

			default:
				return NI_INVALID_PARAMETER;
		}
	} else if (name == "buffer_type") {
		if (_datatype == FEELIB_DATATYPE::SCOPE) {
			*value = "SCISDK_FE_SCOPE_EVENT";
			return NI_OK;
		}
		else if (_datatype == FEELIB_DATATYPE::DPP) {
			*value = "SCISDK_FE_OPENDPP_EVENT";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else {
		string rootpath;
		string board_path;
		int ret = ExtractRootPath(name, rootpath, board_path);
		if (ret) {
			return NI_INVALID_PARAMETER_PATH;
		}
		if (rootpath == "felib") {
			cout << "boardapi: get string " << name << endl;
			return  _hal->GetBoardParamater("/" + board_path, *value);
		}
	}
	return NI_THIS_CODE_SHOULDNOT_BE_EXECUTED;
}

int bd_feelib::ExtractRootPath(string fullpath, string &rootpath, string &board_path) {
	//extract the part of path before the first / and the rest from fullpath
	size_t pos = fullpath.find("/");
	if (pos != string::npos) {
		rootpath = fullpath.substr(0, pos);
		board_path = fullpath.substr(pos + 1);
		return 0;
	}
	else {
		rootpath = fullpath;
		board_path = "";
		return -1;
	}
}

int bd_feelib::ConfigureEndpoint() {

	switch (_datatype) {
		case  FEELIB_DATATYPE::SCOPE:
		{
			int ret = 0;
			uint64_t handle;
			uint64_t ep_handle;
			if (_hal->FELib_GetConnectionHandle(&handle) != NI_OK) return -1;
			ret = _hal->FELib_GetHandle(handle, "/endpoint/scope", &ep_handle);
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			uint64_t ep_folder_handle;
			ret = _hal->FELib_GetParentHandle(ep_handle, NULL, &ep_folder_handle);
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			ret = _hal->FELib_SetValue(handle, "/endpoint/par/activeendpoint", "scope");
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			ret = _hal->FELib_SetReadDataFormat(ep_handle, SCOPE_DATA_FORMAT);
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			return NI_OK;
		}
		break;
		case  FEELIB_DATATYPE::DPP:
		{
			int ret = 0;
			uint64_t handle;
			uint64_t ep_handle;
			if (_hal->FELib_GetConnectionHandle(&handle) != NI_OK) return -1;
			ret = _hal->FELib_GetHandle(handle, "/endpoint/opendpp", &ep_handle);
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			uint64_t ep_folder_handle;
			ret = _hal->FELib_GetParentHandle(ep_handle, NULL, &ep_folder_handle);
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			ret = _hal->FELib_SetValue(handle, "/endpoint/par/activeendpoint", "opendpp");
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			ret = _hal->FELib_SetReadDataFormat(ep_handle, OPENDPP_DATA_FORMAT);
			if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
			return NI_OK;
		}
		break;
	}
	return NI_INVALID_ACQ_MODE;
}



NI_RESULT bd_feelib::ExecuteCommand(string cmd, string param) {
	string rootpath;
	string board_path;
	int ret = ExtractRootPath(cmd, rootpath, board_path);
	std::cout << cmd << "::" << rootpath << "::" << rootpath << endl;
	if (ret) {
		return NI_INVALID_PARAMETER_PATH;
	}
	if (rootpath == "felib") {
		int ret = 0;
		uint64_t handle;
		if (_hal->FELib_GetConnectionHandle(&handle) != NI_OK) return -1;
		ret = _hal->FELib_SendCommand(handle, ("/" + board_path).c_str());
		return CAEN_TO_SCISDK_ErrorTranslate(ret);
	}


	return NI_INVALID_COMMAND;
}

NI_RESULT bd_feelib::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size) {
	int ret;
	char value[256];
	uint64_t handle;
	if (_hal->FELib_GetConnectionHandle(&handle) != NI_OK) return -1;

	switch (_datatype) {
	case  FEELIB_DATATYPE::SCOPE:
	{
		ret = _hal->FELib_GetValue(handle, "/par/NumCh", value);
		if (ret != 0) return CAEN_TO_SCISDK_ErrorTranslate(ret);
		int n_channels = (unsigned)value_to_ull(value);
		SCISDK_FE_SCOPE_EVENT* evt;
		*buffer = (SCISDK_FE_SCOPE_EVENT*)malloc(sizeof(*evt));
		evt = (SCISDK_FE_SCOPE_EVENT*)*buffer;

		if (evt == NULL) {
			return NI_ALLOC_FAILED;
		}
		evt->n_channels = n_channels;
		evt->n_samples = (size_t*)malloc(evt->n_channels * sizeof(*evt->n_samples));
		if (evt->n_samples == NULL) {
			return NI_ALLOC_FAILED;
		}
		evt->n_allocated_samples = (size_t*)malloc(evt->n_channels * sizeof(*evt->n_allocated_samples));
		if (evt->n_allocated_samples == NULL) {
			return NI_ALLOC_FAILED;
		}
		evt->waveform = (uint16_t**)malloc(evt->n_channels * sizeof(*evt->waveform));
		if (evt->waveform == NULL) {
			return NI_ALLOC_FAILED;
		}
		for (unsigned i = 0; i < evt->n_channels; ++i) {
			evt->n_allocated_samples[i] = size;
			evt->waveform[i] = (uint16_t*)malloc(evt->n_allocated_samples[i] * sizeof(*evt->waveform[i]));
			if (evt->waveform[i] == NULL) {
				return NI_ALLOC_FAILED;
			}
		}
		buffer = (void**)evt;
		return NI_OK;
	}
	break;

	case  FEELIB_DATATYPE::DPP:
	{
		AllocateBuffer(bt, buffer);
	}
	
	default:
		return NI_INVALID_ACQ_MODE;
	}
}

NI_RESULT bd_feelib::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer) {
	switch (_datatype) {

	case  FEELIB_DATATYPE::DPP:
	{
		//allocate SCISDK_FE_OPENDPP_EVENT buffer
		SCISDK_FE_OPENDPP_EVENT* evt;
		*buffer = (SCISDK_FE_OPENDPP_EVENT*)malloc(sizeof(*evt));
		evt = (SCISDK_FE_OPENDPP_EVENT*)*buffer;
		if (evt == NULL) {
			return NI_ALLOC_FAILED;
		}

		evt->user_info = (uint64_t*)malloc(sizeof(uint64_t)* DPP_MAX_USER_INFO);
		if (evt->user_info == NULL) {
			return NI_ALLOC_FAILED;
		}
		evt->waveform = (uint16_t*)malloc(sizeof(uint16_t) * DPP_MAX_NWAVE* DPP_MAX_SAMPLE_WAVE);

		if (evt->waveform == NULL) {
			return NI_ALLOC_FAILED;
		}
		
		buffer = (void**)evt;
		return NI_OK;
	}
	break;
	default:
		return NI_INVALID_ACQ_MODE;
	}
	
}

NI_RESULT bd_feelib::ReadData(void* buffer) {
	int ret = 0;
	uint64_t handle;
	
	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}
	
	if (_hal->FELib_GetConnectionHandle(&handle) != NI_OK) return -1;
	
	switch (_datatype) {
	case  FEELIB_DATATYPE::SCOPE:
	{

		SCISDK_FE_SCOPE_EVENT* p;
		p = (SCISDK_FE_SCOPE_EVENT*)buffer;

		uint64_t ep_handle;

		ret = _hal->FELib_GetHandle(handle, "/endpoint/scope", &ep_handle);
		if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
		int ret = _hal->FELib_ReadData(ep_handle, acquisition_timeout_ms,
			&p->timestamp,
			&p->trigger_id,
			p->waveform,
			p->n_samples,
			&p->event_size
		);
		return CAEN_TO_SCISDK_ErrorTranslate(ret);
	}
	break;

	case  FEELIB_DATATYPE::DPP:
	{
		SCISDK_FE_OPENDPP_EVENT* p;
		p = (SCISDK_FE_OPENDPP_EVENT*)buffer;

		uint64_t ep_handle;

		ret = _hal->FELib_GetHandle(handle, "/endpoint/opendpp", &ep_handle);
		if (ret) return CAEN_TO_SCISDK_ErrorTranslate(ret);
		int ret = _hal->FELib_ReadData(ep_handle, acquisition_timeout_ms,
			&p->channel,
			&p->timestamp,
			&p->timestamp_ns,
			&p->fine_timestamp,
			&p->energy,
			&p->flags_b,
			&p->flags_a,
			&p->psd,
			p->user_info,
			&p->user_info_size,
			&p->truncated,
			p->waveform,
			&p->waveform_size,
			&p->board_fail,
			&p->special_event,
			&p->event_size,
			&p->flush,
			&p->aggregate_counter
		);
		return CAEN_TO_SCISDK_ErrorTranslate(ret);
	}

	default:
		return NI_INVALID_ACQ_MODE;
	}
}


static unsigned long long value_to_ull(const char* value) {
	char* value_end;
	const unsigned long long ret = strtoull(value, &value_end, 0);
	if (value == value_end || errno == ERANGE)
		fprintf(stderr, "strtoull error\n");
	return ret;
}
