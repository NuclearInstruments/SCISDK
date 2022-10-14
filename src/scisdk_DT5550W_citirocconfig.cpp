#include "scisdk_DT5550W_citirocconfig.h"

SciSDK_DT5550W_CitirocConfig::SciSDK_DT5550W_CitirocConfig(SciSDK_HAL * hal, json j, string path) : SciSDK_Node(hal, j, path)
{
	address = (uint32_t)j.at("REG_CFG0");
	address_start = (uint32_t)j.at("START_REG_CFG");

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
		RegisterParameter("sc_mask_" + to_string(i), " Discriminator mask (channel 0 to 31) [0: Masked  1 : unmasked]", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_inputDac_" + to_string(i), "8-bit input DAC Voltage Reference (1 = internal 4,5V , 0 = internal 2,5V) ", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_cmdInputDac_" + to_string(i), "Input 8-bit DAC Data from channel 0 to 31  (DAC7...DAC0 + DAC ON) ", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_paHgGain_" + to_string(i), "High Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_paLgGain_" + to_string(i), "Low Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_CtestHg_" + to_string(i), "CtestHG : connect injection capacitance to preamp", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_CtestLg_" + to_string(i), " CtestLG : connect injection capacitance to preamp", SciSDK_Paramcb::Type::U32, this);
		RegisterParameter("sc_enPa_" + to_string(i), "PA disabled : disable preamplifier [0: preamplifier enabled  1: preamplifier disabled]", SciSDK_Paramcb::Type::U32, this);
	}

	RegisterParameter("sc_enDiscri", "Enable Discriminator Charge [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppDiscri", "discriminator Charge power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_latchDiscri", "Select latched (RS : 1) or direct output (trigger : 0) on charge discriminator", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enDiscriT", "Enable Discriminator Time [0 : disabled, force off  1 : Enabled] ", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppDiscriT", "discriminator Time power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enCalibDacQ", "Enable 4-bit DAC charge [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppCalibDacQ", "4-bit DAC charge power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enCalibDacT", "Enable 4-bit DAC time [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppCalibDacT", "4-bit DAC time power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThHg", "High Gain Track & Hold power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThHg", "Enable High Gain Track & Hold [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThLg", "Low Gain Track & Hold power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThLg", "Enable Low Gain Track & Hold [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_biasSca", "SCA bias ( 1 = weak bias, 0 = high bias 5MHz ReadOut Speed)", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPdetHg", "High Gain Peak detector power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPdetHg", "Enable High Gain Peak detector [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPdetLg", "Low Gain Peak detector power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPdetLg", "Enable Low Gain Peak detector [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_scaOrPdHg", "Select Track & Hold or Peak Detector  High Gain [0: peak detector  1 : T&H]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_scaOrPdLg", " Select Track & Hold or Peak Detector  Low Gain [0: peak detector  1 : T&H]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_bypassPd", "Bypass Peak Sensing Cell [0 : cell active  1 : cell bypassed]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_selTrigExtPd", "Select peak sensing cell trigger [0 : internal trigger  1 : external trigger]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppFshBuffer", "fast shaper follower power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enFsh", "Enable fast shaper [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppFsh", "fast shaper power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppSshLg", "low gain slow shaper power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enSshLg", "Enable Low Gain Slow Shaper [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_shapingTimeLg", "Low gain shaper time constant commands (0..2) [active low]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppSshHg", "high gain slow shaper power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enSshHg", "Enable high gain Slow Shaper [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_shapingTimeHg", "High gain shaper time constant commands (0..2) [active low]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_paLgBias", "Low Gain PreAmp bias [0: normal bias - 1: weak bias]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPaHg", "High Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPaHg", "Enable High Gain preamp [0 : disabled, force off  1 : Enabled] ", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppPaLg", "Low Gain preamp power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enPaLg", "Enable Low Gain preamp [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_fshOnLg", "Select preamp to connect to Fast Shaper [0:fast shaper on HG preamp  1 : fast shaper on LG preamp", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enInputDac", "Enable 32 input 8-bit DACs [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_dacRef", "8-bit input DAC Voltage Reference (1 = internal 4,5V , 0 = internal 2,5V)", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppTemp", "Temperature Sensor power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enTemp", "Enable Temperature Sensor [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppBg", "BandGap power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enBg", "Enable BandGap [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThresholdDac1", "Enable DAC charge [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThresholdDac1", "DAC charge power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enThresholdDac2", "Enable DAC time [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppThresholdDac2", "DAC time power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_threshold1", "10-bit DAC1 (MSB-LSB)  Charge discriminator threshold", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_threshold2", "10-bit DAC2 (MSB-LSB)  Time discriminator threshold", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enHgOtaQ", "Enable High Gain OTA [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppHgOtaQ", "High Gain OTA power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enLgOtaQ", "Enable Low Gain OTA [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppLgOtaQ", "Low Gain OTA power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enProbeOtaQ", "Enable Probe OTA [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppProbeOtaQ", "Probe OTA power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_testBitOtaQ", "Output OTA buffer bias automatic off [0 : auto-bias  1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enValEvtReceiver", "Enable Val_Evt receiver [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppValEvtReceiver", "Val_Evt receiver power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enRazChnReceiver", "Enable Raz_Chn receiver [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_ppRazChnReceiver", "Raz Chn receiver power pulsing mode [0: power pin - 1 : force on]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enDigitalMuxOutput", " Enable digital multiplexed output (hit mux out) [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enOr32", "Enable digital OR32 output [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enNor32Oc", "Enable digital OR32 Open Collector output [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_triggerPolarity", "Output trigger polarity choice [0 : positive (rising edge)  1 : negative (falling edge)]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enNor32TOc", "Enable digital OR32_T Open Collector output [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
	RegisterParameter("sc_enTriggersOutput", "Enable 32 channels triggers outputs [0 : disabled, force off  1 : Enabled]", SciSDK_Paramcb::Type::U32, this);
}

NI_RESULT SciSDK_DT5550W_CitirocConfig::ISetParamU32(string name, uint32_t value)
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
			sc_mask[i] = value == 0 ? 0 : 1;
			param_found = true;
			break;
		}
		else if (name == "sc_inputDac_" + to_string(i)) {
			sc_inputDac[i] = (int)value;
			param_found = true;
			break;
		}
		else if (name == "sc_cmdInputDac_" + to_string(i)) {
			sc_cmdInputDac[i] = value == 0 ? 0 : 1;
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
			sc_CtestHg[i] = value == 0 ? 0 : 1;
			param_found = true;
			break;
		}
		else if (name == "sc_CtestLg_" + to_string(i)) {
			sc_CtestLg[i] = value == 0 ? 0 : 1;
			param_found = true;
			break;
		}
		else if (name == "sc_enPa_" + to_string(i)) {
			sc_enPa[i] = value == 0 ? 0 : 1;
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
			sc_enDiscri = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppDiscri") {
			sc_ppDiscri = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_latchDiscri") {
			sc_latchDiscri = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enDiscriT") {
			sc_enDiscriT = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppDiscriT") {
			sc_ppDiscriT = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enCalibDacQ") {
			sc_enCalibDacQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppCalibDacQ") {
			sc_ppCalibDacQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enCalibDacT") {
			sc_enCalibDacT = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppCalibDacT") {
			sc_ppCalibDacT = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppThHg") {
			sc_ppThHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enThHg") {
			sc_enThHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppThLg") {
			sc_ppThLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enThLg") {
			sc_enThLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_biasSca") {
			sc_biasSca = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppPdetHg") {
			sc_ppPdetHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enPdetHg") {
			sc_enPdetHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppPdetLg") {
			sc_ppPdetLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enPdetLg") {
			sc_enPdetLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_scaOrPdHg") {
			sc_scaOrPdHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_scaOrPdLg") {
			sc_scaOrPdLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_bypassPd") {
			sc_bypassPd = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_selTrigExtPd") {
			sc_selTrigExtPd = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppFshBuffer") {
			sc_ppFshBuffer = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enFsh") {
			sc_enFsh = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppFsh") {
			sc_ppFsh = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppSshLg") {
			sc_ppSshLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enSshLg") {
			sc_enSshLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_shapingTimeLg") {
			sc_shapingTimeLg = (int)value;
			param_found = true;
		}
		else if (name == "sc_ppSshHg") {
			sc_ppSshHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enSshHg") {
			sc_enSshHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_shapingTimeHg") {
			sc_shapingTimeHg = (int)value;
			param_found = true;
		}
		else if (name == "sc_paLgBias") {
			sc_paLgBias = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppPaHg") {
			sc_ppPaHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enPaHg") {
			sc_enPaHg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppPaLg") {
			sc_ppPaLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enPaLg") {
			sc_enPaLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_fshOnLg") {
			sc_fshOnLg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enInputDac") {
			sc_enInputDac = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_dacRef") {
			sc_dacRef = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppTemp") {
			sc_ppTemp = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enTemp") {
			sc_enTemp = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppBg") {
			sc_ppBg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enBg") {
			sc_enBg = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enThresholdDac1") {
			sc_enThresholdDac1 = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppThresholdDac1") {
			sc_ppThresholdDac1 = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enThresholdDac2") {
			sc_enThresholdDac2 = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppThresholdDac2") {
			sc_ppThresholdDac2 = value == 0 ? 0 : 1;
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
			sc_enHgOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppHgOtaQ") {
			sc_ppHgOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enLgOtaQ") {
			sc_enLgOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppLgOtaQ") {
			sc_ppLgOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enProbeOtaQ") {
			sc_enProbeOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppProbeOtaQ") {
			sc_ppProbeOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_testBitOtaQ") {
			sc_testBitOtaQ = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enValEvtReceiver") {
			sc_enValEvtReceiver = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppValEvtReceiver") {
			sc_ppValEvtReceiver = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enRazChnReceiver") {
			sc_enRazChnReceiver = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_ppRazChnReceiver") {
			sc_ppRazChnReceiver = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enDigitalMuxOutput") {
			sc_enDigitalMuxOutput = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enOr32") {
			sc_enOr32 = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enNor32Oc") {
			sc_enNor32Oc = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_triggerPolarity") {
			sc_triggerPolarity = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enNor32TOc") {
			sc_enNor32TOc = value == 0 ? 0 : 1;
			param_found = true;
		}
		else if (name == "sc_enTriggersOutput") {
			sc_enTriggersOutput = value == 0 ? 0 : 1;
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

NI_RESULT SciSDK_DT5550W_CitirocConfig::ISetParamI32(string name, int32_t value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_CitirocConfig::ISetParamString(string name, string value)
{
	if (name == "bitstream") {
		bitstream = value;
		return NI_OK;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_CitirocConfig::IGetParamU32(string name, uint32_t * value)
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

NI_RESULT SciSDK_DT5550W_CitirocConfig::IGetParamI32(string name, int32_t * value)
{
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_CitirocConfig::IGetParamString(string name, string *value)
{
	if (name == "bitstream") {
		*value = bitstream;
		return NI_OK;
	}
	return NI_INVALID_PARAMETER;
}

NI_RESULT SciSDK_DT5550W_CitirocConfig::ExecuteCommand(string cmd, string param)
{
	if (cmd == "generate_bit_config") {
		return CmdGenerateBitConfig();
	}
	else if (cmd == "decode_bit_config") {
		return CmdDecodeBitConfig();
	}
	else if (cmd == "write_bitstream") {
		return CmdWriteBitstream();
	}
	else if (cmd == "read_bitstream") {
		return CmdReadBitstream();
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
	if (value < (pow(2, len) - 1)) {
		return (len > 1 ? intToBin(value >> 1, len - 1) : "") + "01"[value & 1];
	}
	else {
		string ret = "";
		for (int i = 0; i < len; i++) {
			ret += "1";
		}
		return ret;
	}
}

//command to encode bitconfig string
NI_RESULT SciSDK_DT5550W_CitirocConfig::CmdGenerateBitConfig()
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
		+ to_string(sc_ppCalibDacQ)
		+ to_string(sc_enCalibDacT)
		+ to_string(sc_ppCalibDacT);

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

	bitstream = strSC;
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
NI_RESULT SciSDK_DT5550W_CitirocConfig::CmdDecodeBitConfig()
{
	if (bitstream.length() == 1144) {

		int base_pos = 0;
		string str_tmp;

		// decode sc_calibDacT
		for (int i = 0; i < NbChannels; i++) {
			str_tmp = bitstream.substr(base_pos, 4);
			sc_calibDacT[i] = binToInt(strRev(str_tmp));
			base_pos += 4;
		}

		// decode sc_calibDacQ
		for (int i = 0; i < NbChannels; i++) {
			str_tmp = bitstream.substr(base_pos, 4);
			sc_calibDacQ[i] = binToInt(strRev(str_tmp));
			base_pos += 4;
		}

		sc_enDiscri = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppDiscri = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_latchDiscri = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enDiscriT = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppDiscriT = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enCalibDacQ = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppCalibDacQ = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enCalibDacT = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppCalibDacT = bitstream[base_pos++] == '1' ? 1 : 0;

		// decode sc_mask
		for (int i = 0; i < NbChannels; i++) {
			sc_mask[i] = bitstream[base_pos++] == '1' ? 1 : 0;
		}

		sc_ppThHg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enThHg = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_ppThLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enThLg = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_biasSca = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_ppPdetHg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enPdetHg = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_ppPdetLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enPdetLg = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_scaOrPdHg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_scaOrPdLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_bypassPd = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_selTrigExtPd = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_ppFshBuffer = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enFsh = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppFsh = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_ppSshLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enSshLg = bitstream[base_pos++] == '1' ? 1 : 0;

		// decode sc_shapingTimeLg
		str_tmp = bitstream.substr(base_pos, 3);
		sc_shapingTimeLg = binToInt(strRev(str_tmp));
		base_pos += 3;

		sc_ppSshHg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enSshHg = bitstream[base_pos++] == '1' ? 1 : 0;

		// decode sc_shapingTimeHg
		str_tmp = bitstream.substr(base_pos, 3);
		sc_shapingTimeHg = binToInt(strRev(str_tmp));
		base_pos += 3;

		sc_paLgBias = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppPaHg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enPaHg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppPaLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enPaLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_fshOnLg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enInputDac = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_dacRef = bitstream[base_pos++] == '1' ? 1 : 0;

		// decode sc_cmdInputDac
		for (int i = 0; i < NbChannels; i++) {
			str_tmp = bitstream.substr(base_pos, 8);
			sc_inputDac[i] = binToInt(str_tmp);
			base_pos += 8;
			sc_cmdInputDac[i] = bitstream[base_pos++] == '1' ? 1 : 0;
		}

		for (int i = 0; i < NbChannels; i++) {
			// decode sc_paHgGain
			str_tmp = bitstream.substr(base_pos, 6);
			sc_paHgGain[i] = binToInt(str_tmp);
			base_pos += 6;

			// decode sc_paLgGain
			str_tmp = bitstream.substr(base_pos, 6);
			sc_paLgGain[i] = binToInt(str_tmp);
			base_pos += 6;

			sc_CtestHg[i] = bitstream[base_pos++] == '1' ? 1 : 0;
			sc_CtestLg[i] = bitstream[base_pos++] == '1' ? 1 : 0;
			sc_enPa[i] = bitstream[base_pos++] == '1' ? 1 : 0;
		}

		sc_ppTemp = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enTemp = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppBg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enBg = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enThresholdDac1 = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppThresholdDac1 = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enThresholdDac2 = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppThresholdDac2 = bitstream[base_pos++] == '1' ? 1 : 0;

		str_tmp = bitstream.substr(base_pos, 10);
		sc_threshold1 = binToInt(str_tmp);
		base_pos += 10;

		str_tmp = bitstream.substr(base_pos, 10);
		sc_threshold2 = binToInt(str_tmp);
		base_pos += 10;

		sc_enHgOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppHgOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enLgOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppLgOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enProbeOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppProbeOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_testBitOtaQ = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enValEvtReceiver = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppValEvtReceiver = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enRazChnReceiver = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_ppRazChnReceiver = bitstream[base_pos++] == '1' ? 1 : 0;

		sc_enDigitalMuxOutput = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enOr32 = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enNor32Oc = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_triggerPolarity = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enNor32TOc = bitstream[base_pos++] == '1' ? 1 : 0;
		sc_enTriggersOutput = bitstream[base_pos++] == '1' ? 1 : 0;

		return NI_OK;
	}
	else {
		return NI_SPECIFIC_ERROR;
	}
}

// command to write bitstream on device
NI_RESULT SciSDK_DT5550W_CitirocConfig::CmdWriteBitstream()
{
	// check if bitstream is valid (string length == 1144 and string contains only 0 and 1)
	if (bitstream.length() == 1144 && bitstream.find_first_not_of("01")) {
		uint32_t datavector[36];

		bool bitarray[1152];

		for (int i = 0; i < 1152; i++) {
			if (i < 1144) {
				bitarray[i] = bitstream[i] == '1' ? true : false;
			}
			else {
				bitarray[i] = false;
			}
		}

		for (int i = 0; i < 36; i++) {
			datavector[i] = 0;
			for (int j = 0; j < 32; j++) {
				datavector[i] += ((uint32_t)(bitarray[(i * 32) + j] == true ? 1 : 0)) << j;
			}
		}

		// write uint32_t array on device
		int ret = 0;
		for (int i = 0; i < 36; i++) {
			ret |= _hal->WriteReg(datavector[i], address + (uint32_t)i);
		}

		ret |= _hal->WriteReg(0, address_start);
		ret |= _hal->WriteReg(1, address_start);
		if (ret) {
			return NI_ERROR_INTERFACE;
		}
		return NI_OK;
	}
	else {
		return NI_SPECIFIC_ERROR;
	}
}

NI_RESULT SciSDK_DT5550W_CitirocConfig::CmdReadBitstream()
{
	//read data from device
	int ret = 0;
	uint32_t datavector[36];

	for (int i = 0; i < 36; i++) {
		uint32_t value;
		ret |= _hal->ReadReg(&value, address + (uint32_t)i);
		datavector[i] = value;
		if (ret) {
			return NI_ERROR_INTERFACE;
		}
	}

	bool bitarray[1152];

	for (int i = 0; i < 36; i++) {
		bitset<32> value(datavector[i]);
		for (int j = 0; j < 32; j++) {
			bitarray[i] = value[32 - j - 1] == 1 ? true : false;
		}
	}

	// generate bistream
	bitstream = "";
	for (int i = 0; i < 1144; i++) {
		bitstream += bitarray[i] == true ? '1' : '0';
	}

	return NI_OK;
}