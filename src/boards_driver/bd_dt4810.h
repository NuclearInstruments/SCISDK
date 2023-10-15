#ifndef HEADER_H_BD_DT4810_DRIVER
#define HEADER_H_BD_DT4810_DRIVER
#include <iostream>
#include <algorithm>
#include <list>
#include "../scisdk_scinode.h"
#include "../scisdk_device.h"

#include "bd_common.h"

#include <mutex>
#include <thread>  
#include <queue>
#include <atomic> 
using json = nlohmann::json;

using namespace std;

class bd_dt4810 : public  SciSDK_Node {
    public:
	    bd_dt4810(SciSDK_HAL *hal, void *dev, json j, string path);
        NI_RESULT ISetParamU32(string name, uint32_t value);
        NI_RESULT IGetParamU32(string name, uint32_t *value);
        NI_RESULT ISetParamDouble(string name, double value);
        NI_RESULT IGetParamDouble(string name, double* value);
        NI_RESULT ISetParamString(string name, string value);
        NI_RESULT IGetParamString(string name, string* value);
        NI_RESULT ExecuteCommand(string cmd, string param);
        NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void** buffer, int size);
        NI_RESULT WriteData(void* buffer);

private:
        NI_RESULT UpdateHardware();
		SciSDK_Device *_dev;


    struct {
        struct {
            double rate;
            enum {
                    PERIODIC,
                    POISSON
				} mode;
        } timebase;
        struct {
            uint32_t constant;
            enum {
                    FIXED,
                    SPECTRUM
            } mode;
        }energy;
        
        struct {
            double tau;
            double rise;
        }shape;

        struct {
            struct {
                double gain;
                bool enabled;
            } gaussian;
        }noise;
    } hw_config;

    struct {
        bd_reg A_TB_POISSON_P;
        bd_reg A_TB_RATE;
        bd_reg offset;
    } board_params;
    double clock_fpga;
    double clock_dac;
};
#endif 