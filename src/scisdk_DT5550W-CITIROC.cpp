#include "scisdk_DT5550W-CITIROC.h"

SciSDK_DT5550W_Citiroc::SciSDK_DT5550W_Citiroc(SciSDK_HAL * hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address = (uint32_t)j.at("Address");

	for (int i = 0; i < 32; i++) {
		sc_calibDacT[i] = 0;
		sc_calibDacQ[i] = 0;
		sc_mask[i] = 0;
		sc_inputDac[i] = 0;
		sc_cmdInputDac[i] = 0;
		sc_paHgGain[i] = 0;
		sc_paLgGain[i] = 0;
		sc_CtestHg[i] = 0;
		sc_CtestLg[i] = 0;
		sc_enPa[i] = 0;
	}

	RegisterParameter("bitstream", "ASIC BITSTREAM", SciSDK_Paramcb::Type::str, this);

	for (int i = 0; i < NbChannels; i++) {
		RegisterParameter("sc_calibDacT_" + to_string(i), "Ch0 to 31 4-bit DAC_t([0..3]) ", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_calibDacQ_" + to_string(i), "Ch0 to 31 4-bit DAC([0..3]) ", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_mask_" + to_string(i), " Discriminator mask (channel 0 to 31) [0: Masked – 1 : unmasked]", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_inputDac_" + to_string(i), "", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_cmdInputDac_" + to_string(i), "", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_paHgGain_" + to_string(i), "High Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_paLgGain_" + to_string(i), "Low Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_CtestHg_" + to_string(i), "", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_CtestLg_" + to_string(i), "", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_enPa_" + to_string(i), "", SciSDK_Paramcb::Type::U32, this);
	}

	RegisterParameter("sc_enDiscri", "Enable Discriminator Charge [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppDiscri", "discriminator Charge power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_latchDiscri", "Select latched (RS : 1) or direct output (trigger : 0) on charge discriminator", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enDiscriT", "Enable Discriminator Time [0 : disabled, force off – 1 : Enabled] ", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppDiscriT", "discriminator Time power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enCalibDacQ", "Enable 4-bit DAC charge [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppCalibDacQ", "4-bit DAC charge power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enCalibDacT", "Enable 4-bit DAC time [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppCalibDacT", "4-bit DAC time power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThHg", "High Gain Track & Hold power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThHg", "Enable High Gain Track & Hold [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThLg", "Low Gain Track & Hold power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThLg", "Enable Low Gain Track & Hold [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_biasSca", "SCA bias ( 1 = weak bias, 0 = high bias 5MHz ReadOut Speed)", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPdetHg", "High Gain Peak detector power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPdetHg", "Enable High Gain Peak detector [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPdetLg", "Low Gain Peak detector power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPdetLg", "Enable Low Gain Peak detector [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_scaOrPdHg", "Select Track & Hold or Peak Detector – High Gain [0: peak detector – 1 : T&H]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_scaOrPdLg", " Select Track & Hold or Peak Detector – Low Gain [0: peak detector – 1 : T&H]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_bypassPd", "Bypass Peak Sensing Cell [0 : cell active – 1 : cell bypassed]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_selTrigExtPd", "Select peak sensing cell trigger [0 : internal trigger – 1 : external trigger]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppFshBuffer", "fast shaper follower power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enFsh", "Enable fast shaper [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppFsh", "fast shaper power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppSshLg", "low gain slow shaper power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enSshLg", "Enable Low Gain Slow Shaper [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_shapingTimeLg", "Low gain shaper time constant commands (0..2) [active low]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppSshHg", "high gain slow shaper power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enSshHg", "Enable high gain Slow Shaper [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_shapingTimeHg", "High gain shaper time constant commands (0..2) [active low]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_paLgBias", "Low Gain PreAmp bias [0: normal bias - 1: weak bias]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPaHg", "High Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPaHg", "Enable High Gain preamp [0 : disabled, force off – 1 : Enabled] ", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPaLg", "Low Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPaLg", "Enable Low Gain preamp [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_fshOnLg", "Select preamp to connect to Fast Shaper [0:fast shaper on HG preamp – 1 : fast shaper on LG preamp", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enInputDac", "Enable 32 input 8-bit DACs [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_dacRef", "8-bit input DAC Voltage Reference (1 = internal 4,5V , 0 = internal 2,5V)", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppTemp", "Temperature Sensor power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enTemp", "Enable Temperature Sensor [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppBg", "BandGap power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enBg", "Enable BandGap [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThresholdDac1", "Enable DAC charge [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThresholdDac1", "DAC charge power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThresholdDac2", "Enable DAC time [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThresholdDac2", "DAC time power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_threshold1", "10-bit DAC1 (MSB-LSB) – Charge discriminator threshold", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_threshold2", "10-bit DAC2 (MSB-LSB) – Time discriminator threshold", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enHgOtaQ", "Enable High Gain OTA [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppHgOtaQ", "High Gain OTA power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enLgOtaQ", "Enable Low Gain OTA [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppLgOtaQ", "Low Gain OTA power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enProbeOtaQ", "Enable Probe OTA [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppProbeOtaQ", "Probe OTA power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_testBitOtaQ", "Output OTA buffer bias automatic off [0 : auto-bias – 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enValEvtReceiver", "Enable Val_Evt receiver [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppValEvtReceiver", "Val_Evt receiver power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enRazChnReceiver", "Enable Raz_Chn receiver [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppRazChnReceiver", "Raz Chn receiver power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enDigitalMuxOutput", " Enable digital multiplexed output (hit mux out) [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enOr32", "Enable digital OR32 output [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enNor32Oc", "Enable digital OR32 Open Collector output [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_triggerPolarity", "Output trigger polarity choice [0 : positive (rising edge) – 1 : negative (falling edge)]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enNor32TOc", "Enable digital OR32_T Open Collector output [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enTriggersOutput", "Enable 32 channels triggers outputs [0 : disabled, force off – 1 : Enabled]", SciSDK_Paramcb::Type::U32, this);

}

NI_RESULT SciSDK_DT5550W_Citiroc::ISetParamU32(string name, uint32_t value)
{
	bool param_found = false;

	for (int i = 0; i < NbChannels; i++) {
		if (name == "sc_calibDacT_" + to_string(i)) {
			sc_calibDacT[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_calibDacQ_" + to_string(i)) {
			sc_calibDacQ[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_mask_" + to_string(i)) {
			sc_mask[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_inputDac_" + to_string(i)) {
			sc_inputDac[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_cmdInputDac_" + to_string(i)) {
			sc_cmdInputDac[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_paHgGain_" + to_string(i)) {
			sc_paHgGain[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_paLgGain_" + to_string(i)) {
			sc_paLgGain[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_CtestHg_" + to_string(i)) {
			sc_CtestHg[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_CtestLg_" + to_string(i)) {
			sc_CtestLg[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_enPa_" + to_string(i)) {
			sc_enPa[i] = (int)value;
			param_found = true;
			break;
		}
		else {
			param_found = false;
		}
	}

	if (!param_found) {
		if (name == "sc_ppThHg") {
			sc_ppThHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enDiscri") {
			sc_enDiscri = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppDiscri") {
			sc_ppDiscri = (int)value;
			param_found = true;
		}
		else if (name == "sc_latchDiscri") {
			sc_latchDiscri = (int)value;
			param_found = true;
		}
		else if (name == "sc_enDiscriT") {
			sc_enDiscriT = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppDiscriT") {
			sc_ppDiscriT = (int)value;
			param_found = true;
		}
		else if (name == "sc_enCalibDacQ") {
			sc_enCalibDacQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppCalibDacQ") {
			sc_ppCalibDacQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_enCalibDacT") {
			sc_enCalibDacT = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppCalibDacT") {
			sc_ppCalibDacT = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppThHg") {
			sc_ppThHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enThHg") {
			sc_enThHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppThLg") {
			sc_ppThLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enThLg") {
			sc_enThLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_biasSca") {
			sc_biasSca = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppPdetHg") {
			sc_ppPdetHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enPdetHg") {
			sc_enPdetHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppPdetLg") {
			sc_ppPdetLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enPdetLg") {
			sc_enPdetLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_scaOrPdHg") {
			sc_scaOrPdHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_scaOrPdLg") {
			sc_scaOrPdLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_bypassPd") {
			sc_bypassPd = (int)value;
			param_found = true;
		}
		else if (name == "sc_selTrigExtPd") {
			sc_selTrigExtPd = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppFshBuffer") {
			sc_ppFshBuffer = (int)value;
			param_found = true;
		}
		else if (name == "sc_enFsh") {
			sc_enFsh = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppFsh") {
			sc_ppFsh = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppSshLg") {
			sc_ppSshLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enSshLg") {
			sc_enSshLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_shapingTimeLg") {
			sc_shapingTimeLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppSshHg") {
			sc_ppSshHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enSshHg") {
			sc_enSshHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_shapingTimeHg") {
			sc_shapingTimeHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_paLgBias") {
			sc_paLgBias = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppPaHg") {
			sc_ppPaHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enPaHg") {
			sc_enPaHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppPaLg") {
			sc_ppPaLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enPaLg") {
			sc_enPaLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_fshOnLg") {
			sc_fshOnLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enInputDac") {
			sc_enInputDac = (int)value;
			param_found = true;
		}
		else if (name == "sc_dacRef") {
			sc_dacRef = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppTemp") {
			sc_ppTemp = (int)value;
			param_found = true;
		}
		else if (name == "sc_enTemp") {
			sc_enTemp = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppBg") {
			sc_ppBg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enBg") {
			sc_enBg = (int)value;
			param_found = true;
		}
		else if (name == "sc_enThresholdDac1") {
			sc_enThresholdDac1 = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppThresholdDac1") {
			sc_ppThresholdDac1 = (int)value;
			param_found = true;
		}
		else if (name == "sc_enThresholdDac2") {
			sc_enThresholdDac2 = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppThresholdDac2") {
			sc_ppThresholdDac2 = (int)value;
			param_found = true;
		}
		else if (name == "sc_threshold1") {
			sc_threshold1 = (int)value;
			param_found = true;
		}
		else if (name == "sc_threshold2") {
			sc_threshold2 = (int)value;
			param_found = true;
		}
		else if (name == "sc_enHgOtaQ") {
			sc_enHgOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppHgOtaQ") {
			sc_ppHgOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_enLgOtaQ") {
			sc_enLgOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppLgOtaQ") {
			sc_ppLgOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_enProbeOtaQ") {
			sc_enProbeOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppProbeOtaQ") {
			sc_ppProbeOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_testBitOtaQ") {
			sc_testBitOtaQ = (int)value;
			param_found = true;
		}
		else if (name == "sc_enValEvtReceiver") {
			sc_enValEvtReceiver = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppValEvtReceiver") {
			sc_ppValEvtReceiver = (int)value;
			param_found = true;
		}
		else if (name == "sc_enRazChnReceiver") {
			sc_enRazChnReceiver = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppRazChnReceiver") {
			sc_ppRazChnReceiver = (int)value;
			param_found = true;
		}
		else if (name == "sc_enDigitalMuxOutput") {
			sc_enDigitalMuxOutput = (int)value;
			param_found = true;
		}
		else if (name == "sc_enOr32") {
			sc_enOr32 = (int)value;
			param_found = true;
		}
		else if (name == "sc_enNor32Oc") {
			sc_enNor32Oc = (int)value;
			param_found = true;
		}
		else if (name == "sc_triggerPolarity") {
			sc_triggerPolarity = (int)value;
			param_found = true;
		}
		else if (name == "sc_enNor32TOc") {
			sc_enNor32TOc = (int)value;
			param_found = true;
		}
		else if (name == "sc_enTriggersOutput") {
			sc_enTriggersOutput = (int)value;
			param_found = true;
		}
		else {
			param_found = false;
		}
	}


	if (param_found) {
		return NI_OK;
	}
	else {
		return NI_INVALID_PARAMETER;
	}
}

NI_RESULT SciSDK_DT5550W_Citiroc::ISetParamI32(string name, int32_t value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_Citiroc::ISetParamString(string name, string value)
{
	if (name == "bitstream") {
		bitstream = value;
		return NI_OK;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_Citiroc::IGetParamU32(string name, uint32_t * value)
{
	bool param_found = false;

	for (int i = 0; i < NbChannels; i++) {
		if (name == "sc_calibDacT_" + to_string(i)) {
			*value = (uint32_t)sc_calibDacT[i];
			param_found = true;
			break;
		}
		else if (name == "sc_calibDacQ_" + to_string(i)) {
			*value = (uint32_t)sc_calibDacQ[i];
			param_found = true;
			break;
		}
		else if (name == "sc_mask_" + to_string(i)) {
			*value = (uint32_t)sc_mask[i];
			param_found = true;
			break;
		}
		else if (name == "sc_inputDac_" + to_string(i)) {
			*value = (uint32_t)sc_inputDac[i];
			param_found = true;
			break;
		}
		else if (name == "sc_cmdInputDac_" + to_string(i)) {
			*value = (uint32_t)sc_cmdInputDac[i];
			param_found = true;
			break;
		}
		else if (name == "sc_paHgGain_" + to_string(i)) {
			*value = (uint32_t)sc_paHgGain[i];
			param_found = true;
			break;
		}
		else if (name == "sc_paLgGain_" + to_string(i)) {
			*value = (uint32_t)sc_paLgGain[i];
			param_found = true;
			break;
		}
		else if (name == "sc_CtestHg_" + to_string(i)) {
			*value = (uint32_t)sc_CtestHg[i];
			param_found = true;
			break;
		}
		else if (name == "sc_CtestLg_" + to_string(i)) {
			*value = (uint32_t)sc_CtestLg[i];
			param_found = true;
			break;
		}
		else if (name == "sc_enPa_" + to_string(i)) {
			*value = (uint32_t)sc_enPa[i];
			param_found = true;
			break;
		}
		else {
			param_found = false;
		}
	}

	if (!param_found) {
		if (name == "sc_ppThHg") {
			*value = (uint32_t)sc_ppThHg;
			param_found = true;
		}
		else if (name == "sc_enDiscri") {
			*value = (uint32_t)sc_enDiscri;
			param_found = true;
		}
		else if (name == "sc_ppDiscri") {
			*value = (uint32_t)sc_ppDiscri;
			param_found = true;
		}
		else if (name == "sc_latchDiscri") {
			*value = (uint32_t)sc_latchDiscri;
			param_found = true;
		}
		else if (name == "sc_enDiscriT") {
			*value = (uint32_t)sc_enDiscriT;
			param_found = true;
		}
		else if (name == "sc_ppDiscriT") {
			*value = (uint32_t)sc_ppDiscriT;
			param_found = true;
		}
		else if (name == "sc_enCalibDacQ") {
			*value = (uint32_t)sc_enCalibDacQ;
			param_found = true;
		}
		else if (name == "sc_ppCalibDacQ") {
			*value = (uint32_t)sc_ppCalibDacQ;
			param_found = true;
		}
		else if (name == "sc_enCalibDacT") {
			*value = (uint32_t)sc_enCalibDacT;
			param_found = true;
		}
		else if (name == "sc_ppCalibDacT") {
			*value = (uint32_t)sc_ppCalibDacT;
			param_found = true;
		}
		else if (name == "sc_ppThHg") {
			*value = (uint32_t)sc_ppThHg;
			param_found = true;
		}
		else if (name == "sc_enThHg") {
			*value = (uint32_t)sc_enThHg;
			param_found = true;
		}
		else if (name == "sc_ppThLg") {
			*value = (uint32_t)sc_ppThLg;
			param_found = true;
		}
		else if (name == "sc_enThLg") {
			*value = (uint32_t)sc_enThLg;
			param_found = true;
		}
		else if (name == "sc_biasSca") {
			*value = (uint32_t)sc_biasSca;
			param_found = true;
		}
		else if (name == "sc_ppPdetHg") {
			*value = (uint32_t)sc_ppPdetHg;
			param_found = true;
		}
		else if (name == "sc_enPdetHg") {
			*value = (uint32_t)sc_enPdetHg;
			param_found = true;
		}
		else if (name == "sc_ppPdetLg") {
			*value = (uint32_t)sc_ppPdetLg;
			param_found = true;
		}
		else if (name == "sc_enPdetLg") {
			*value = (uint32_t)sc_enPdetLg;
			param_found = true;
		}
		else if (name == "sc_scaOrPdHg") {
			*value = (uint32_t)sc_scaOrPdHg;
			param_found = true;
		}
		else if (name == "sc_scaOrPdLg") {
			*value = (uint32_t)sc_scaOrPdLg;
			param_found = true;
		}
		else if (name == "sc_bypassPd") {
			*value = (uint32_t)sc_bypassPd;
			param_found = true;
		}
		else if (name == "sc_selTrigExtPd") {
			*value = (uint32_t)sc_selTrigExtPd;
			param_found = true;
		}
		else if (name == "sc_ppFshBuffer") {
			*value = (uint32_t)sc_ppFshBuffer;
			param_found = true;
		}
		else if (name == "sc_enFsh") {
			*value = (uint32_t)sc_enFsh;
			param_found = true;
		}
		else if (name == "sc_ppFsh") {
			*value = (uint32_t)sc_ppFsh;
			param_found = true;
		}
		else if (name == "sc_ppSshLg") {
			*value = (uint32_t)sc_ppSshLg;
			param_found = true;
		}
		else if (name == "sc_enSshLg") {
			*value = (uint32_t)sc_enSshLg;
			param_found = true;
		}
		else if (name == "sc_shapingTimeLg") {
			*value = (uint32_t)sc_shapingTimeLg;
			param_found = true;
		}
		else if (name == "sc_ppSshHg") {
			*value = (uint32_t)sc_ppSshHg;
			param_found = true;
		}
		else if (name == "sc_enSshHg") {
			*value = (uint32_t)sc_enSshHg;
			param_found = true;
		}
		else if (name == "sc_shapingTimeHg") {
			*value = (uint32_t)sc_shapingTimeHg;
			param_found = true;
		}
		else if (name == "sc_paLgBias") {
			*value = (uint32_t)sc_paLgBias;
			param_found = true;
		}
		else if (name == "sc_ppPaHg") {
			*value = (uint32_t)sc_ppPaHg;
			param_found = true;
		}
		else if (name == "sc_enPaHg") {
			*value = (uint32_t)sc_enPaHg;
			param_found = true;
		}
		else if (name == "sc_ppPaLg") {
			*value = (uint32_t)sc_ppPaLg;
			param_found = true;
		}
		else if (name == "sc_enPaLg") {
			*value = (uint32_t)sc_enPaLg;
			param_found = true;
		}
		else if (name == "sc_fshOnLg") {
			*value = (uint32_t)sc_fshOnLg;
			param_found = true;
		}
		else if (name == "sc_enInputDac") {
			*value = (uint32_t)sc_enInputDac;
			param_found = true;
		}
		else if (name == "sc_dacRef") {
			*value = (uint32_t)sc_dacRef;
			param_found = true;
		}
		else if (name == "sc_ppTemp") {
			*value = (uint32_t)sc_ppTemp;
			param_found = true;
		}
		else if (name == "sc_enTemp") {
			*value = (uint32_t)sc_enTemp;
			param_found = true;
		}
		else if (name == "sc_ppBg") {
			*value = (uint32_t)sc_ppBg;
			param_found = true;
		}
		else if (name == "sc_enBg") {
			*value = (uint32_t)sc_enBg;
			param_found = true;
		}
		else if (name == "sc_enThresholdDac1") {
			*value = (uint32_t)sc_enThresholdDac1;
			param_found = true;
		}
		else if (name == "sc_ppThresholdDac1") {
			*value = (uint32_t)sc_ppThresholdDac1;
			param_found = true;
		}
		else if (name == "sc_enThresholdDac2") {
			*value = (uint32_t)sc_enThresholdDac2;
			param_found = true;
		}
		else if (name == "sc_ppThresholdDac2") {
			*value = (uint32_t)sc_ppThresholdDac2;
			param_found = true;
		}
		else if (name == "sc_threshold1") {
			*value = (uint32_t)sc_threshold1;
			param_found = true;
		}
		else if (name == "sc_threshold2") {
			*value = (uint32_t)sc_threshold2;
			param_found = true;
		}
		else if (name == "sc_enHgOtaQ") {
			*value = (uint32_t)sc_enHgOtaQ;
			param_found = true;
		}
		else if (name == "sc_ppHgOtaQ") {
			*value = (uint32_t)sc_ppHgOtaQ;
			param_found = true;
		}
		else if (name == "sc_enLgOtaQ") {
			*value = (uint32_t)sc_enLgOtaQ;
			param_found = true;
		}
		else if (name == "sc_ppLgOtaQ") {
			*value = (uint32_t)sc_ppLgOtaQ;
			param_found = true;
		}
		else if (name == "sc_enProbeOtaQ") {
			*value = (uint32_t)sc_enProbeOtaQ;
			param_found = true;
		}
		else if (name == "sc_ppProbeOtaQ") {
			*value = (uint32_t)sc_ppProbeOtaQ;
			param_found = true;
		}
		else if (name == "sc_testBitOtaQ") {
			*value = (uint32_t)sc_testBitOtaQ;
			param_found = true;
		}
		else if (name == "sc_enValEvtReceiver") {
			*value = (uint32_t)sc_enValEvtReceiver;
			param_found = true;
		}
		else if (name == "sc_ppValEvtReceiver") {
			*value = (uint32_t)sc_ppValEvtReceiver;
			param_found = true;
		}
		else if (name == "sc_enRazChnReceiver") {
			*value = (uint32_t)sc_enRazChnReceiver;
			param_found = true;
		}
		else if (name == "sc_ppRazChnReceiver") {
			*value = (uint32_t)sc_ppRazChnReceiver;
			param_found = true;
		}
		else if (name == "sc_enDigitalMuxOutput") {
			*value = (uint32_t)sc_enDigitalMuxOutput;
			param_found = true;
		}
		else if (name == "sc_enOr32") {
			*value = (uint32_t)sc_enOr32;
			param_found = true;
		}
		else if (name == "sc_enNor32Oc") {
			*value = (uint32_t)sc_enNor32Oc;
			param_found = true;
		}
		else if (name == "sc_triggerPolarity") {
			*value = (uint32_t)sc_triggerPolarity;
			param_found = true;
		}
		else if (name == "sc_enNor32TOc") {
			*value = (uint32_t)sc_enNor32TOc;
			param_found = true;
		}
		else if (name == "sc_enTriggersOutput") {
			*value = (uint32_t)sc_enTriggersOutput;
			param_found = true;
		}
		else {
			param_found = false;
		}
	}

	if (param_found) {
		return NI_OK;
	}
	else {
		return NI_INVALID_PARAMETER;
	}
}

NI_RESULT SciSDK_DT5550W_Citiroc::IGetParamI32(string name, int32_t * value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_Citiroc::IGetParamString(string name, string *value)
{
	if (name == "bitstream") {
		*value = bitstream;
		return NI_OK;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_Citiroc::ExecuteCommand(string cmd, string param)
{
	if (cmd == "generate_bit_config") {
		return CmdGenerateBitConfig();
	}
	else if (cmd == "decode_bit_config") {
		return CmdDecodeBitConfig();
	}

	return NI_INVALID_COMMAND;
}

//method that reverse a string
string strRev(string string) {
	reverse(string.begin(), string.end());
	return string;
}

//method that convert an int value to his binary representation, value is number to convert and len is the length of output binary number
string intToBin(int value, int len) {
	// return (len > 1 ? intToBin(value >> 1, len - 1):null) + "01"[value & 1];
	return (len > 1 ? intToBin(value >> 1, len - 1) : "") + "01"[value & 1];
}

NI_RESULT SciSDK_DT5550W_Citiroc::CmdGenerateBitConfig()
{
	string strSC = "";

	for (int i = 0; i < NbChannels; i++) {
		strSC += strRev(intToBin(sc_calibDacT[i], 4));
	}

	for (int i = 0; i < NbChannels; i++) {
		strSC += strRev(intToBin(sc_calibDacQ[i], 4));
	}

	strSC += to_string(sc_enDiscri)
		+ to_string(sc_ppDiscri)
		+ to_string(sc_latchDiscri)
		+ to_string(sc_enDiscriT)
		+ to_string(sc_ppDiscriT)
		+ to_string(sc_enCalibDacQ)
		+ to_string(sc_enCalibDacQ)
		+ to_string(sc_enCalibDacT)
		+ to_string(sc_enCalibDacT);

	for (int i = 0; i < NbChannels; i++)
		strSC += to_string(sc_mask[i]);

	strSC += to_string(sc_ppThHg)
		+ to_string(sc_enThHg)
		+ to_string(sc_ppThLg)
		+ to_string(sc_enThLg)
		+ to_string(sc_biasSca)
		+ to_string(sc_ppPdetHg)
		+ to_string(sc_enPdetHg)
		+ to_string(sc_ppPdetLg)
		+ to_string(sc_enPdetLg)
		+ to_string(sc_scaOrPdHg)
		+ to_string(sc_scaOrPdLg)
		+ to_string(sc_bypassPd)
		+ to_string(sc_selTrigExtPd)
		+ to_string(sc_ppFshBuffer)
		+ to_string(sc_enFsh)
		+ to_string(sc_ppFsh)
		+ to_string(sc_ppSshLg)
		+ to_string(sc_enSshLg)
		+ strRev(intToBin(sc_shapingTimeLg, 3))
		+ to_string(sc_ppSshHg)
		+ to_string(sc_enSshHg)
		+ strRev(intToBin(sc_shapingTimeHg, 3))
		+ to_string(sc_paLgBias)
		+ to_string(sc_ppPaHg)
		+ to_string(sc_enPaHg)
		+ to_string(sc_ppPaLg)
		+ to_string(sc_enPaLg)
		+ to_string(sc_fshOnLg)
		+ to_string(sc_enInputDac)
		+ to_string(sc_dacRef);

	for (int i = 0; i < NbChannels; i++)
		strSC += intToBin(sc_inputDac[i], 8) + to_string(sc_cmdInputDac[i]);

	for (int i = 0; i < NbChannels; i++)
		strSC += intToBin(sc_paHgGain[i], 6) + intToBin(sc_paLgGain[i], 6) + to_string(sc_CtestHg[i]) + to_string(sc_CtestLg[i]) + to_string(sc_enPa[i]);


	strSC += to_string(sc_ppTemp)
		+ to_string(sc_enTemp)
		+ to_string(sc_ppBg)
		+ to_string(sc_enBg)
		+ to_string(sc_enThresholdDac1)
		+ to_string(sc_ppThresholdDac1)
		+ to_string(sc_enThresholdDac2)
		+ to_string(sc_ppThresholdDac2)
		+ intToBin(sc_threshold1, 10)
		+ intToBin(sc_threshold2, 10)
		+ to_string(sc_enHgOtaQ)
		+ to_string(sc_ppHgOtaQ)
		+ to_string(sc_enLgOtaQ)
		+ to_string(sc_ppLgOtaQ)
		+ to_string(sc_enProbeOtaQ)
		+ to_string(sc_ppProbeOtaQ)
		+ to_string(sc_testBitOtaQ)
		+ to_string(sc_enValEvtReceiver)
		+ to_string(sc_ppValEvtReceiver)
		+ to_string(sc_enRazChnReceiver)
		+ to_string(sc_ppRazChnReceiver)
		+ to_string(sc_enDigitalMuxOutput)
		+ to_string(sc_enOr32)
		+ to_string(sc_enNor32Oc)
		+ to_string(sc_triggerPolarity)
		+ to_string(sc_enNor32TOc)
		+ to_string(sc_enTriggersOutput);

	//cout << strSC << endl;
	bitstream = strSC;
	cout << bitstream << endl;
	return NI_OK;
}

//method that convert a binary number to his decimal representation
int binToInt(string bin) {
	int num = 0;
	for (int i = 0; i < bin.length(); i++) {
		num += bin[i] == '1' ? pow(2, (bin.length() - i - 1)) : 0;
	}
	return num;
}

//command to decode bitconfig string
NI_RESULT SciSDK_DT5550W_Citiroc::CmdDecodeBitConfig()
{
	if (bitstream.length() == 1144) {

		int base_pos = 0;

		for (int i = 0; i < NbChannels; i++) {
			string number = bitstream.substr(base_pos, 4);
			sc_calibDacT[i] = binToInt(strRev(number));
			base_pos += 4;
		}

		for (int i = 0; i < NbChannels; i++) {
			string number = bitstream.substr(base_pos, 4);
			sc_calibDacQ[i] = binToInt(strRev(number));
			base_pos += 4;
		}

		/*	strSC += to_string(sc_enDiscri)
				+ to_string(sc_ppDiscri)
				+ to_string(sc_latchDiscri)
				+ to_string(sc_enDiscriT)
				+ to_string(sc_ppDiscriT)
				+ to_string(sc_enCalibDacQ)
				+ to_string(sc_enCalibDacQ)
				+ to_string(sc_enCalibDacT)
				+ to_string(sc_enCalibDacT);*/

		sc_enDiscri = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppDiscri = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_latchDiscri = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enDiscriT = bitstream[base_pos++] == '1' ? 1 : 0;

		return NI_OK;
	}
	else {
		return NI_SPECIFIC_ERROR;
	}
}
