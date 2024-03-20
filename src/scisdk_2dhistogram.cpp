#include "scisdk_2dhistogram.h"
#include <functional>
#include <chrono>
#include <thread>

/*
		DEVICE DRIVER FOR OSCILLOSCPE

		IP Compatible version: 1.4

*/

SciSDK_2DHistogram::SciSDK_2DHistogram(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address.has_timestamp = false;
	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "CONFIG") address.cfg = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_LIMIT") address.cfg_limit = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "STATUS") address.status = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "READ_TIMESTAMP") {
			address.has_timestamp = true;
			address.timestamp = (uint32_t)r.at("Address");
		}
		if ((string)r.at("Name") == "STATUS_TIME") {
			address.has_statustime = true;
			address.status_time = (uint32_t)r.at("Address");
		}
	}


	settings.nbinsX = (uint32_t)j.at("BinsX");
	settings.nbinsY = (uint32_t)j.at("BinsY");
	settings.bitbin = (uint32_t)j.at("CountsBit");

	spectrum_limit = LIMIT_TYPE::FREE_RUNNING;

	RegisterParameter("limit", "set target limit for histogram integration", SciSDK_Paramcb::Type::U32, this);


	const std::list<std::string> listOfLimit = { "freerun", "time_ms","total_count","peak_count" };
	RegisterParameter("limitmode", "set limit mode for the integration on the histogram", SciSDK_Paramcb::Type::str, listOfLimit, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	RegisterParameter("binsX", "maximum number of bins in the histogram", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("binsY", "maximum number of bins in the histogram", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("max_counts", "maximum number of counts per bin", SciSDK_Paramcb::Type::U32, this);
}

NI_RESULT SciSDK_2DHistogram::ISetParamU32(string name, uint32_t value) {

	if (name == "limit") {
		limit_value = value;
		return ConfigureSpectrum();
	}


	return NI_INVALID_PARAMETER;
}


NI_RESULT SciSDK_2DHistogram::ISetParamI32(string name, int32_t value) {

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_2DHistogram::ISetParamString(string name, string value) {
	if (name == "limitmode") {
		if (value == "freerun") {
			spectrum_limit = LIMIT_TYPE::FREE_RUNNING;
			return ConfigureSpectrum();
		}
		else if (value == "time_ms") {
			spectrum_limit = LIMIT_TYPE::TIME;
			return ConfigureSpectrum();
		}
		else if (value == "total_count") {
			spectrum_limit = LIMIT_TYPE::TOTAL_CNT;
			return ConfigureSpectrum();
		}
		else if (value == "peak_count") {
			spectrum_limit = LIMIT_TYPE::PEAK_CNT;
			return ConfigureSpectrum();
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_2DHistogram::IGetParamU32(string name, uint32_t *value) {
	
	if (name == "limit") {
		*value = limit_value;
		return NI_OK;
	} 
	else if (name == "binsX") {
		*value = settings.nbinsX;
		return NI_OK;
	}
	else if (name == "binsY") {
		*value = settings.nbinsY;
		return NI_OK;
	}
	else if (name == "max_counts") {
		*value = 1<<settings.bitbin;
		return NI_OK;
	}


	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_2DHistogram::IGetParamI32(string name, int32_t *value) {

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_2DHistogram::IGetParamString(string name, string *value) {
	if (name == "limitmode") {
		if (spectrum_limit == LIMIT_TYPE::FREE_RUNNING) {
			*value = "freerun";
			return NI_OK;
		}
		else if (spectrum_limit == LIMIT_TYPE::TIME) {
			*value = "time_ms";
			return NI_OK;
		}
		else if (spectrum_limit == LIMIT_TYPE::TOTAL_CNT) {
			*value = "total_count";
			return NI_OK;
		}
		else if (spectrum_limit == LIMIT_TYPE::PEAK_CNT) {
			*value = "peak_count";
			return NI_OK;
		}
		else return NI_PARAMETER_OUT_OF_RANGE;
	}
	else if (name == "buffer_type") {
		*value = "SCISDK_2DHISTOGRAM_DECODED_BUFFER";
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_2DHistogram::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

	*buffer = (SCISDK_2DHISTOGRAM_DECODED_BUFFER *)malloc(sizeof(SCISDK_2DHISTOGRAM_DECODED_BUFFER));
	if (*buffer == NULL) {
		return NI_ALLOC_FAILED;
	}
	SCISDK_2DHISTOGRAM_DECODED_BUFFER*p;
	p = (SCISDK_2DHISTOGRAM_DECODED_BUFFER*)*buffer;
	p->data = (uint32_t*)malloc(8 + settings.nbinsX *settings.nbinsY * ceil((double)settings.bitbin / 32.0) * sizeof(uint32_t));

	if (p->data == NULL) {
		return NI_ALLOC_FAILED;
	}
	p->magic = BUFFER_TYPE_2DHISTOGRAM_DECODED;
	p->timecode = 0;
	p->inttime = 0;
	p->info.buffer_size = settings.nbinsX * settings.nbinsX * ceil((double)settings.bitbin / 32.0);
	p->info.total_bins = settings.nbinsX * settings.nbinsY ;
	p->info.binsX = settings.nbinsX;
	p->info.binsY = settings.nbinsY;
	p->info.valid_bins = 0;

	return NI_OK;

}

NI_RESULT SciSDK_2DHistogram::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {

	if (*buffer == NULL) {
		return NI_MEMORY_NOT_ALLOCATED;
	}
	SCISDK_2DHISTOGRAM_DECODED_BUFFER *p;
	p = (SCISDK_2DHISTOGRAM_DECODED_BUFFER*)*buffer;
	if (p->data != NULL) {
		free(p->data);
		p->data = NULL;
	}

	p->magic = BUFFER_TYPE_INVALID;
	p->timecode = 0;
	p->inttime = 0;
	p->info.buffer_size = 0;
	p->info.total_bins = 0;
	p->info.valid_bins = 0;
	p->info.binsX = 0;
	p->info.binsY = 0;
	return NI_OK;

}

NI_RESULT SciSDK_2DHistogram::ReadData(void *buffer) {
	uint32_t dv;
	uint32_t rtimestamp[2];
	uint32_t inttime;
	uint64_t timestamp;
	bool ready = false;
	bool armed = false;
	bool running = false;
	if (buffer == NULL) {
		return NI_INVALID_BUFFER;
	}

	if (address.has_timestamp) {
		if (_hal->ReadData(rtimestamp, 2, address.timestamp, 100, &dv)) return NI_ERROR_INTERFACE;
		timestamp = (uint64_t)rtimestamp[0] + (((uint64_t)rtimestamp[1]) << 32UL);
	}
	else {
		std::chrono::time_point<std::chrono::steady_clock> ts = std::chrono::steady_clock::now();
		timestamp = std::chrono::duration_cast<std::chrono::microseconds>(ts.time_since_epoch()).count();
	}

	if (address.has_statustime) {
		if (_hal->ReadReg(&inttime, address.status_time)) return NI_ERROR_INTERFACE;
	}



	SCISDK_2DHISTOGRAM_DECODED_BUFFER *p;
	p = (SCISDK_2DHISTOGRAM_DECODED_BUFFER*)buffer;
	//check user buffer
	if (p->magic != BUFFER_TYPE_2DHISTOGRAM_DECODED) return NI_INVALID_BUFFER_TYPE;
	if (p->info.total_bins != settings.nbinsX*settings.nbinsY) return NI_INCOMPATIBLE_BUFFER;



	//download data
	uint32_t buffer_size = settings.nbinsX* settings.nbinsY * ceil((double)settings.bitbin / 32.0);
	if (_hal->ReadData(p->data, buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;

	if (dv < buffer_size) {
		return NI_INCOMPLETE_READ;
	}
	//decode data
	p->timecode = timestamp;
	p->inttime = inttime;
	p->info.valid_bins = settings.nbinsX*settings.nbinsY;

	return NI_OK;
}

NI_RESULT SciSDK_2DHistogram::ConfigureSpectrum() {
	int ret = 0;

	int32_t limit_reg = 0;
	switch (spectrum_limit) {
	case LIMIT_TYPE::FREE_RUNNING:
		limit_reg = 0;
		break;
	case LIMIT_TYPE::TIME:
		limit_reg = (2 << 29);
		break;
	case LIMIT_TYPE::TOTAL_CNT:
		limit_reg = (1 << 29);
		break;
	case LIMIT_TYPE::PEAK_CNT:
		limit_reg = (3 << 29);
		break;

	}

	ret |= _hal->WriteReg(limit_reg, address.cfg_limit);

	if (ret)
		return NI_ERROR_INTERFACE;
	else {
		return NI_OK;
	}
}

NI_RESULT SciSDK_2DHistogram::CmdStart() {
	int ret = 0;
	current_cfg_reg = 1 << 2;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_2DHistogram::CmdStop() {
	int ret = 0;
	current_cfg_reg = 0;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_2DHistogram::CmdReset() {
	int ret = 0;
	current_cfg_reg = 0;
	ret |= _hal->WriteReg(1, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return NI_OK;
	}

}

NI_RESULT SciSDK_2DHistogram::CmdResetCounters() {
	int ret = 0;
	ret |= _hal->WriteReg(0, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}


NI_RESULT SciSDK_2DHistogram::ExecuteCommand(string cmd, string param) {
	if (cmd == "start") {
		return CmdStart();
	}
	else if (cmd == "stop") {
		return CmdStop();
	}
	else if (cmd == "reset") {
		return CmdReset();
	}
	else if (cmd == "reset_counters") {
		return CmdResetCounters();
	}

	return NI_INVALID_COMMAND;
}

NI_RESULT SciSDK_2DHistogram::CheckSpectrum(bool *running, bool *completed, uint32_t *limitcnt,
	uint32_t *peak_max, uint32_t *total_counter, double *integration_time)
{
	uint32_t status;
	if (_hal->ReadReg(&status, address.status)) return NI_ERROR_INTERFACE;
	*running = (status & 0x1) ? true : false;
	*completed = ((status >> 1) & 0x1) ? true : false;
	*limitcnt = (status >> 4);
	*peak_max = 0;
	*total_counter = 0;
	integration_time = 0;
	return NI_OK;
}


NI_RESULT SciSDK_2DHistogram::ReadStatus(void *buffer) {
	SCISDK_SPECTRUM_STATUS *p = (SCISDK_SPECTRUM_STATUS*)buffer;
	return CheckSpectrum(&p->running, &p->completed, &p->progress,
		&p->peak_max, &p->total_counter, &p->integration_time);
}

NI_RESULT SciSDK_2DHistogram::Detach()
{
	return CmdStop();
}
