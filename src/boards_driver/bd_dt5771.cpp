#include "bd_dt5771.h"
#include <functional>
#include <chrono>
#include <thread>


#define DT5771_IR_IMP_SET		10
#define DT5771_IR_RANGE_SET		11
//#define DT5771_IR_BW_SET		41
#define DT5771_IR_COUPLING_SET	9
#define DT5771_IR_GAIN_SET		7
#define DT5771_IR_DTERM_SET		40
#define DT5771_IR_OFFSET_SET	8
/*
BOARD DEVICE DRIVER FOR DT5771


Framework Compatible version: 1.0

*/
bd_dt5771::bd_dt5771(SciSDK_HAL* hal, json j, string path) : SciSDK_Node(hal, j, path) {
	
	dgtz_timeout = 1000;
	
	for (int i = 0; i < 1; i++) {
		RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".offset", "set analog offset", SciSDK_Paramcb::Type::d, this);
		RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".gain", "set analog gain", SciSDK_Paramcb::Type::I32, this);
		RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".range", "set analog range", SciSDK_Paramcb::Type::str, listOfRange, this);
		RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".impedance", "set analog input impedance", SciSDK_Paramcb::Type::str, listOfImpedance, this);
		//RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".bw", "set analog input bandwith", SciSDK_Paramcb::Type::str, listOfBw, this);
		RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".shaper", "set analog shaper", SciSDK_Paramcb::Type::str, listOfCoupling, this);
	}

	RegisterParameter("boardapi/digitalio/LEMO0.impedance", "set digital lemo input impedance", SciSDK_Paramcb::Type::str, listOfDigitalImpedance, this);
	RegisterParameter("boardapi/digitalio/LEMO1.impedance", "set digital lemo input impedance", SciSDK_Paramcb::Type::str, listOfDigitalImpedance, this);
	RegisterParameter("boardapi/digitizer/timeout", "digitizer timeout in ms", SciSDK_Paramcb::Type::d, this);
	RegisterParameter("boardapi/buffer_type", "return the buffer type to be allocated for the current configuration", SciSDK_Paramcb::Type::str, this);
}


NI_RESULT bd_dt5771::ISetParamDouble(string name, double value)
{
	for (int i = 0; i < 1; i++) {
		if (name == "analog/CH" + std::to_string(i) + ".offset") {
			analog_settings[i].offset_mV = value;
			int ret = _hal->ConfigurationRegisterSet(0x6496 + (int)analog_settings[i].offset_mV, DT5771_IR_OFFSET_SET, i);
			if (ret) {
				return NI_ERROR_INTERFACE;
			}
			return NI_OK;
		}
	}

	if (name == "boardapi/digitizer/timeout") {
		dgtz_timeout = value;
		return NI_OK;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt5771::IGetParamDouble(string name, double* value)
{
	for (int i = 0; i < 1; i++) {
		if (name == "analog/CH" + std::to_string(i) + ".offset") {
			*value = analog_settings[i].offset_mV;
			return NI_OK;
		}
	}
	if (name == "boardapi/digitizer/timeout") {
		*value = dgtz_timeout;
		return NI_OK;
	}
	return NI_INVALID_PARAMETER;
}



NI_RESULT bd_dt5771::ISetParamI32(string name, int32_t value)
{
	for (int i = 0; i < 1; i++) {
		if (name == "analog/CH" + std::to_string(i) + ".gain") {
			analog_settings[i].gain = value;
			int ret = _hal->ConfigurationRegisterSet((int)analog_settings[i].gain, DT5771_IR_GAIN_SET, i);
			if (ret) {
				return NI_ERROR_INTERFACE;
			}

			return NI_OK;
		}
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt5771::IGetParamI32(string name, int32_t* value)
{
	for (int i = 0; i < 1; i++) {
		if (name == "analog/CH" + std::to_string(i) + ".gain") {
			*value = analog_settings[i].gain;
			return NI_OK;
		}
	}
	return NI_INVALID_PARAMETER;
}




NI_RESULT bd_dt5771::ISetParamString(string name, string value)
{
	std::transform(value.begin(), value.end(), value.begin(), ::tolower);

	for (int i = 0; i < 1; i++) {
		if (name == "analog/CH" + std::to_string(i)  + ".impedance") {
			if (value == "50r") {
				analog_settings[i].r50 = true;
			}
			else {
				analog_settings[i].r50 = false;
			}
			int ret = _hal->ConfigurationRegisterSet((int)analog_settings[i].r50?1:0, DT5771_IR_IMP_SET, i);
			if (ret) {
				return NI_ERROR_INTERFACE;
			}

			return NI_OK;
		}
		if (name == "analog/CH" + std::to_string(i) + ".range") {
			if (value == "10v") {
				analog_settings[i].div = true;
			}
			else {
				analog_settings[i].div = false;
			}
			int ret = _hal->ConfigurationRegisterSet((int)analog_settings[i].div ? 1 : 0, DT5771_IR_RANGE_SET, i);
			if (ret) {
				return NI_ERROR_INTERFACE;
			}

			return NI_OK;
		}
		//if (name == "analog/CH" + std::to_string(i) + ".bw") {
		//	int index = findIndexInList(listOfBw, value);
		//	if (index == -1) {
		//		return NI_PARAMETER_OUT_OF_RANGE;
		//	}
		//	analog_settings[i].bw = (BANDWIDTH) index;

		//	int ret = _hal->ConfigurationRegisterSet((int)analog_settings[i].bw, DT5771_IR_BW_SET, i);
		//	if (ret) {
		//		return NI_ERROR_INTERFACE;
		//	}
		//	return NI_OK;
		//}

		if (name == "analog/CH" + std::to_string(i) + ".shaper") {
			int index = findIndexInList(listOfCoupling, value);
			if (index == -1) {
				return NI_PARAMETER_OUT_OF_RANGE;
			}
			analog_settings[i].coupling = (COUPLING)index;

			int ret = _hal->ConfigurationRegisterSet((int)analog_settings[i].coupling, DT5771_IR_COUPLING_SET, i);
			if (ret) {
				return NI_ERROR_INTERFACE;
			}

			return NI_OK;
		}
	}

	for (int i = 0; i < 2; i++) {
		if (name == "digitalio/LEMO" + std::to_string(i) + ".impedance") {
			if (value == "50r") {
				dlemo[i].r50 = true;
			}
			else {
				dlemo[i].r50 = false;
			}

			int ret = _hal->ConfigurationRegisterSet((int)dlemo[i].r50 ? 1 : 0, DT5771_IR_DTERM_SET , i);
			if (ret) {
				return NI_ERROR_INTERFACE;
			}

			return NI_OK;
		}
	}

	return NI_THIS_CODE_SHOULDNOT_BE_EXECUTED;
}

NI_RESULT bd_dt5771::IGetParamString(string name, string* value) {
    for (int i = 0; i < 1; i++) {
        if (name == "analog/CH" + std::to_string(i) + ".impedance") {
            *value = analog_settings[i].r50 ? "50r" : "1k";
            return NI_OK;
        }
        if (name == "analog/CH" + std::to_string(i) + ".range") {
            *value = analog_settings[i].div ? "10v" : "2v";
            return NI_OK;
        }
		//if (name == "analog/CH" + std::to_string(i) + ".bw") {
		//	if ((int)analog_settings[i].bw >= 0 && (int)analog_settings[i].bw < listOfBw.size()) {
		//		auto it = std::next(listOfBw.begin(), (int)analog_settings[i].bw);
		//		*value = *it;
		//		return NI_OK;
		//	}
		//	else {
		//		return NI_PARAMETER_OUT_OF_RANGE;
		//	}
		//}
        if (name == "analog/CH" + std::to_string(i) + ".shaper") {
            if ((int)analog_settings[i].coupling < listOfCoupling.size()) {
                auto it = std::next(listOfCoupling.begin(), (int)analog_settings[i].coupling);
                *value = *it;
                return NI_OK;
            } else {
                return NI_PARAMETER_OUT_OF_RANGE;
            }
        }
    }

	for (int i = 0; i < 2; i++) {
		if (name == "digitalio/LEMO" + std::to_string(i) + ".impedance") {
			*value = dlemo[i].r50 ? "50r" : "hi"; 
			return NI_OK;
		}
	}

	if (name == "buffer_type") {
		*value = "SCISDK_FE_SCOPE_EVENT";
		return NI_OK;
	}
    return NI_ERROR; // If name does not match any parameter
}

NI_RESULT bd_dt5771::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size) {
	int ret;
	char value[256];
	uint64_t handle;

	
	SCISDK_FE_SCOPE_EVENT* evt;
	*buffer = (SCISDK_FE_SCOPE_EVENT*)malloc(sizeof(*evt));
	evt = (SCISDK_FE_SCOPE_EVENT*)*buffer;

	if (evt == NULL) {
		return NI_ALLOC_FAILED;
	}
	evt->n_channels = 1;
	evt->event_size = 0;
	evt->n_samples = NULL;
	evt->n_allocated_samples = NULL;
	evt->waveform = NULL;
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
		evt->waveform[i] = NULL;
		evt->n_samples[i] = 0;
		evt->n_allocated_samples[i] = 0;
	}
	for (unsigned i = 0; i < evt->n_channels; ++i) {
		evt->n_allocated_samples[i] = size;
		evt->waveform[i] = (uint16_t*)malloc((evt->n_allocated_samples[i]*2 +0x10000) * sizeof(*evt->waveform[i]));
		if (evt->waveform[i] == NULL) {
			return NI_ALLOC_FAILED;
		}
	}
	buffer = (void**)evt;
	return NI_OK;
	

}

NI_RESULT bd_dt5771::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer) {
	return NI_ALLOC_FAILED;


}

NI_RESULT bd_dt5771::FreeBuffer(T_BUFFER_TYPE bt, void** buffer) {
	SCISDK_FE_SCOPE_EVENT* evt;
	evt = (SCISDK_FE_SCOPE_EVENT*)*buffer;
	if (evt->waveform == NULL) {
		return NI_ALLOC_FAILED;
	}
	for (unsigned i = 0; i < evt->n_channels; ++i) {
		if (evt->waveform[i] != NULL) {
			free(evt->waveform[i]);
			evt->waveform[i] = NULL;
		}
	}
	free(evt->waveform);
	evt->waveform = NULL;
	if (evt->n_samples) free(evt->n_samples);
	evt->n_samples = NULL;
	if (evt->n_allocated_samples) free(evt->n_allocated_samples);
	evt->n_allocated_samples = NULL;
	evt->event_size = 0;
	return NI_OK;
}
NI_RESULT bd_dt5771::ReadData(void* buffer) {
	uint32_t read_data;
	SCISDK_FE_SCOPE_EVENT* p;
	p = (SCISDK_FE_SCOPE_EVENT*)buffer;
	uint32_t size = p->n_allocated_samples[0];
	size = size > 16 * 1024 * 1024 ? 16 * 1024 * 1024 : size;
	int r = _hal->ReadDataDMA((uint8_t*)p->waveform[0],
		size,
		0,
		(uint32_t)dgtz_timeout,
		&read_data
	);
	p->timestamp = 0;
	p->trigger_id = 0;
	if (r == 0) {
		if (read_data == 0) {
			p->n_samples[0] = 0;
			return NI_TIMEOUT;
		}
		else {
			p->n_samples[0] = read_data/2;
			return NI_OK;
		}
		
	}
	else {
		p->n_samples[0] = 0;
		return NI_ERROR_GENERIC;
	}
	

	return NI_OK;

}
int bd_dt5771::findIndexInList(const std::list<std::string>& lst, const std::string& value) {
	int index = 0;
	for (const auto& item : lst) {
		if (item == value) {
			return index;
		}
		++index;
	}
	return -1; // Return -1 if the value is not found
}