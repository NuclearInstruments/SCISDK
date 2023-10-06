#include "bd_dt4810.h"
#include <functional>
#include <chrono>
#include <thread>

/*
BOARD DEVICE DRIVER FOR DT1260


Framework Compatible version: 1.0

*/

bd_dt4810::bd_dt4810(SciSDK_HAL *hal, SciSDK_Device *dev, json j, string path) : SciSDK_Node(hal, j, path) {
    
    _dev = dev;
    board_params.offset.value = 2048;
    board_params.offset.address = 4294967289;
    //for (auto r : j["Registers"]) {
    //    cout << r["Name"] << endl;
    //}
     
    clock_fpga = j["clock"];
    clock_dac = j["dac"];
    
	cout << " DT4810 Fpga Clock: " << clock_fpga << endl << " DAC Clock: " << clock_dac << endl;
    

    cout << "Registering DT4810 endpoints" << endl;

    const std::list<std::string> listOfTrueFalse = { "false", "true" };

    RegisterParameter("timebase.rate", "set generation rate", SciSDK_Paramcb::Type::d, this);
    const std::list<std::string> listOfTimebaseMode = { "fixed", "random"};
    const std::list<std::string> listOfAmplitudeMode = { "fixed", "spectrum" };
    RegisterParameter("boardapi/timebase.mode", "set timebase mode", SciSDK_Paramcb::Type::str, listOfTimebaseMode, this);
    RegisterParameter("boardapi/timebase.parallizable", "set parallizable machine mode", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("timebase.seed1", "set timebase seed 1", SciSDK_Paramcb::Type::U64,  this);
    RegisterParameter("timebase.seed2", "set timebase seed 2", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("timebase.seed3", "set timebase seed 3", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("timebase.seed4", "set timebase seed 4", SciSDK_Paramcb::Type::U64, this);
    
    RegisterParameter("boardapi/amplitude.mode", "set amplitude mode", SciSDK_Paramcb::Type::str, listOfAmplitudeMode, this);
    RegisterParameter("amplitude.fixed", "set amplitude fixed value", SciSDK_Paramcb::Type::U32, this);
    RegisterParameter("amplitude.seed1", "set amplitude seed 1", SciSDK_Paramcb::Type::U64, this);

    RegisterParameter("shape.drc.decay", "set decay constant for DRC", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("shape.drc.risetime", "set risetime for DRC", SciSDK_Paramcb::Type::d, this);

    RegisterParameter("boardapi/noise.gauss.enable", "enable disable gaussian noise generator", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("noise.gauss.gain", "set magniture of gaussian noise", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("noise.gauss.seed1", "set timebase seed 1", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("noise.gauss.seed2", "set timebase seed 2", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("noise.gauss.seed3", "set timebase seed 3", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("noise.gauss.seed4", "set timebase seed 4", SciSDK_Paramcb::Type::U64, this);
    
    RegisterParameter("boardapi/main.invert", "invert signal polarity", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("main.gain", "global gain", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("main.offset", "global offseet", SciSDK_Paramcb::Type::I32, this);
    RegisterParameter("boardapi/main.output.enable", "enable output", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("boardapi/main.output.gate", "gate output", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);


}


NI_RESULT bd_dt4810::ISetParamU32(string name, uint32_t value)
{
    int ret = 0;
	if (name == "amplitude.fixed") {
        if ((value > 16384) || (value<0)) {
            return NI_PARAMETER_OUT_OF_RANGE;
        } else {
            ret = _dev->SetRegister("/Registers/A_EN_FIXED_ENERGY", value);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.energy.constant = value;
            return NI_OK;
        }
    }
    

    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt4810::IGetParamU32(string name, uint32_t *value)
{
	int ret = 0;
    if (name == "amplitude.fixed") {
        uint32_t fixed;
        _dev->GetRegister("/Registers/A_EN_FIXED_ENERGY", &fixed);
        if (ret) return NI_ERROR_INTERFACE;
        hw_config.energy.constant = fixed;
        *value = hw_config.energy.constant;
        return NI_OK;
    }

    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt4810::ISetParamDouble(string name, double value)
{
    int ret = 0;

    if (name == "timebase.rate") {
        if ((value > 10000000) || (value < 0)) {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
        else {
			uint32_t rate = clock_fpga / value;
            uint32_t prate = ((value * (1.0 / clock_fpga))) * pow(2.0, 32);
            ret |= _dev->SetRegister("/Registers/A_TB_RATE", rate);
            ret |= _dev->SetRegister("/Registers/A_TB_POISSON_P", prate);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.timebase.rate = value;
            return NI_OK;
        }
    }

    if (name == "shape.drc.decay") {
        
        double deltaT = 1 / clock_dac;
        double tau = value * 1e-6;
        double Mf1 = 1 / (exp(deltaT / tau));
        double Ma[8];
		uint32_t MaI[8];
        for (int i = 0; i < 7; i++) {
            float MX = Mf1;
            for (int j = 0; j < i; j++) {
                MX = MX * Mf1;
            }
            Ma[i] = MX;
            MaI[i] = Ma[i] * 0xFFFFFFFF;
            cout << "Ma[" << i << "] : " << std::hex << (uint64_t)(Ma[i] * (0xFFFFFFFF)) << "   ";
        }

		double ratio = clock_dac / clock_fpga;
        double Mf2 = 1 / (exp(ratio * deltaT / tau));
        double M = Mf2;
        uint32_t MI = M * 0xFFFF;
        ret = _dev->SetRegister("/Registers/A_SH_DRC_M1", MaI[0]);
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M2", MaI[1]);
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M3", MaI[2]);
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M4", MaI[3]);
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M5", MaI[4]);
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M6", MaI[5]);
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M7", MaI[6]);
        
        ret |= _dev->SetRegister("/Registers/A_SH_DRC_M", MI);

        if (ret) return NI_ERROR_INTERFACE;

        hw_config.shape.tau = value;
        return NI_OK;
        
        
    }
    
    if (name == "shape.drc.risetime") {
		
        if (value == 0) {
            ret = _dev->SetRegister("/Registers/A_SH_DRC_BYPASS_LOWPASS", 1);
            if (ret) return NI_ERROR_INTERFACE;
        }
        else {
            ret = _dev->SetRegister("/Registers/A_SH_DRC_BYPASS_LOWPASS", 0);
            if (ret) return NI_ERROR_INTERFACE;
        }

        hw_config.shape.rise = value;
        return NI_OK;
    }
    
    if (name == "noise.gauss.gain") {
		uint32_t g = value * 0xFFFF;
        ret = _dev->SetRegister("/Registers/A_NS_GAUSS_GAIN", g);
        if (ret) return NI_ERROR_INTERFACE;
		hw_config.noise.gaussian.gain = value;
        return NI_OK;
    }


    return NI_INVALID_PARAMETER;
    
    
}

NI_RESULT bd_dt4810::IGetParamDouble(string name, double* value)
{
    int ret = 0;
    
    if (name == "timebase.rate") {
        uint32_t rate;
        ret = _dev->GetRegister("/Registers/A_TB_RATE", &rate);
        if (ret) return NI_ERROR_INTERFACE;
        hw_config.timebase.rate = clock_fpga / rate;
        *value = hw_config.timebase.rate;  
        return NI_OK;
    }

    if (name == "shape.drc.decay") {
		*value = hw_config.shape.tau;
    }

    if (name == "shape.drc.risetime") {
		*value = hw_config.shape.rise;
    }

    if (name == "noise.gauss.gain") {
		*value = hw_config.noise.gaussian.gain;
    }
    return NI_INVALID_PARAMETER;

}

NI_RESULT bd_dt4810::ISetParamString(string name, string value) {
    int ret = 0;

    if (name == "timebase.mode") {
        if (value == "fixed") {
			ret = _dev->SetRegister("/Registers/A_TB_MODE", 0x0);
            hw_config.timebase.mode = hw_config.timebase.PERIODIC;
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "random") {
            ret = _dev->SetRegister("/Registers/A_TB_MODE", 0x1);
            hw_config.timebase.mode = hw_config.timebase.POISSON;
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
        
    }

    if (name == "amplitude.mode") {
        if (value == "fixed") {
            ret = _dev->SetRegister("/Registers/A_EN_MODE", 0x0);
			hw_config.energy.mode = hw_config.energy.FIXED;
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "spectrum") {
            ret = _dev->SetRegister("/Registers/A_EN_MODE", 0x1);
            hw_config.energy.mode = hw_config.energy.SPECTRUM ;
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }

    }


    if (name == "noise.gauss.enable") {
        if (value == "true") {
            ret = _dev->SetRegister("/Registers/A_NS_ENABLE", 0x1);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.noise.gaussian.enabled = true;
            return NI_OK;
        }
        else if (value == "false") {
            ret = _dev->SetRegister("/Registers/A_NS_ENABLE", 0x0);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.noise.gaussian.enabled = false;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }

    }
    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt4810::IGetParamString(string name, string* value) {
    
    return NI_INVALID_PARAMETER;
}


NI_RESULT bd_dt4810::ExecuteCommand(string cmd, string param) {
    if (cmd == "initialize") {
        _dev->SetRegister("/Registers/A_TB_SEED_REPROGRAM", 0x1);
        _dev->SetRegister("/Registers/A_TB_SEED_REPROGRAM", 0x0);
        _dev->SetRegister("/Registers/A_EN_SEED_REPROGRAM", 0x1);
        _dev->SetRegister("/Registers/A_EN_SEED_REPROGRAM", 0x0);
        _dev->SetRegister("/Registers/A_NS_GAUSS_SEED_REPROGRAM", 0x1);
        _dev->SetRegister("/Registers/A_NS_GAUSS_SEED_REPROGRAM", 0x0);

        _dev->SetRegister("/Registers/A_TB_RESET", 0x1);
        _dev->SetRegister("/Registers/A_TB_RESET", 0x0);

        _dev->SetRegister("/Registers/A_GLOBAL_LSFR_SETTINGS", 0x4);
        return NI_OK;
    }
    
    return NI_INVALID_COMMAND;
}


NI_RESULT bd_dt4810::UpdateHardware()
{
    int ret;

   // int ret = _hal->WriteReg(board_params.offset.value, board_params.offset.address);
		
    if (ret) {
        return NI_ERROR_INTERFACE;
    } else
    return NI_OK;

}


NI_RESULT bd_dt4810::AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size) {
    
    SCISDK_EMULATOR_ENERGY_SPECTRUM* evt;
    *buffer = (SCISDK_EMULATOR_ENERGY_SPECTRUM*)malloc(sizeof(*evt));
    evt = (SCISDK_EMULATOR_ENERGY_SPECTRUM*)*buffer;

    if (evt == NULL) {
        return NI_ALLOC_FAILED;
    }

	evt->data = (uint32_t*)malloc(sizeof(uint32_t) * size);
 
    if (evt->data == NULL) {
        free(evt);
        return NI_ALLOC_FAILED;
    }

	evt->info.allocated_bins = size;
    evt->info.valid_bins= 0;
    
    return NI_OK;

}


NI_RESULT bd_dt4810::WriteData(void* buffer) {
    int ret = 0;
    uint64_t handle;

    if (buffer == NULL) {
        return NI_INVALID_BUFFER;
    }

    SCISDK_EMULATOR_ENERGY_SPECTRUM* p;
    p = (SCISDK_EMULATOR_ENERGY_SPECTRUM*)buffer;

    int i;
    unsigned int q;
    double max;
    double* cumulativo = new double[32000];
    unsigned int* Icumulativo = new unsigned int[32000];
    q = p->data[0];
    cumulativo[0] = 0;
    for (i = 1; i < 16384; i++)
    {
        if (p->info.valid_bins > i){
            q = p->data[i];
        } else {
            q = 0;
        }

        cumulativo[i] = cumulativo[i - 1] + q;
    }

    max = cumulativo[16383];

    for (i = 0; i < 16384; i++)
    {
        cumulativo[i] = ((cumulativo[i] / max) * (pow(2.0, 32) - 1));
        Icumulativo[i] =  (unsigned int)cumulativo[i];
    }
    uint32_t writted_data;
    _dev->SetRegister("/Registers/A_EN_PROG_MODE", 0x1);
	ret = _hal->WriteData(Icumulativo, 16384, 0x10000, 500, &writted_data);
    _dev->SetRegister("/Registers/A_EN_PROG_MODE", 0x0);
	if (ret) {
		return NI_ERROR_INTERFACE;
	}

	return NI_OK;

}
