#include "bd_dt4810.h"
#include <functional>
#include <chrono>
#include <thread>

/*
BOARD DEVICE DRIVER FOR DT1260


Framework Compatible version: 1.0

*/

bd_dt4810::bd_dt4810(SciSDK_HAL *hal, void *dev, json j, string path) : SciSDK_Node(hal, dev, j, path) {
    
    _dev = (SciSDK_Device*)dev;
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
    RegisterParameter("main.offset", "global offseet", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/main.output.enable", "enable output", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("boardapi/main.output.gate", "gate output", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);


    RegisterParameter("mon.icr", "input count rate", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.ocr", "output count rate", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.ic", "input count", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.oc", "output count", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.sc", "saturated count", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.deadp", "percent dead event", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.dead", "dead time in us", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.rt", "real time in us", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.lt", "live time in us", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.los", "number of lost events", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("mon.ovl", "number of overflow events", SciSDK_Paramcb::Type::d, this);

    RegisterParameter("boardapi/fwver", "firmware version", SciSDK_Paramcb::Type::str, this);
    RegisterParameter("boardapi/hwver", "hardware version", SciSDK_Paramcb::Type::str, this);


    hw_config.energy.mode = hw_config.energy.FIXED;
    hw_config.energy.constant = 10000;
    hw_config.noise.gaussian.enabled = false;
    hw_config.noise.gaussian.gain = 0;
    hw_config.shape.rise = 0;
    hw_config.shape.tau = 2;
    hw_config.timebase.mode = hw_config.timebase.PERIODIC;
    hw_config.timebase.rate = 1000;

}


NI_RESULT bd_dt4810::ISetParamU32(string name, uint32_t value)
{
    int ret = 0;
	if (name == "amplitude.fixed") {
        if ((value > 16384) || (value<0)) {
            return NI_PARAMETER_OUT_OF_RANGE;
        } else {
            ret = _dev->SetRegister("/Registers/A_EN_FIXED_ENERGY", value*2);
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
    if (name == "main.offset") {
        *value = hw_config.global.offset;
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
            uint32_t prate = ((value * (1.0 / clock_dac))) * pow(2.0, 32);
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
            double compensation = 1/( - 0.135 * log(value) + 2.6091);
            float alfa =  exp(-1 / (1e-9*value * compensation * clock_dac));
            double a[8];
            uint32_t aU32[8];
            double b;
            uint32_t bU32;

            for (int i = 0; i < 8; i++) {
                float aX = alfa;
                for (int j = 0; j < i; j++) {
                    aX = aX * alfa;
                }
                a[i] = aX;
				aU32[i] = a[i] * 0xFFFFFF;
                cout << "a[" << i << "] : " << std::hex << (uint32_t)(a[i] * (0xFFFFFF)) << "   ";
            }

            b = 1 - alfa;
			bU32 = b * 0xFFFF;
            ret = 0;
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A1", aU32[0]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A2", aU32[1]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A3", aU32[2]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A4", aU32[3]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A5", aU32[4]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A6", aU32[5]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A7", aU32[6]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_A8", aU32[7]);
            ret |= _dev->SetRegister("/Registers/A_SH_DRC_B", bU32);
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

    if (name == "main.gain") {
        uint32_t g = value * 0xFFFF;
        ret = _dev->SetRegister("/Registers/A_OM_GAIN", g);
        if (ret) return NI_ERROR_INTERFACE;
        hw_config.global.gain = value;
        return NI_OK;

    }
    if (name == "main.offset") {
        uint32_t g = 0x7FFF-(value*2) ;
        ret = _dev->SetRegister("/Registers/A_OM_OFFSET", g);
        if (ret) return NI_ERROR_INTERFACE;
        hw_config.global.gain = value;
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

    if (name == "mon.icr") {
        uint32_t l=0, h=0;
        ret = _dev->GetRegister("/Registers/a_ICR", &l);
		if (ret) return NI_ERROR_INTERFACE;
		uint64_t u64 = l + (((uint64_t)h)<<32L);
		*value = ((double)u64)*4;
    }

    if (name == "mon.ocr") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_OCR", &l);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = ((double)u64)* 4;
    }

    if (name == "mon.ic") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_IC_L", &l);
        ret = _dev->GetRegister("/Registers/a_IC_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (double)u64;
    }
    if (name == "mon.oc") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_OC_L", &l);
        ret = _dev->GetRegister("/Registers/a_OC_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (double)u64;
    }
    if (name == "mon.sc") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_SC_L", &l);
        ret = _dev->GetRegister("/Registers/a_SC_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (double)u64;
    }

    if (name == "mon.deadp") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_IC_L", &l);
        ret = _dev->GetRegister("/Registers/a_IC_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t ic = l + (((uint64_t)h) << 32L);
        ret = _dev->GetRegister("/Registers/a_OC_L", &l);
        ret = _dev->GetRegister("/Registers/a_OC_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t oc = l + (((uint64_t)h) << 32L);

        *value = (1-((double)oc)/ ((double)ic)) * 100.0;
    }
    
    if (name == "mon.dead") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_DT_L", &l);
        ret = _dev->GetRegister("/Registers/a_DT_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (((double)u64) * 2e-3) ;
    }
    
    if (name == "mon.rt") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_RT_L", &l);
        ret = _dev->GetRegister("/Registers/a_RT_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (((double)u64) *2e-3);
    }

    if (name == "mon.lt") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_LT_L", &l);
        ret = _dev->GetRegister("/Registers/a_LT_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (((double)u64) * 2e-3);
    }

    if (name == "mon.los") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_LOS_L", &l);
        ret = _dev->GetRegister("/Registers/a_LOS_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (double)u64;
    }

    if (name == "mon.ovl") {
        uint32_t l = 0, h = 0;
        ret = _dev->GetRegister("/Registers/a_OVL_L", &l);
        ret = _dev->GetRegister("/Registers/a_OVL_H", &h);
        if (ret) return NI_ERROR_INTERFACE;
        uint64_t u64 = l + (((uint64_t)h) << 32L);
        *value = (double)u64;
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

    if (name == "main.output.enable") {
        if (value == "true") {
            ret = _dev->SetRegister("/Registers/A_OM_ON", 0x1);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.global.en = true;
            return NI_OK;
        }
        else if (value == "false") {
            ret = _dev->SetRegister("/Registers/A_OM_ON", 0x0);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.global.en = false;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
    }

    if (name == "main.output.gate") {
        if (value == "true") {
            ret = _dev->SetRegister("/Registers/A_OM_GATE", 0x1);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.global.tmgate = true;
            return NI_OK;
        }
        else if (value == "false") {
            ret = _dev->SetRegister("/Registers/A_OM_GATE", 0x0);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.global.tmgate = false;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
    }
    
    if (name == "main.invert") {
        if (value == "true") {
            ret = _dev->SetRegister("/Registers/A_OM_INVERT", 0x0);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.global.invert = true;
            return NI_OK;
        }
        else if (value == "false") {
            ret = _dev->SetRegister("/Registers/A_OM_INVERT", 0x1);
            if (ret) return NI_ERROR_INTERFACE;
            hw_config.global.invert = false;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
    }
    
    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt4810::IGetParamString(string name, string* value) {
    int ret;
    if (name == "fwver") {
		uint32_t u32;
        ret = _dev->GetRegister("/Registers/a_FWVER", &u32);
        if (ret) return NI_ERROR_INTERFACE;

        std::stringstream hexStream;
        hexStream << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << u32;
        std::string hexString = hexStream.str();

        // Rimuovi il prefisso "0x" dalla stringa
        hexString = hexString.substr(2);

        // Dividi la stringa in quattro parti da due caratteri ciascuna
        std::string year = hexString.substr(0, 2);
        std::string month = hexString.substr(2, 2);
        std::string day = hexString.substr(4, 2);
        std::string revision = hexString.substr(6, 2);

        // Costruisci la stringa di versione nel formato "yyyy.mm.dd.revisione"
        std::string finalVersion = "20" + year + "." + month + "." + day + "." + revision;

        *value = finalVersion;
        return NI_OK;
		
    }

    if (name == "hwver") {
        uint32_t u32;
        ret = _dev->GetRegister("/Registers/a_HWVER", &u32);
        if (ret) return NI_ERROR_INTERFACE;

        std::stringstream hexStream;
        hexStream << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << u32;
        std::string hexString = hexStream.str();

        // Rimuovi il prefisso "0x" dalla stringa
        hexString = hexString.substr(2);

        // Dividi la stringa in quattro parti da due caratteri ciascuna
        std::string year = hexString.substr(0, 2);
        std::string month = hexString.substr(2, 2);
        std::string day = hexString.substr(4, 2);
        std::string revision = hexString.substr(6, 2);

        // Costruisci la stringa di versione nel formato "yyyy.mm.dd.revisione"
        std::string finalVersion = year + "." + month + "." + day + "." + revision;

        *value = finalVersion;
        return NI_OK;

    }
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
        _dev->SetRegister("/Registers/A_OM_INVERT", 0x1);
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
    //for (i = 1; i < 2000; i++) {
    //    cout << p->data[i] << endl;
    //}
	//cout << "valid bins " << p->info.valid_bins << endl;
    //cout << "allocated bins " << p->info.allocated_bins  << endl;
    //cout << "magic " << p->magic << endl;
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
