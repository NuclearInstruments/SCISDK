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

    RegisterParameter("boardapi/timebase.rate", "set generation rate", SciSDK_Paramcb::Type::d, this);
    const std::list<std::string> listOfTimebaseMode = { "fixed", "random"};
    const std::list<std::string> listOfAmplitudeMode = { "fixed", "spectrum" };

    const std::list<std::string> listOfShapeMode = { "exp", "pulse" };

    const std::list<std::string> listOfIoOutput = { "disabled", "run", "trigger", "trigger_valid", "saturation", "loopback"};
    const std::list<std::string> listOfIoInput = { "disabled", "trigger_gate", "trigger_veto", "output_gate", "output_veto", "trigger_external"};

    RegisterParameter("boardapi/timebase.mode", "set timebase mode", SciSDK_Paramcb::Type::str, listOfTimebaseMode, this);
    RegisterParameter("boardapi/timebase.parallizable", "set parallizable machine mode", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("boardapi/timebase.seed1", "set timebase seed 1", SciSDK_Paramcb::Type::U64,  this);
    RegisterParameter("boardapi/timebase.seed2", "set timebase seed 2", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("boardapi/timebase.seed3", "set timebase seed 3", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("boardapi/timebase.seed4", "set timebase seed 4", SciSDK_Paramcb::Type::U64, this);
    
    RegisterParameter("boardapi/amplitude.mode", "set amplitude mode", SciSDK_Paramcb::Type::str, listOfAmplitudeMode, this);
    RegisterParameter("boardapi/amplitude.fixed", "set amplitude fixed value", SciSDK_Paramcb::Type::U32, this);
    RegisterParameter("boardapi/amplitude.seed1", "set amplitude seed 1", SciSDK_Paramcb::Type::U64, this);

    RegisterParameter("boardapi/shape.drc.decay", "set decay constant for DRC", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/shape.drc.risetime", "set risetime for DRC", SciSDK_Paramcb::Type::d, this);

    RegisterParameter("boardapi/shape.pulse.risetime", "set the pulse rise time", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/shape.pulse.falltime", "set the pulse fall time", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/shape.pulse.flat", "set the pulse flat width", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/shape.pulse.scale", "set the decimator on the pulse", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/shape.mode", "set timebase mode", SciSDK_Paramcb::Type::str, listOfShapeMode, this);

    RegisterParameter("boardapi/noise.gauss.enable", "enable disable gaussian noise generator", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("boardapi/noise.gauss.gain", "set magniture of gaussian noise", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/noise.gauss.seed1", "set timebase seed 1", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("boardapi/noise.gauss.seed2", "set timebase seed 2", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("boardapi/noise.gauss.seed3", "set timebase seed 3", SciSDK_Paramcb::Type::U64, this);
    RegisterParameter("boardapi/noise.gauss.seed4", "set timebase seed 4", SciSDK_Paramcb::Type::U64, this);
    
    RegisterParameter("boardapi/main.invert", "invert signal polarity", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("boardapi/main.gain", "global gain", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/main.offset", "global offseet", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/main.output.enable", "enable output", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);
    RegisterParameter("boardapi/main.output.gate", "gate output", SciSDK_Paramcb::Type::str, listOfTrueFalse, this);

    RegisterParameter("boardapi/main.io.output", "io output mode", SciSDK_Paramcb::Type::str, listOfIoOutput, this);
    RegisterParameter("boardapi/main.io.input", "io input mode", SciSDK_Paramcb::Type::str, listOfIoInput, this);
    RegisterParameter("boardapi/main.io.width", "io output pulse width", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/main.io.delay", "io output pulse delay", SciSDK_Paramcb::Type::d, this);

    RegisterParameter("boardapi/mon.icr", "input count rate", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.ocr", "output count rate", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.ic", "input count", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.oc", "output count", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.sc", "saturated count", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.deadp", "percent dead event", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.dead", "dead time in us", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.rt", "real time in us", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.lt", "live time in us", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.los", "number of lost events", SciSDK_Paramcb::Type::d, this);
    RegisterParameter("boardapi/mon.ovl", "number of overflow events", SciSDK_Paramcb::Type::d, this);

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

    hw_config.shape.pulse_risetime = 32.0;
    hw_config.shape.pulse_flattime = 1024.0;
    hw_config.shape.pulse_falltime = 32.0;
    hw_config.shape.pulse_scale = 1.0;
    hw_config.shape.mode = hw_config.shape.EXP;

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
    double dacTs = 1e9 / clock_dac;
    double fpgaTS = 1e9 / clock_fpga;
    if (name == "timebase.rate") {
        if ((value > 10000000) || (value < 0)) {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
        else {
            uint32_t rate = (uint32_t)((clock_fpga) / value);
            uint32_t prate = (uint32_t)(((value * (1.0 / clock_dac))) * pow(2.0, 32));
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
            float MX = (float) Mf1;
            for (int j = 0; j < i; j++) {
                MX = MX * Mf1;
            }
            Ma[i] = MX;
            MaI[i] = Ma[i] * 0xFFFFFFFF;
            cout << "Ma[" << i << "] : " << std::hex << (uint64_t)(Ma[i] * (0xFFFFFFFF)) << "   ";
        }

		double ratio =2* clock_dac / clock_fpga;
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

    if (name == "shape.pulse.risetime") {
        
        if ((hw_config.shape.pulse_risetime + hw_config.shape.pulse_flattime + hw_config.shape.pulse_falltime)* hw_config.shape.pulse_scale > 4093 * fpgaTS) {
            return NI_PARAMETER_OUT_OF_RANGE;
        }

        hw_config.shape.pulse_risetime = value;
        uint32_t r;
        r = hw_config.shape.pulse_risetime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_LE", r<1?1:r);
        r = hw_config.shape.pulse_flattime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_PE", r < 1 ? 1 : r);
        r = hw_config.shape.pulse_falltime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_TE", r < 1 ? 1 : r);
        return NI_OK;
    }
    if (name == "shape.pulse.falltime") {
        
        if ((hw_config.shape.pulse_risetime + hw_config.shape.pulse_flattime + hw_config.shape.pulse_falltime) * hw_config.shape.pulse_scale > 4093 * fpgaTS) {
            return NI_PARAMETER_OUT_OF_RANGE;
        }

        hw_config.shape.pulse_falltime = value;

        uint32_t r;
        r = hw_config.shape.pulse_risetime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_LE", r < 1 ? 1 : r);
        r = hw_config.shape.pulse_flattime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_PE", r < 1 ? 1 : r);
        r = hw_config.shape.pulse_falltime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_TE", r < 1 ? 1 : r);

        return NI_OK;
    }
    if (name == "shape.pulse.flat") {
        
        if ((hw_config.shape.pulse_risetime + hw_config.shape.pulse_flattime + hw_config.shape.pulse_falltime) * hw_config.shape.pulse_scale > 4093 * fpgaTS) {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
        
        hw_config.shape.pulse_flattime = value;

        uint32_t r;
        r = hw_config.shape.pulse_risetime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_LE", r < 1 ? 1 : r);
        r = hw_config.shape.pulse_flattime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_PE", r < 1 ? 1 : r);
        r = hw_config.shape.pulse_falltime / (fpgaTS);
        ret = _dev->SetRegister("/Registers/A_SH_TE", r < 1 ? 1 : r);

        return NI_OK;
    }
    if (name == "shape.pulse.scale") {
        hw_config.shape.pulse_scale = value;
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

    if (name == "main.io.width") {
        uint32_t g = value / (fpgaTS);
        uint32_t reg_value;
        g = g > 0xFFFF ? 0xFFFF : g;
        _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
        reg_value = (reg_value & 0x0000FFFF) + (g << 16);
        ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
        if (ret) return NI_ERROR_INTERFACE;
        return NI_OK;
    }


    if (name == "main.io.delay") {
        uint32_t d = value / (fpgaTS);
        d = d > 4095 ? 4095 : d;
        ret = _dev->SetRegister("/Registers/A_OM_IO_DELAY", d);
        if (ret) return NI_ERROR_INTERFACE;
        return NI_OK;
    }

    return NI_INVALID_PARAMETER;
    
    
}

NI_RESULT bd_dt4810::IGetParamDouble(string name, double* value)
{
    int ret = 0;
    double fpgaTS = 1e9 / clock_fpga;

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



    if (name == "shape.pulse.risetime") {
        *value = hw_config.shape.pulse_risetime;
    }

    if (name == "shape.pulse.falltime") {
        *value = hw_config.shape.pulse_falltime;
    }

    if (name == "shape.pulse.flat") {
        *value = hw_config.shape.pulse_flattime;
    }

    if (name == "shape.pulse.scale") {
        *value = hw_config.shape.pulse_scale;
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

    if (name == "main.io.width") {
        uint32_t l;
        ret = _dev->GetRegister("/Registers/A_OM_IO", &l);
        *value = ((double)(l >> 16)) * fpgaTS;
    }

    if (name == "main.io.delay") {
        uint32_t l;
        ret = _dev->GetRegister("/Registers/A_OM_IO_DELAY", &l);
        *value = ((double)(l)) * fpgaTS;
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

    if (name == "shape.mode") {
        if (value == "exp") {
            ret = _dev->SetRegister("/Registers/A_SH_SHAPEMODE", 0x0);
            hw_config.shape.mode = hw_config.shape.EXP;
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "pulse") {
            ret = _dev->SetRegister("/Registers/A_SH_SHAPEMODE", 0x1);
            hw_config.shape.mode = hw_config.shape.PULSE;
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


    if (name == "main.io.output") {
        if (value == "disabled") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFFFF00)  | (0x0);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "run") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFFFF00) | (0x1);
            //cout << endl << endl << "                           RUN: " << reg_value << endl << endl;
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "trigger") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFFFF00) | (0x2);
            //cout << endl << endl << "                           TRIGGER: " << reg_value << endl << endl;
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "trigger_valid") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFFFF00) | (0x3);
            //cout << endl << endl << "                           TRIGGER VALID: " << reg_value << endl << endl;
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "saturation") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFFFF00) | (0x4);
            //cout << endl << endl << "                           SATURATION: " << reg_value << endl << endl;
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "loopback") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFFFF00) | (0x5);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else {
            return NI_PARAMETER_OUT_OF_RANGE;
        }
    }


    if (name == "main.io.input") {
        if (value == "disabled") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFF00FF) | (0x0 << 8);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "trigger_gate") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFF00FF) | (0x1 << 8);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "trigger_veto") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFF00FF) | (0x2 << 8);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "output_gate") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFF00FF) | (0x3 << 8);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "output_veto") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFF00FF) | (0x4 << 8);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
            return NI_OK;
        }
        else if (value == "trigger_external") {
            uint32_t reg_value;
            _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
            reg_value = (reg_value & 0xFFFF00FF) | (0x5 << 8);
            ret = _dev->SetRegister("/Registers/A_OM_IO", reg_value);
            if (ret) return NI_ERROR_INTERFACE;
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
    uint32_t reg_value;
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

    if (name == "timebase.mode") {
        ret = _dev->GetRegister("/Registers/A_TB_MODE", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x0) ? "fixed" : "random";
        return NI_OK;
    }

    if (name == "amplitude.mode") {
        ret = _dev->GetRegister("/Registers/A_EN_MODE", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x0) ? "fixed" : "spectrum";
        return NI_OK;
    }

    if (name == "shape.mode") {
        ret = _dev->GetRegister("/Registers/A_SH_SHAPEMODE", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x0) ? "exp" : "pulse";
        return NI_OK;
    }

    if (name == "noise.gauss.enable") {
        ret = _dev->GetRegister("/Registers/A_NS_ENABLE", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x1) ? "true" : "false";
        return NI_OK;
    }

    if (name == "main.output.enable") {
        ret = _dev->GetRegister("/Registers/A_OM_ON", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x1) ? "true" : "false";
        return NI_OK;
    }

    if (name == "main.output.gate") {
        ret = _dev->GetRegister("/Registers/A_OM_GATE", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x1) ? "true" : "false";
        return NI_OK;
    }

    if (name == "main.invert") {
        ret = _dev->GetRegister("/Registers/A_OM_INVERT", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        *value = (reg_value == 0x0) ? "true" : "false";
        return NI_OK;
    }

    if (name == "main.io.output") {
        ret = _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        switch (reg_value & 0xFF) {
        case 0x0: *value = "disabled"; break;
        case 0x1: *value = "run"; break;
        case 0x2: *value = "trigger"; break;
        case 0x3: *value = "trigger_valid"; break;
        case 0x4: *value = "saturation"; break;
        case 0x5: *value = "loopback"; break;
        default: return NI_PARAMETER_OUT_OF_RANGE;
        }
        return NI_OK;
    }

    if (name == "main.io.input") {
        ret = _dev->GetRegister("/Registers/A_OM_IO", &reg_value);
        if (ret) return NI_ERROR_INTERFACE;

        switch ((reg_value >> 8) & 0xFF) {
        case 0x0: *value = "disabled"; break;
        case 0x1: *value = "trigger_gate"; break;
        case 0x2: *value = "trigger_veto"; break;
        case 0x3: *value = "output_gate"; break;
        case 0x4: *value = "output_veto"; break;
        case 0x5: *value = "trigger_external"; break;
        default: return NI_PARAMETER_OUT_OF_RANGE;
        }
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
   // int ret;

   //// int ret = _hal->WriteReg(board_params.offset.value, board_params.offset.address);
		
   // if (ret) {
   //     return NI_ERROR_INTERFACE;
   // } else
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
