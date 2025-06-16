#include "bd_dt5560se.h"
#include <functional>
#include <chrono>
#include <thread>

#define DT5560_IR_TERM_SET   10
#define DT5560_IR_TERM_GET   10
#define DT5560_IR_DIV_SET    11
#define DT5560_IR_DIV_GET    11
#define DT5560_IR_GAIN_SET   7
#define DT5560_IR_GAIN_GET   7
#define DT5560_IR_OFFSET_SET 8
#define DT5560_IR_OFFSET_GET 8
#define DT5560_IR_SHAPER     9
#define DT5560_IR_SHAPER     9
#define DT5560_IR_SYNC_REG   20
#define DT5560_IR_CONFIGURE_AFE   255


#define DT5771_IR_IMP_SET		10
#define DT5771_IR_RANGE_SET		11
#define DT5771_IR_BW_SET		41
#define DT5771_IR_COUPLIG_SET	9
#define DT5771_IR_GAIN_SET		7
#define DT5771_IR_DTERM_SET		40
#define DT5771_IR_OFFSET_SET	8



/*
BOARD DEVICE DRIVER FOR DT5560SE


Framework Compatible version: 1.0

*/

bd_dt5560se::bd_dt5560se(SciSDK_HAL* hal, json j, string path) : SciSDK_Node(hal, j, path) {
    const std::list<std::string> listOfCoupling = { "dc", "1us", "10us", "30us" };
    const std::list<std::string> listOfBoolean = { "false", "true" };
    const std::list<std::string> listOfSync = { "lemo0","lemo1", "lemo2", "sync0", "sync1", "sync2", "clk", "reg0", "reg1", "reg2" };
    for (int i = 0; i < 32; i++) {
		RegisterParameter("boardapi/analog/CH" + std::to_string(i) + ".offset", "set analog offset", SciSDK_Paramcb::Type::I32, this);
        analog_settings.offset_mV[i] = 0;
    }
	for (int i =0;i<16;i++){
        RegisterParameter("boardapi/analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) + ".gain", "set analog gain", SciSDK_Paramcb::Type::I32, this);
		RegisterParameter("boardapi/analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) + ".50r", "set analog gain", SciSDK_Paramcb::Type::str, listOfBoolean, this);
		RegisterParameter("boardapi/analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) + ".div", "set analog gain", SciSDK_Paramcb::Type::str, listOfBoolean, this);
        analog_settings.r50[i] = false;
        analog_settings.div[i] = false;
        analog_settings.gain[i] = 0;
	}
	
	RegisterParameter("boardapi/analog.shaper", "set analog shaper", SciSDK_Paramcb::Type::str, listOfCoupling, this);
    RegisterParameter("boardapi/sync_reg.0", "set sync reg 0", SciSDK_Paramcb::Type::I32, this);
    RegisterParameter("boardapi/sync_reg.1", "set sync reg 1", SciSDK_Paramcb::Type::I32, this);
    RegisterParameter("boardapi/sync_reg.2", "set sync reg 2", SciSDK_Paramcb::Type::I32, this);

    analog_settings.coupling[0] = COUPLING::DC;
    analog_settings.coupling[1] = COUPLING::DC;
	/*RegisterParameter("daqsync.lane0", "set daqsync source (0)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("daqsync.lane1", "set daqsync source (1)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("daqsync.lane2", "set daqsync source (2)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("syncout.lane0", "set sync connector source (0)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("syncout.lane1", "set sync connector source (1)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("syncout.lane2", "set sync connector source (2)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("lemo0", "set lemo connector source (0)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("lemo1", "set lemo connector source (1)", SciSDK_Paramcb::Type::str, listOfSync, this);
	RegisterParameter("lemo2", "set lemo connector source (2)", SciSDK_Paramcb::Type::str, listOfSync, this);*/
}




NI_RESULT bd_dt5560se::ISetParamI32(string name, int32_t value)
{
    for (int i = 0; i < 32; i++) {
        if (name == "analog/CH" + std::to_string(i) + ".offset") {
			analog_settings.offset_mV[i] = value;
            return NI_OK;
        }
    }
  
     for (int i = 0; i < 16; i++) {
        if (name == "analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) +  ".gain") {
            analog_settings.gain[i] = value;
            return NI_OK;
        }
		
    }

     for (int i = 0; i < 3; i++) {
         if (name == "sync_reg." + std::to_string(i)) {
             int ret = _hal->ConfigurationRegisterSet(value, DT5560_IR_SYNC_REG, i);
			 if (ret != 0) {
				 return NI_ERROR_INTERFACE;
			 }
             return NI_OK;
         }

     }
  

    return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt5560se::IGetParamI32(string name, int32_t *value)
{
    for (int i = 0; i < 32; i++) {
        if (name == "analog/CH" + std::to_string(i) + ".offset") {
            *value = analog_settings.offset_mV[i];
            return NI_OK;
        }
    }
        
    for (int i = 0; i < 16; i++) {
        if (name == "analog/CH" + std::to_string(i * 2) + "_" + std::to_string(i * 2 + 1) + ".gain") {
            *value = analog_settings.gain[i];
            return NI_OK;
        }
    }

    return NI_INVALID_PARAMETER;
}


NI_RESULT bd_dt5560se::ISetParamString(string name, string value)
{
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    
	for (int i = 0; i < 16; i++) {
		if (name == "analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) +  ".50r") {
            if (value == "true") {
                analog_settings.r50[i] = true;
            }
            else {
                analog_settings.r50[i] = false;
            }
			return NI_OK;
        }
		
		if (name == "analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) +  ".div") {
            if (value == "true") {
                analog_settings.div[i] = true;
            }
            else {
                analog_settings.div[i] = false;
            }
            return NI_OK;
        }
    }

     if (name == "analog.shaper")
     {
        for (int i=0;i<2;i++)
        {
	        if (value == "dc")
	        {
	        	analog_settings.coupling[i] = COUPLING::DC;
			}
			else if (value == "1us")
			{
				analog_settings.coupling[i] = COUPLING::US1;
			}
			else if (value == "10us")
			{
				analog_settings.coupling[i] = COUPLING::US10;
			}
			else if (value == "30us")
			{
				analog_settings.coupling[i] = COUPLING::US30;
			}
			else
			{
				return NI_INVALID_PARAMETER;
			}
		}
     }

    for (int i = 0; i < 3; i++) {
        if (name == "daqsync.lane" + std::to_string(i)) {
            bd_dt5560se::SyncType t = stringToSyncType(value);
            if (t == bd_dt5560se::SyncType::INVALID)
                return NI_INVALID_PARAMETER;
            else {
                sync.daqsyncsource[i] = t;
                return NI_OK;
            }
        }
        if (name == "syncout.lane" + std::to_string(i)) {
            bd_dt5560se::SyncType t = stringToSyncType(value);
            if (t == bd_dt5560se::SyncType::INVALID)
                return NI_INVALID_PARAMETER;
            else {
                sync.syncsource[i] = t;
                return NI_OK;
            }
        }
        if (name == "lemo" + std::to_string(i)) {
            bd_dt5560se::SyncType t = stringToSyncType(value);
            if (t == bd_dt5560se::SyncType::INVALID)
                return NI_INVALID_PARAMETER;
            else {
                sync.lemooutsource[i] = t;
                return NI_OK;
            }
        }
    }
    
	return NI_INVALID_PARAMETER;
}

NI_RESULT bd_dt5560se::IGetParamString(string name, string* value)
{
	for (int i = 0; i < 16; i++) {
		if (name == "analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) +  ".50r") {
			if (analog_settings.r50[i])
				*value = "true";
			else
				*value = "false";
            return NI_OK;
        }
		
		if (name == "analog/CH" + std::to_string(i*2) + "_" + std::to_string(i*2+1) +  ".div") {
            if (analog_settings.div[i])
                *value = "true";
            else
                *value = "false";
            return NI_OK;
        }
    }

    if (name == "analog.shaper")
    {
	    switch (analog_settings.coupling[0])
	    {
	    	case COUPLING::DC:
				*value = "dc";
				break;
			case COUPLING::US1:
				*value = "1us";
				break;
			case COUPLING::US10:
				*value = "10us";
				break;
			case COUPLING::US30:
				*value = "30us";
				break;
			default:
				*value = "dc";
				break;
		}
		return NI_OK;
    }

    for (int i = 0; i < 3; i++) {
        if (name == "daqsync.lane" + std::to_string(i)) {
            *value = syncTypeToString(sync.daqsyncsource[i]);
            return NI_OK;
        }
        if (name == "syncout.lane" + std::to_string(i)) {
            *value = syncTypeToString(sync.syncsource[i]);
            return NI_OK;
        }
        if (name == "lemo" + std::to_string(i)) {
            *value = syncTypeToString(sync.lemooutsource[i]);
            return NI_OK;
        }
    }


	return NI_INVALID_PARAMETER;
}


bd_dt5560se::SyncType bd_dt5560se::stringToSyncType(const std::string& str) {
    // Convert the string to uppercase
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    // Convert the string to a SyncType enum value
    if (s == "LEMO0") {
        return SyncType::LEMO0;
    }
    else if (s == "LEMO1") {
        return SyncType::LEMO1;
    }
    else if (s == "LEMO2") {
        return SyncType::LEMO2;
    }
    else if (s == "SYNC0") {
        return SyncType::SYNC0;
    }
    else if (s == "SYNC1") {
        return SyncType::SYNC1;
    }
    else if (s == "SYNC2") {
        return SyncType::SYNC2;
    }
    else if (s == "CLK") {
        return SyncType::CLK;
    }
    else if (s == "REG0") {
        return SyncType::REG0;
    }
    else if (s == "REG1") {
        return SyncType::REG1;
    }
    else if (s == "REG2") {
        return SyncType::REG2;
    }
    else {
        // Return a default value if the string does not match any of the enum values
        return SyncType::INVALID;
    }
}


std::string bd_dt5560se::syncTypeToString(bd_dt5560se::SyncType t) {
    switch (t) {
    case SyncType::LEMO0:
        return "lemo0";
    case SyncType::LEMO1:
        return "lemo1";
    case SyncType::LEMO2:
        return "lemo2";
    case SyncType::SYNC0:
        return "sync0";
    case SyncType::SYNC1:
        return "sync1";
    case SyncType::SYNC2:
        return "sync2";
    case SyncType::CLK:
        return "clk";
    case SyncType::REG0:
        return "reg0";
    case SyncType::REG1:
        return "reg1";
    case SyncType::REG2:
        return "reg2";
    default:
        // Return a default value if the enum value is not recognized
        return "lemo0";
    }
}


NI_RESULT bd_dt5560se::UpdateHardware()
{
    int ret = 0;
    for (int i = 0; i < 32; i++) {
        uint32_t p =  analog_settings.offset_mV[i];
        ret = _hal->ConfigurationRegisterSet(p, DT5560_IR_OFFSET_SET, i);
        if (ret) {
            return NI_ERROR_INTERFACE;
        }
    }

    for (int i = 0; i < 16; i++) {
        ret = _hal->ConfigurationRegisterSet(analog_settings.gain[i], DT5560_IR_GAIN_SET, i);
        if (ret) {
            return NI_ERROR_INTERFACE;
        }
        
    }

    uint32_t r50 = 0;
    uint32_t div = 0;
    for (int i = 0; i < 16; i++) {
        r50 |= (analog_settings.r50[i] ? 1 : 0) << (i * 2);
        r50 |= (analog_settings.r50[i] ? 1 : 0) << (i * 2+1);
        div |= (analog_settings.div[i] ? 0 : 1) << (i*2);
        div |= (analog_settings.div[i] ? 0 : 1) << (i * 2+1);
    }

    ret = _hal->ConfigurationRegisterSet(r50, DT5560_IR_TERM_SET, 0);
    if (ret) {
        return NI_ERROR_INTERFACE;
    }

    ret = _hal->ConfigurationRegisterSet(div, DT5560_IR_DIV_SET, 0);
    if (ret) {
        return NI_ERROR_INTERFACE;
    }

    ret = _hal->ConfigurationRegisterSet((uint32_t)analog_settings.coupling[0], DT5560_IR_SHAPER, 0);
    if (ret) {
        return NI_ERROR_INTERFACE;
    }

    //sleep for 10 ms
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ret = _hal->ConfigurationRegisterSet(0, DT5560_IR_CONFIGURE_AFE, 0);
    if (ret) {
        return NI_ERROR_INTERFACE;
    }

	return NI_OK;

}

NI_RESULT bd_dt5560se::ExecuteCommand(string cmd, string param) {
    string rootpath;
    string board_path;

    if (cmd == "configure")
    {
	    return UpdateHardware();
    }


    return NI_INVALID_COMMAND;
}