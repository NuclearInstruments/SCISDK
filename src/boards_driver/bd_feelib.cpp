#include "bd_feelib.h"
#include <functional>
#include <chrono>
#include <thread>

#define SCOPE_DATA_FORMAT " \
	[ \
		{ \"name\" : \"TIMESTAMP\", \"type\" : \"U64\" }, \
		{ \"name\" : \"TRIGGER_ID\", \"type\" : \"U32\" }, \
		{ \"name\" : \"WAVEFORM\", \"type\" : \"U16\", \"dim\" : 2 }, \
		{ \"name\" : \"WAVEFORM_SIZE\", \"type\" : \"SIZE_T\", \"dim\" : 1 }, \
		{ \"name\" : \"EVENT_SIZE\", \"type\" : \"SIZE_T\" } \
	] \
"
static unsigned long long value_to_ull(const char* value);

/*
BOARD DEVICE DRIVER FOR FEE-LIB COMPATIBLE DEVICES


Framework Compatible version: 1.0

*/

bd_feelib::bd_feelib(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path) {
    RegisterParameter("boardapi/felib/**", "router all board api to string access", SciSDK_Paramcb::Type::str, this);
	const std::list<std::string> listOfDataFormat = { "scope" };
	RegisterParameter("boardapi/readout.datatype", "select data format", SciSDK_Paramcb::Type::str, listOfDataFormat, this);

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
		}	else {
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

}

NI_RESULT bd_feelib::IGetParamString(string name, string* value)
{
	if (name == "readout.datatype") {
		switch (_datatype) {
			case FEELIB_DATATYPE::SCOPE:
				*value = "scope";
				return NI_OK;
			default:
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
			cout << "boardapi: get string " << name << endl;
			return  _hal->GetBoardParamater("/" + board_path, *value);
		}
	}

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
			if (ret) return ret;
			uint64_t ep_folder_handle;
			ret = _hal->FELib_GetParentHandle(ep_handle, NULL, &ep_folder_handle);
			if (ret) return ret;
			ret = _hal->FELib_SetValue(handle, "/endpoint/par/activeendpoint", "scope");
			if (ret) return ret;
			ret = _hal->FELib_SetReadDataFormat(ep_handle, SCOPE_DATA_FORMAT);
			if (ret) return ret;
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
	if (ret) {
		return NI_INVALID_PARAMETER_PATH;
	}
	if (rootpath == "felib") {
		int ret = 0;
		uint64_t handle;
		uint64_t ep_handle;
		if (_hal->FELib_GetConnectionHandle(&handle) != NI_OK) return -1;
		ret = _hal->FELib_SendCommand(handle, ("/" + board_path).c_str());
		return ret;
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
		if (ret != 0) return ret;
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
		if (ret) return ret;
		int ret = _hal->FELib_ReadData(ep_handle, acquisition_timeout_ms,
			&p->timestamp,
			&p->trigger_id,
			p->waveform,
			p->n_samples,
			&p->event_size
		);
		return ret;
	}
	break;

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
