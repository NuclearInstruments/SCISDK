#ifndef HEADER_H_DT5550W_CITIROCCONFIG
#define HEADER_H_DT5550W_CITIROCCONFIG

#include <iostream>
#include <algorithm>
#include <list>
#include "scisdk_scinode.h"

#include <mutex>
#include <thread>  
#include <queue>
#include <atomic> 
#include <bitset>
using json = nlohmann::json;

using namespace std;

class SciSDK_DT5550W_CitirocConfig : public  SciSDK_Node {
public:
	SciSDK_DT5550W_CitirocConfig(SciSDK_HAL *hal, json j, string path);

	NI_RESULT ISetParamU32(string name, uint32_t value);
	NI_RESULT ISetParamI32(string name, int32_t value);
	NI_RESULT ISetParamString(string name, string value);

	NI_RESULT IGetParamU32(string name, uint32_t *value);
	NI_RESULT IGetParamI32(string name, int32_t *value);
	NI_RESULT IGetParamString(string name, string *value);

	NI_RESULT ExecuteCommand(string cmd, string param);

private:

	string bitstream;
	const int NbChannels = 32;
	uint32_t address;
	uint32_t address_start;

	//params
	int sc_calibDacT[32];
	int sc_calibDacQ[32];
	int sc_enDiscri = 0;
	int sc_ppDiscri = 0;
	int sc_latchDiscri = 0;
	int sc_enDiscriT = 0;
	int sc_ppDiscriT = 0;
	int sc_enCalibDacQ = 0;
	int sc_ppCalibDacQ = 0;
	int sc_enCalibDacT = 0;
	int sc_ppCalibDacT = 0;
	int sc_mask[32];

	int sc_ppThHg = 0;
	int sc_enThHg = 0;
	int sc_ppThLg = 0;
	int sc_enThLg = 0;
	int sc_biasSca = 0;
	int sc_ppPdetHg = 0;
	int sc_enPdetHg = 0;
	int sc_ppPdetLg = 0;
	int sc_enPdetLg = 0;
	int sc_scaOrPdHg = 0;
	int sc_scaOrPdLg = 0;
	int sc_bypassPd = 0;
	int sc_selTrigExtPd = 0;
	int sc_ppFshBuffer = 0;
	int sc_enFsh = 0;
	int sc_ppFsh = 0;
	int sc_ppSshLg = 0;
	int sc_enSshLg = 0;
	int sc_shapingTimeLg = 0;
	int sc_ppSshHg = 0;
	int sc_enSshHg = 0;
	int sc_shapingTimeHg = 0;
	int sc_paLgBias = 0;
	int sc_ppPaHg = 0;
	int sc_enPaHg = 0;
	int sc_ppPaLg = 0;
	int sc_enPaLg = 0;
	int sc_fshOnLg = 0;
	int sc_enInputDac = 0;
	int sc_dacRef = 0;

	int sc_inputDac[32];
	int sc_cmdInputDac[32];
	int sc_paHgGain[32];
	int sc_paLgGain[32];
	int sc_CtestHg[32];
	int sc_CtestLg[32];
	int sc_enPa[32];

	int sc_ppTemp = 0;
	int sc_enTemp = 0;
	int sc_ppBg = 0;
	int sc_enBg = 0;
	int sc_enThresholdDac1 = 0;
	int sc_ppThresholdDac1 = 0;
	int sc_enThresholdDac2 = 0;
	int sc_ppThresholdDac2 = 0;
	int sc_threshold1 = 0;
	int sc_threshold2 = 0;
	int sc_enHgOtaQ = 0;
	int sc_ppHgOtaQ = 0;
	int sc_enLgOtaQ = 0;
	int sc_ppLgOtaQ = 0;
	int sc_enProbeOtaQ = 0;
	int sc_ppProbeOtaQ = 0;
	int sc_testBitOtaQ = 0;
	int sc_enValEvtReceiver = 0;
	int sc_ppValEvtReceiver = 0;
	int sc_enRazChnReceiver = 0;
	int sc_ppRazChnReceiver = 0;
	int sc_enDigitalMuxOutput = 0;
	int sc_enOr32 = 0;
	int sc_enNor32Oc = 0;
	int sc_triggerPolarity = 0;
	int sc_enNor32TOc = 0;
	int sc_enTriggersOutput = 0;

	NI_RESULT CmdGenerateBitConfig();
	NI_RESULT CmdDecodeBitConfig();
	NI_RESULT CmdWriteBitstream();
	NI_RESULT CmdReadBitstream();
};
#endif
