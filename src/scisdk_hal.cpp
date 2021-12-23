#include "scisdk_hal.h"
#include "../headers/SCIDK_API_C.h"

NI_RESULT SciSDK_HAL::Connect(string Path, string model) {
	if ((model == "DT1260") || (model == "SCIDK"))
		_model = BOARD_MODEL::DT1260;
	if ((model == "DT5550") || (model == "DT5550W"))
		_model = BOARD_MODEL::DT5550X;
	if ((model == "R5560") || (model == "R5560SE") || (model == "DT5560"))
		_model = BOARD_MODEL::X5560;
	if ((model == "V2495") || (model == "DT2495"))
		_model = BOARD_MODEL::X2495;
	if ((model == "V2740") || (model == "DT2740"))
		_model = BOARD_MODEL::X2495;

	vector<std::string> p = SplitPath(Path, ':');
	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		_handle = malloc(sizeof(SCIDK_HANDLE));
		if (p[0] == "usb") {
			NI_RESULT r = SCIDK_ConnectUSB((char*)p[1].c_str(), (SCIDK_HANDLE*) _handle);
			return r;
		}
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::CloseConnection() {

	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		SCIDK_CloseConnection((SCIDK_HANDLE*) _handle);
		free(_handle);
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::Enumerate(string board_model, vector<string> devices) {
	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::WriteReg(uint32_t value,
	uint32_t address) {

	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		SCIDK_WriteReg(value, address, (SCIDK_HANDLE*)_handle);
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadReg(uint32_t *value,
	uint32_t address) {

	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		SCIDK_ReadReg(value, address, (SCIDK_HANDLE*)_handle);
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::WriteData(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t BusMode,
	uint32_t timeout_ms,
	uint32_t *written_data) {

	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}
	return NI_OK;
}

NI_RESULT SciSDK_HAL::ReadData(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t BusMode,
	uint32_t timeout_ms,
	uint32_t *read_data,
	uint32_t *valid_data) {

	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}

	return NI_OK;
}

string SciSDK_HAL::ReadFirmwareInformationApp() {

	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		break;
	case BOARD_MODEL::DT5550X:
		break;
	case BOARD_MODEL::X5560:
		break;
	case BOARD_MODEL::X2495:
		break;
	case BOARD_MODEL::X2740:
		break;
	default:
		break;
	}

	return NI_OK;
}

std::vector<std::string> SciSDK_HAL::SplitPath(string path, char separator) {
	std::stringstream test(path);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, separator))
	{
		seglist.push_back(segment);
	}
	return seglist;
}
