#include "scisdk_spectrum.h"
#include <functional>
#include <chrono>
#include <thread>

/*
		DEVICE DRIVER FOR OSCILLOSCPE

		IP Compatible version: 1.4

*/

SciSDK_Spectrum::SciSDK_Spectrum(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address.has_timestamp = false;
	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "CONFIG") address.cfg = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_LIMIT") address.cfg_limit = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_REBIN") address.cfg_rebin = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_MIN") address.cfg_min = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_MAX") address.cfg_max = (uint32_t)r.at("Address");
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


	settings.nbins = (uint32_t)j.at("bins");
	settings.bitbin = (uint32_t)j.at("CountsBit");
	emin = 0;
	emax = settings.nbins - 1;
	spectrum_limit = LIMIT_TYPE::FREE_RUNNING;
	rebin =0;

	RegisterParameter("rebin", "set rebin factor. 0: no rebin", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("min", "set low cut energy value on data input(before rebinning)", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("max", "set low high energy value on data input(before rebinning)", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("limit", "set target limit for spectrum integration", SciSDK_Paramcb::Type::U32, this);


	const std::list<std::string> listOfLimit = { "freerun", "time_ms","total_count","peak_count" };
	RegisterParameter("limitmode", "set limit mode for the integration on the spectrum", SciSDK_Paramcb::Type::str, listOfLimit, this);
	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

	RegisterParameter("bins", "maximum number of bins in the spectrum", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("max_counts", "maximum number of counts per bin", SciSDK_Paramcb::Type::U32, this);
}

NI_RESULT SciSDK_Spectrum::ISetParamU32(string name, uint32_t value) {

	if (name == "rebin") {
		if ((value < 0) || (value > 7))
			return NI_PARAMETER_OUT_OF_RANGE;
		rebin = value;
		return ConfigureSpectrum();
	}
	else if (name == "min") {
		if ((value < 0) || (value >= settings.nbins))
			return NI_PARAMETER_OUT_OF_RANGE;
		emin = value;
		return ConfigureSpectrum();
	}
	else if (name == "max") {
		if ((value < 0) || (value >= settings.nbins))
			return NI_PARAMETER_OUT_OF_RANGE;
		emax = value;
		return ConfigureSpectrum();
	}
	else if (name == "limit") {
		limit_value = value;
		return ConfigureSpectrum();
	}


	return NI_INVALID_PARAMETER;
}


NI_RESULT SciSDK_Spectrum::ISetParamI32(string name, int32_t value) {

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Spectrum::ISetParamString(string name, string value) {
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

NI_RESULT SciSDK_Spectrum::IGetParamU32(string name, uint32_t *value) {

	if (name == "rebin") {
		*value = rebin;
		return NI_OK;
	}
	else if (name == "min") {
		*value = emin;
		return NI_OK;
	}
	else if (name == "max") {
		*value = emax;
		return NI_OK;
	}
	else if (name == "limit") {
		*value = limit_value;
		return NI_OK;
	} 
	else if (name == "bins") {
		*value = settings.nbins;
		return NI_OK;
	}
	else if (name == "max_counts") {
		*value = 1<<settings.bitbin;
		return NI_OK;
	}


	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Spectrum::IGetParamI32(string name, int32_t *value) {

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_Spectrum::IGetParamString(string name, string *value) {
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
		*value = "SCISDK_SPECTRUM_DECODED_BUFFER";
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_Spectrum::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

	*buffer = (SCISDK_SPECTRUM_DECODED_BUFFER *)malloc(sizeof(SCISDK_SPECTRUM_DECODED_BUFFER));
	if (*buffer == NULL) {
		return NI_ALLOC_FAILED;
	}
	SCISDK_SPECTRUM_DECODED_BUFFER *p;
	p = (SCISDK_SPECTRUM_DECODED_BUFFER*)*buffer;
	p->data = (uint32_t*)malloc(8 + settings.nbins * ceil((double)settings.bitbin / 32.0) * sizeof(uint32_t));

	if (p->data == NULL) {
		return NI_ALLOC_FAILED;
	}
	p->magic = BUFFER_TYPE_SPECTRUM_DECODED;
	p->timecode = 0;
	p->inttime = 0;
	p->info.buffer_size = settings.nbins * ceil((double)settings.bitbin / 32.0);
	p->info.total_bins = settings.nbins;
	p->info.valid_bins = 0;

	return NI_OK;

}

NI_RESULT SciSDK_Spectrum::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {

	if (*buffer == NULL) {
		return NI_MEMORY_NOT_ALLOCATED;
	}
	SCISDK_SPECTRUM_DECODED_BUFFER *p;
	p = (SCISDK_SPECTRUM_DECODED_BUFFER*)*buffer;
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
	return NI_OK;

}

NI_RESULT SciSDK_Spectrum::ReadData(void *buffer) {
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



	SCISDK_SPECTRUM_DECODED_BUFFER *p;
	p = (SCISDK_SPECTRUM_DECODED_BUFFER*)buffer;
	//check user buffer
	if (p->magic != BUFFER_TYPE_SPECTRUM_DECODED) return NI_INVALID_BUFFER_TYPE;
	if (p->info.total_bins != settings.nbins) return NI_INCOMPATIBLE_BUFFER;



	//download data
	uint32_t buffer_size = settings.nbins * ceil((double)settings.bitbin / 32.0) / (rebin + 1);
	if (_hal->ReadData(p->data, buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;

	if (dv < buffer_size) {
		return NI_INCOMPLETE_READ;
	}
	//decode data
	p->timecode = timestamp;
	p->inttime = inttime;
	p->info.valid_bins = settings.nbins / (rebin + 1);

	return NI_OK;
}

NI_RESULT SciSDK_Spectrum::ConfigureSpectrum() {
	int ret = 0;
	ret |= _hal->WriteReg(rebin, address.cfg_rebin);
	ret |= _hal->WriteReg(emin, address.cfg_min);
	ret |= _hal->WriteReg(emax, address.cfg_max);
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

NI_RESULT SciSDK_Spectrum::CmdStart() {
	int ret = 0;
	current_cfg_reg = 1 << 2;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_Spectrum::CmdStop() {
	int ret = 0;
	current_cfg_reg = 0;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_Spectrum::CmdReset() {
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

NI_RESULT SciSDK_Spectrum::CmdResetCounters() {
	int ret = 0;
	ret |= _hal->WriteReg(0, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}


NI_RESULT SciSDK_Spectrum::ExecuteCommand(string cmd, string param) {
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

NI_RESULT SciSDK_Spectrum::CheckSpectrum(bool *running, bool *completed, uint32_t *limitcnt,
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


NI_RESULT SciSDK_Spectrum::ReadStatus(void *buffer) {
	SCISDK_SPECTRUM_STATUS *p = (SCISDK_SPECTRUM_STATUS*)buffer;
	return CheckSpectrum(&p->running, &p->completed, &p->progress,
		&p->peak_max, &p->total_counter, &p->integration_time);
}

NI_RESULT SciSDK_Spectrum::Detach()
{
	return CmdStop();
}
