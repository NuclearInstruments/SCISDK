#include "scisdk_hal.h"
//#include "../headers/SCIDK_API_C.h"

NI_RESULT SciSDK_HAL::Connect(string Path, string model) {
	if ((model == "dt1260") || (model == "SCIDK")) {
		_model = BOARD_MODEL::DT1260;
		//load the correct dll
		h_lib_instance = LoadLibrary(L"SCIDK_LibD.dll");
		//cout << "Last error " << GetLastError() << endl;
	}
	if ((model == "DT5550") || (model == "DT5550W")) {
		_model = BOARD_MODEL::DT5550X;
	}
	if ((model == "R5560") || (model == "R5560SE") || (model == "DT5560")) {
		_model = BOARD_MODEL::X5560;
	}
	if ((model == "V2495") || (model == "DT2495")) {
		_model = BOARD_MODEL::X2495;
	}
	if ((model == "V2740") || (model == "DT2740")) {
		_model = BOARD_MODEL::X2495;
	}

	vector<std::string> p = SplitPath(Path, ':');
	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		_handle = malloc(sizeof(int));
		if (p[0] == "usb") {
			/*_handle = malloc(sizeof(SCISDK::NI_HANDLE));
			if (p[0] == "usb") {
			NI_RESULT r = SCISDK::SCIDK_ConnectUSB((char*)p[1].c_str(), (SCISDK::NI_HANDLE*) _handle);
			return r;
			}*/

			if (h_lib_instance != NULL) {
				//pointer to connect function inside dll file
				CONNECT_PROC_PTR connectUSB = (CONNECT_PROC_PTR)GetProcAddress(h_lib_instance, "SCIDK_ConnectUSB");
				if (connectUSB) {
					NI_RESULT r = connectUSB((char*)p[1].c_str(), (NI_HANDLE*)_handle);
					return r;
				}
			} 
			
			return NI_ERROR;
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
		//SCISDK::NI_CloseConnection((SCISDK::NI_HANDLE*) _handle);
		if (h_lib_instance != NULL) {
			//pointer to connect function inside dll file
			CLOSE_CONNECTION_PROC_PTR close_connection = (CLOSE_CONNECTION_PROC_PTR)GetProcAddress(h_lib_instance, "NI_CloseConnection");
			if (close_connection) {
				NI_RESULT r = close_connection((NI_HANDLE*)_handle);
				FreeLibrary(h_lib_instance);
				return r;
			}
		}
	free(_handle);
		return NI_ERROR;
		
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
		//SCISDK::NI_WriteReg(value, address, (SCISDK::NI_HANDLE*)_handle);
		if (h_lib_instance != NULL) {
			//pointer to connect function inside dll file
			WRITE_REG_PROC_PTR write_reg = (WRITE_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_WriteReg");
			if (write_reg) {
				NI_RESULT r = write_reg(value, address, (NI_HANDLE*)_handle);
				return r;
			}
		}
		return NI_ERROR;
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
		//SCISDK::NI_ReadReg(value, address, (SCISDK::NI_HANDLE*)_handle);
		if (h_lib_instance != NULL) {
			//pointer to connect function inside dll file
			READ_REG_PROC_PTR read_reg = (READ_REG_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadReg");
			if (read_reg) {
				NI_RESULT r =read_reg(value, address, (NI_HANDLE*)_handle);
				return r;
			}
		}
		return NI_ERROR;
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
	uint32_t timeout_ms,
	uint32_t *read_data) {
	uint32_t rd, vd;
	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		/*SCISDK::NI_ReadData(value,
			length,
			address,
			REG_ACCESS,
			timeout_ms,
			(SCISDK::NI_HANDLE*)_handle,
			&rd,
			read_data);*/
		if (h_lib_instance != NULL) {
			//pointer to connect function inside dll file
			READ_DATA_PROC_PTR read_data_proc = (READ_DATA_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadData");
			if (read_data_proc) {
				NI_RESULT r = read_data_proc(value,
					length,
					address,
					REG_ACCESS,
					timeout_ms,
					(NI_HANDLE*)_handle,
					&rd,
					read_data);
				return r;
			}
		}
		return NI_ERROR;
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

NI_RESULT SciSDK_HAL::ReadFIFO(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t addressStatus,
	uint32_t timeout_ms,
	uint32_t *read_data) {
	uint32_t rd, vd;
	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		/*SCISDK::NI_ReadFIFO(value,
			length,
			address,
			STREAMING,
			timeout_ms,
			(SCISDK::NI_HANDLE*)_handle,
			&rd,
			read_data);*/
		if (h_lib_instance != NULL) {
			//pointer to connect function inside dll file
			READ_FIFO_PROC_PTR read_data_proc = (READ_FIFO_PROC_PTR)GetProcAddress(h_lib_instance, "NI_ReadFIFO");
			if (read_data_proc) {
				NI_RESULT r = read_data_proc(value,
					length,
					address,
					STREAMING,
					timeout_ms,
					(NI_HANDLE*)_handle,
					&rd,
					read_data);
				return r;
			}
		}
		return NI_ERROR;
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

NI_RESULT SciSDK_HAL::ReadFIFODMA(uint32_t *value,
	uint32_t length,
	uint32_t address,
	uint32_t addressStatus,
	uint32_t timeout_ms,
	uint32_t *read_data) {
	uint32_t rd, vd;
	switch (_model)
	{
	case BOARD_MODEL::DT1260:
		return NI_NOT_IMPLEMENTED;
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
