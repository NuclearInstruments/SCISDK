#include "scisdk_rate_meter.h"

SciSDK_Ratemeter::SciSDK_Ratemeter(SciSDK_HAL * hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address = (uint32_t)j.at("Address");
	settings.nchannels = (uint32_t)j.at("Channels");
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

}

NI_RESULT SciSDK_Ratemeter::ISetParamU32(string name, uint32_t value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Ratemeter::ISetParamI32(string name, int32_t value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Ratemeter::ISetParamString(string name, string value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Ratemeter::IGetParamU32(string name, uint32_t * value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Ratemeter::IGetParamI32(string name, int32_t * value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Ratemeter::IGetParamString(string name, string * value)
{
	if (name == "buffer_type") {
		*value = "SCISDK_RM_RAW_BUFFER";
		return NI_OK;
	}		
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Ratemeter::AllocateBuffer(T_BUFFER_TYPE bt, void ** buffer)
{
	if (bt == T_BUFFER_TYPE_RAW) {
		*buffer = (SCISDK_RM_RAW_BUFFER *)malloc(sizeof(SCISDK_RM_RAW_BUFFER));
		if (*buffer == NULL) {
			return NI_ALLOC_FAILED;
		}

		SCISDK_RM_RAW_BUFFER *p;
		p = (SCISDK_RM_RAW_BUFFER*)*buffer;
		p->data = (double*)malloc(sizeof(double) * (settings.nchannels ));
		if (p->data == NULL) {
			return NI_ALLOC_FAILED;
		}
		p->magic = BUFFER_TYPE_LIST_RAW;
		p->info.nchannels = settings.nchannels;
		p->info.buffer_size = settings.nchannels;
		p->info.valid_data = settings.nchannels;
		return NI_OK;
	}
	else {
		return NI_INCOMPATIBLE_BUFFER;
	}
}

NI_RESULT SciSDK_Ratemeter::AllocateBuffer(T_BUFFER_TYPE bt, void ** buffer, int size)
{
	return NI_PARAMETER_OUT_OF_RANGE;
}

NI_RESULT SciSDK_Ratemeter::FreeBuffer(T_BUFFER_TYPE bt, void ** buffer)
{
	if (bt == T_BUFFER_TYPE_RAW) {
		if (*buffer == NULL) {
			return NI_MEMORY_NOT_ALLOCATED;
		}
		SCISDK_RM_RAW_BUFFER *p;
		p = (SCISDK_RM_RAW_BUFFER*)*buffer;
		if (p->data != NULL) {
			free(p->data);
			p->data = NULL;
		}

		p->magic = BUFFER_TYPE_INVALID;
		p->info.buffer_size = 0;
		p->info.nchannels = 0;
		return NI_OK;
	}
	else {
		return NI_PARAMETER_OUT_OF_RANGE;
	}
}

NI_RESULT SciSDK_Ratemeter::ReadData(void * buffer)
{

	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}

	SCISDK_RM_RAW_BUFFER *p;
	p = (SCISDK_RM_RAW_BUFFER *)buffer;
	if (p->magic != BUFFER_TYPE_LIST_RAW) return NI_INVALID_BUFFER_TYPE;

	uint32_t *data;
	data = (uint32_t *)malloc(p->info.nchannels *sizeof(uint32_t) + 8);

	uint32_t vd;

	if (_hal->ReadData(data, p->info.nchannels, address, 100, &vd)) return NI_ERROR_INTERFACE;

	//if (vd < p->info.nchannels) return NI_ERROR_INTERFACE;

	for (int i = 0; i < p->info.nchannels; i++) {
		p->data[i] = (double)data[i];
	}


	free(data);
	return NI_OK;
}

NI_RESULT SciSDK_Ratemeter::ExecuteCommand(string cmd, string param)
{
	return NI_INVALID_COMMAND;
}

NI_RESULT SciSDK_Ratemeter::ReadStatus(void * buffer)
{
	return NI_OK;
}
