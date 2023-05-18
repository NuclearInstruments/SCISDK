#include "scisdk_TOFspectrum.h"
#include <functional>
#include <chrono>
#include <thread>

/*
		DEVICE DRIVER FOR TOF SPECTRUM

		IP Compatible version: 1.0

*/

SciSDK_TOFSpectrum::SciSDK_TOFSpectrum(SciSDK_HAL *hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address.has_timestamp = false;
	address.base = (uint32_t)j.at("Address");
	for (auto& r : j.at("Registers")) {
		if ((string)r.at("Name") == "CONFIG") address.cfg = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_BINWIDTH") address.binwidth  = (uint32_t)r.at("Address");
		if ((string)r.at("Name") == "CONFIG_START_DELAY") address.startdelay = (uint32_t)r.at("Address");
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
	binwidth = 10;

	RegisterParameter("binwidth", "set bin width in clock cycles. 3 is minimum", SciSDK_Paramcb::Type::U32, 3,100000000, this);
	RegisterParameter("start_delay", "set low cut energy value on data input(before rebinning)", SciSDK_Paramcb::Type::U32, 0,2000000000, this);

	RegisterParameter("buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);

}

NI_RESULT SciSDK_TOFSpectrum::ISetParamU32(string name, uint32_t value) {

	if (name == "binwidth") {
		if ((value < 3) || (value > 100000000))
			return NI_PARAMETER_OUT_OF_RANGE;
		binwidth = value;
		return ConfigureSpectrum();
	}
	else if (name == "start_delay") {
		if ((value < 0) || (value >= 2000000000))
			return NI_PARAMETER_OUT_OF_RANGE;
		startdelay = value;
		return ConfigureSpectrum();
	}


	return NI_INVALID_PARAMETER;
}


NI_RESULT SciSDK_TOFSpectrum::ISetParamI32(string name, int32_t value) {

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_TOFSpectrum::ISetParamString(string name, string value) {
	

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_TOFSpectrum::IGetParamU32(string name, uint32_t *value) {

	if (name == "binwidth") {
		*value = binwidth;
		return NI_OK;
	}
	else if (name == "start_delay") {
		*value = startdelay;
		return NI_OK;
	}


	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_TOFSpectrum::IGetParamI32(string name, int32_t *value) {

	return NI_INVALID_PARAMETER;
}
NI_RESULT SciSDK_TOFSpectrum::IGetParamString(string name, string *value) {
	if (name == "buffer_type") {
		*value = "SCISDK_SPECTRUM_DECODED_BUFFER";
		return NI_OK;
	}

	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_TOFSpectrum::AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) {

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

NI_RESULT SciSDK_TOFSpectrum::FreeBuffer(T_BUFFER_TYPE bt, void **buffer) {

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

NI_RESULT SciSDK_TOFSpectrum::ReadData(void *buffer) {
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
	uint32_t buffer_size = settings.nbins * ceil((double)settings.bitbin / 32.0) ;
	if (_hal->ReadData(p->data, buffer_size, address.base, 5000, &dv)) return NI_ERROR_INTERFACE;

	if (dv < buffer_size) {
		return NI_INCOMPLETE_READ;
	}
	//decode data
	p->timecode = timestamp;
	p->inttime = inttime;
	p->info.valid_bins = settings.nbins ;

	return NI_OK;
}

NI_RESULT SciSDK_TOFSpectrum::ConfigureSpectrum() {
	int ret = 0;
	ret |= _hal->WriteReg(binwidth-3 , address.binwidth );
	ret |= _hal->WriteReg(startdelay, address.startdelay);
	
	if (ret)
		return NI_ERROR_INTERFACE;
	else {
		return NI_OK;
	}
}

NI_RESULT SciSDK_TOFSpectrum::CmdStart() {
	int ret = 0;
	current_cfg_reg = 1;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_TOFSpectrum::CmdStop() {
	int ret = 0;
	current_cfg_reg = 0;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

NI_RESULT SciSDK_TOFSpectrum::CmdReset() {
	int ret = 0;
	current_cfg_reg = 2;
	ret |= _hal->WriteReg(current_cfg_reg, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return NI_OK;
	}

}

NI_RESULT SciSDK_TOFSpectrum::CmdResetCounters() {
	int ret = 0;
	ret |= _hal->WriteReg(0, address.cfg);
	if (ret)
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}


NI_RESULT SciSDK_TOFSpectrum::ExecuteCommand(string cmd, string param) {
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

NI_RESULT SciSDK_TOFSpectrum::CheckSpectrum(bool *running, bool *completed, uint32_t *limitcnt,
	uint32_t *peak_max, uint32_t *total_counter, double *integration_time)
{
	uint32_t status;
	if (_hal->ReadReg(&status, address.status)) return NI_ERROR_INTERFACE;
	*running = (status & 0x1) ? true : false;
	*completed = 0;
	*limitcnt = 0;
	*peak_max = 0;
	*total_counter = 0;
	integration_time = 0;
	return NI_OK;
}


NI_RESULT SciSDK_TOFSpectrum::ReadStatus(void *buffer) {
	SCISDK_SPECTRUM_STATUS *p = (SCISDK_SPECTRUM_STATUS*)buffer;
	return CheckSpectrum(&p->running, &p->completed, &p->progress,
		&p->peak_max, &p->total_counter, &p->integration_time);
}

NI_RESULT SciSDK_TOFSpectrum::Detach()
{
	return CmdStop();
}
