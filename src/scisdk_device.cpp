#include "scisdk_device.h"

#include <fstream>
#include <iostream>

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "scisdk_register.h"
#include "scisdk_oscilloscope.h"
#include "scisdk_digitizer.h"
#include "scisdk_list.h"
#include "scisdk_fftmon.h"
#include "scisdk_spectrum.h"
#include "scisdk_custom_packet.h"
#include "scisdk_rate_meter.h"
#include "scisdk_DT5550W_citirocconfig.h"
#include "scisdk_registers.h"
#include "scisdk_oscilloscope_dual.h"
#include "scisdk_TOFspectrum.h"
#include "scisdk_frame.h"
#include "scisdk_DT5550W_citirocframe.h"
#include "scisdk_DT5550W_petirocframe.h"
#include "boards_driver/bd_feelib.h"
#include "boards_driver/bd_dt1260.h"
#include "boards_driver/bd_dt4810.h"
#include "boards_driver/bd_dt5560se.h"
#include "boards_driver/bd_dt5771.h"

void toLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

SciSDK_Device::SciSDK_Device(string DevicePath, string DeviceModel, string JSONFwFilePath, string Name) {
	_DevicePath = DevicePath;
	_DeviceModel = DeviceModel;
	_JSONFwFilePath = JSONFwFilePath;
	_Name = Name;
	connected = false;
}
SciSDK_Device::~SciSDK_Device() {
	connected = false;
}

NI_RESULT SciSDK_Device::Connect() {
	if (connected) return NI_ALREADY_CONNECTED;
	if (_JSONFwFilePath != "") {

		try {
			std::ifstream ifile(_JSONFwFilePath);
			if (ifile) {
				ifile >> jcfg;
				_hal = new SciSDK_HAL();

				//should verify here that json DeviceModel is the same as the one passed in the constructor
				
				
				BuildTree(jcfg, "");
				NI_RESULT res = _hal->Connect(_DevicePath, _DeviceModel);

				if (res == NI_OK) {
					connected = true;
				}
				else {
					return res;
				}
				

			}
			else {
				return NI_CFG_JSON_NOT_FOUND;
			}

		}
		catch (json::exception& e) {
			std::cout << "Configuration file JSON ERROR: " << e.what() << " -- exception id: " << e.id << std::endl;
			return NI_INVALID_CFG_JSON;
		}


	}
	return NI_OK;
}

NI_RESULT SciSDK_Device::CloseConnection()
{
	if (!connected) return NI_ERROR;

	// stop components threads
	for (auto component : mmcs) {
		component->Detach();
	}

	return _hal->CloseConnection();
}


string ToUpper(string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

bool StartWith(string str, string starts) {
	if (str.rfind(starts, 0) == 0) {
		return true;
	}
	else {
		return false;
	}
}

NI_RESULT SciSDK_Device::LocateParameter(string Path, string* name, SciSDK_Node** node) {
	//check if Path starts with boardapi
	if (StartWith(Path, "/boardapi/")) {
		*name = Path.substr(10);
		*node = FindBoardApi(Path);
		if (!*node) return NI_NOT_FOUND;
		return NI_OK;
	}
	else {
		vector<string> qPP = SplitPath(Path, '.');
		if (qPP.size() != 2) return NI_INVALID_PATH;
		string path = qPP[0];
		*name = qPP[1];
		*node = FindMMC(path);
		if (!*node) return NI_NOT_FOUND;
		return NI_OK;
	}
}

NI_RESULT SciSDK_Device::SetParameter(string Path, string value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->SetParam(name, value);
}
NI_RESULT SciSDK_Device::GetParameter(string Path, string *value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->GetParam(name, value);
}
NI_RESULT SciSDK_Device::SetParameter(string Path, uint32_t value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->SetParam(name, value);
}
NI_RESULT SciSDK_Device::GetParameter(string Path, uint32_t *value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->GetParam(name, value);
}
NI_RESULT SciSDK_Device::SetParameter(string Path, int32_t value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->SetParam(name, value);
}
NI_RESULT SciSDK_Device::GetParameter(string Path, int32_t *value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->GetParam(name, value);
}
NI_RESULT SciSDK_Device::SetParameter(string Path, uint64_t value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->SetParam(name, value);
}
NI_RESULT SciSDK_Device::GetParameter(string Path, uint64_t *value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->GetParam(name, value);
}
NI_RESULT SciSDK_Device::SetParameter(string Path, int64_t value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->SetParam(name, value);
}
NI_RESULT SciSDK_Device::GetParameter(string Path, int64_t *value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->GetParam(name, value);
}
NI_RESULT SciSDK_Device::SetParameter(string Path, double value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->SetParam(name, value);
}
NI_RESULT SciSDK_Device::GetParameter(string Path, double *value) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->GetParam(name, value);
}

NI_RESULT SciSDK_Device::SetRegister(string Path, uint32_t value) {
	SciSDK_Node *node = NULL;
	SciSDK_Register *reg = NULL;
	node = FindMMC(Path);
	if (node != NULL) {
		reg = dynamic_cast<SciSDK_Register*> (node);
		reg->SetValueU32(value);
		return NI_OK;
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Device::GetRegister(string Path, uint32_t *value) {
	SciSDK_Node *node = NULL;
	SciSDK_Register *reg = NULL;
	node = FindMMC(Path);
	reg = dynamic_cast<SciSDK_Register*> (node);
	reg->GetValueU32(value);
	return NI_OK;
}

NI_RESULT SciSDK_Device::AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer) {
	SciSDK_Node *node = NULL;
	node = FindMMC(Path);
	if (!node) return NI_NOT_FOUND;
	return node->AllocateBuffer(bt, buffer);
}

NI_RESULT SciSDK_Device::AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer, int size) {
	SciSDK_Node *node = NULL;
	node = FindMMC(Path);
	if (!node) return NI_NOT_FOUND;
	return node->AllocateBuffer(bt, buffer, size);
}

NI_RESULT SciSDK_Device::FreeBuffer(string Path, T_BUFFER_TYPE bt, void **buffer) {
	SciSDK_Node *node = NULL;
	node = FindMMC(Path);
	if (!node) return NI_NOT_FOUND;
	return node->FreeBuffer(bt, buffer);
}

NI_RESULT SciSDK_Device::ReadData(string Path, void *buffer) {
	SciSDK_Node *node = NULL;
	node = FindMMC(Path);
	if (!node) return NI_NOT_FOUND;
	return node->ReadData(buffer);
}

NI_RESULT SciSDK_Device::WriteData(string Path, void* buffer) {
	SciSDK_Node* node = NULL;
	node = FindMMC(Path);
	if (!node) return NI_NOT_FOUND;
	return node->WriteData(buffer);
}

NI_RESULT SciSDK_Device::ReadStatus(string Path, void *buffer){
	SciSDK_Node *node = NULL;
	node = FindMMC(Path);
	if (!node) return NI_NOT_FOUND;
	return node->ReadStatus(buffer);
}

NI_RESULT SciSDK_Device::GetComponentList(string Type, string * res, bool return_json)
{
	if (!return_json) {
		*res = "";
		for (int i = 0; i < mmcs.size(); i++) {
			if (ToUpper(mmcs.at(i)->GetType()) == ToUpper(Type) || ToUpper(Type) == "ALL") {
				*res += mmcs.at(i)->GetName() + ";";
			}
		}
	}
	else {
		json json_array = json::array();
		for (int i = 0; i < mmcs.size(); i++) {
			if (ToUpper(mmcs.at(i)->GetType()) == ToUpper(Type) || ToUpper(Type) == "ALL") {
				json json_object = json::object();
				json_object["name"] = mmcs.at(i)->GetName();
				json_object["path"] = this->_Name + ":" + mmcs.at(i)->GetPath();
				json_object["type"] = mmcs.at(i)->GetType();
				json_array.push_back(json_object);
			}
		}

		*res = json_array.dump();
	}
	return NI_OK;
}

NI_RESULT SciSDK_Device::GetAllParameters(string Path, string * ret)
{
	vector<string> splitted_path = SplitPath(Path, '/');
	if (splitted_path.size() == 3) {
		for (int i = 0; i < mmcs.size(); i++) {
			if (mmcs.at(i)->GetName() == splitted_path.at(2)) {
				return mmcs.at(i)->GetAllParameters(ret);
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Device::GetParameterDescription(string Path, string * ret)
{
	vector<string> splitted_path = SplitPath(Path, '/');
	if (splitted_path.size() == 3) {
		vector<string> splitted_name = SplitPath(splitted_path.at(2), '.');
		if (splitted_name.size() == 2) {
			for (int i = 0; i < mmcs.size(); i++) {
				if (mmcs.at(i)->GetName() == splitted_name.at(0)) {
					return mmcs.at(i)->GetParameterDescription(splitted_name.at(1), ret);
				}
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Device::GetParameterListOfValues(string Path, string * ret)
{
	vector<string> splitted_path = SplitPath(Path, '/');
	if (splitted_path.size() == 3) {
		vector<string> splitted_name = SplitPath(splitted_path.at(2), '.');
		if (splitted_name.size() == 2) {
			for (int i = 0; i < mmcs.size(); i++) {
				if (mmcs.at(i)->GetName() == splitted_name.at(0)) {
					return mmcs.at(i)->GetParameterListOfValues(splitted_name.at(1), ret);
				}
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Device::GetParameterMinimumValue(string Path, double * ret)
{
	vector<string> splitted_path = SplitPath(Path, '/');
	if (splitted_path.size() == 3) {
		vector<string> splitted_name = SplitPath(splitted_path.at(2), '.');
		if (splitted_name.size() == 2) {
			for (int i = 0; i < mmcs.size(); i++) {
				if (mmcs.at(i)->GetName() == splitted_name.at(0)) {
					return mmcs.at(i)->GetParameterMinimumValue(splitted_name.at(1), ret);
				}
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Device::GetParameterMaximumValue(string Path, double * ret)
{
	vector<string> splitted_path = SplitPath(Path, '/');
	if (splitted_path.size() == 3) {
		vector<string> splitted_name = SplitPath(splitted_path.at(2), '.');
		if (splitted_name.size() == 2) {
			for (int i = 0; i < mmcs.size(); i++) {
				if (mmcs.at(i)->GetName() == splitted_name.at(0)) {
					return mmcs.at(i)->GetParameterMaximumValue(splitted_name.at(1), ret);
				}
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Device::GetParametersProperties(string Path, string * ret)
{
	vector<string> splitted_path = SplitPath(Path, '/');
	if (splitted_path.size() == 3) {
		for (int i = 0; i < mmcs.size(); i++) {
			if (mmcs.at(i)->GetName() == splitted_path.at(2)) {
				return mmcs.at(i)->GetParametersProperties(ret);
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Device::ExecuteCommand(string Path) {
	SciSDK_Node *node = NULL;
	string name;
	int ret;
	if ((ret = LocateParameter(Path, &name, &node)) != 0) return ret;
	return node->ExecuteCommand(name, "");
}

SciSDK_Node * SciSDK_Device::FindMMC(string Path) {
	/*	auto f = std::find(mmcs.begin(), mmcs.end(), Path);
		if (f != mmcs.end()) {
			*node = *f;
			return true;
		}
		else {
			return false;
		}*/
	for (auto n : mmcs) {
		//cout << " ### " << n->GetPath() << endl;
		if ((n->GetPath() == Path))
		{
			return n;
		}
	}
	return NULL;
}

SciSDK_Node* SciSDK_Device::FindBoardApi(string Path) {
	for (auto n : mmcs) {
		//cout << " ### " << n->GetPath() << endl;
		if ((n->GetType() == "boardapi"))
		{
			return n;
		}
	}
	return NULL;
}

std::vector<std::string> SciSDK_Device::SplitPath(string path, char separator) {

	std::stringstream test(path);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, separator))
	{
		seglist.push_back(segment);
	}
	return seglist;

}


NI_RESULT SciSDK_Device::BuildTree(json rs, string parent) {
	try {
		for (json::iterator it = rs.begin(); it != rs.end(); ++it) {

			if (StartWith(ToUpper(it.key()), ToUpper("Registers"))) {
				for (json r : rs.at(it.key()))
				{
					mmcs.push_back(new SciSDK_Register(_hal, r, parent + "/" + (string)it.key()));
				}
			}
			else {
				if (StartWith(ToUpper(it.key()), ToUpper("MMCComponents")) == true) {
					for (json r : rs.at(it.key()))
					{
						if (ToUpper(r.at("Type")) == "OSCILLOSCOPE") {
							mmcs.push_back(new SciSDK_Oscilloscope(_hal, r, parent + "/" + (string)it.key()));
						}
						else 	if (ToUpper(r.at("Type")) == "WAVEDUMP") {
							mmcs.push_back(new SciSDK_Digitizer(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "LISTMODULE") {
							mmcs.push_back(new SciSDK_List(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "FFTMONITOR") {
							mmcs.push_back(new SciSDK_FFT(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "SPECTRUM") {
							mmcs.push_back(new SciSDK_Spectrum(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "CUSTOMPACKET") {
							mmcs.push_back(new SciSDK_CustomPacket(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "MCRATEMETER") {
							mmcs.push_back(new SciSDK_Ratemeter(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "CITIROCCFG") {
							mmcs.push_back(new SciSDK_DT5550W_CitirocConfig(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "REGISTERFILE") {
							mmcs.push_back(new SciSDK_Registers(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "OSCILLOSCOPEDUAL") {
							mmcs.push_back(new SciSDK_Oscilloscope_Dual(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "TOFSPECTRUM") {
							mmcs.push_back(new SciSDK_TOFSpectrum(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "FRAMETRANSFER") {
							mmcs.push_back(new SciSDK_Frame(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "CITIROCFRAME") {
							mmcs.push_back(new SciSDK_DT5550W_CitirocFrame(_hal, r, parent + "/" + (string)it.key()));
						}
						else if (ToUpper(r.at("Type")) == "PETIROCFRAME") {
							mmcs.push_back(new SciSDK_DT5550W_PetirocFrame(_hal, r, parent + "/" + (string)it.key()));
						}
					}
				}
				else {
					if (StartWith(ToUpper(it.key()), ToUpper("Device")) == true) {
						string model = it.value().get<string>();
						toLower(model);
						if ((model == "x2740_wave") || (model == "x2740_dpp") || (model == "x2745_wave") || (model == "x2745_dpp")) {
							mmcs.push_back(new bd_feelib(_hal, rs, parent + "/" + (string)it.key()));
						} else if ((model == "dt1260") || (model == "scidk") ) {
							mmcs.push_back(new bd_dt1260(_hal, rs, parent + "/" + (string)it.key()));
						}
						else if ((model == "dt4810") ) {
							mmcs.push_back(new bd_dt4810(_hal, this, rs, parent + "/" + (string)it.key()));
						}
						else if ((model == "dt5560") || (model == "dt5560se")) {
							mmcs.push_back(new bd_dt5560se(_hal, rs, parent + "/" + (string)it.key()));
						}
						else if (model == "dt5771")  {
							mmcs.push_back(new bd_dt5771(_hal, rs, parent + "/" + (string)it.key()));
						}
					} 
					else {
						if (rs.at(it.key()).is_object()) {
							BuildTree(rs.at(it.key()), parent + "/" + it.key());
						}
					}
				}
					
			}

		}

	}
	catch (json::exception& e) {
		std::cout << "Path Error: " << e.what() << " -- exception id: " << e.id << std::endl;
		return NI_INVALID_PATH;
	}



	return NI_OK;
}

NI_RESULT SciSDK_Device::FindElement(string Path, SciElement *param) {

	vector<string> qPP = SplitPath(Path, '.');
	if (qPP.size() < 1)
		return NI_INVALID_PATH;

	vector<string> qP = SplitPath(qPP[0], '/');
	if (qP.size() < 1)
		return NI_INVALID_PATH;

	try
	{
		json rs = jcfg;
		for (int i = 0; i < qP.size() - 1; i++) {
			rs = rs.at(qP[i]);
		}

		if (qP[qP.size() - 1] == "Registers") {
			rs = rs.at("Registers");
			for (json r : rs) {
				string name = r.at("Name");
				if (name == qPP[1]) {

					param->type = MMC_ELEMENT::REGISTER;
					param->j = r;
					for (int i = 1; i < qPP.size(); i++) {
						param->params.push_back(qPP[i]);
					}
					return NI_OK;
				}
			}
		}
		else if (qP[qP.size() - 1] == "mmc") {
			rs = rs.at("MMCComponents");
			for (json r : rs) {
				string name = r.at("Name");
				if (name == qPP[1]) {
					if (r.at("Type") == "WaveDump")
						param->type = MMC_ELEMENT::WAVEDUMP;
					else if (r.at("Type") == "Oscilloscope")
						param->type = MMC_ELEMENT::OSCILLOSCOPE;
					else
						return NI_INVALID_TYPE;

					param->j = r;
					for (int i = 1; i < qPP.size(); i++) {
						param->params.push_back(qPP[i]);
					}
					return NI_OK;
				}
			}
		}
		else {
			return NI_INVALID_TYPE;
		}

	}
	catch (json::exception& e) {
		std::cout << "Path Error: " << e.what() << " -- exception id: " << e.id << std::endl;
		return NI_INVALID_PATH;
	}

	return NI_NOT_FOUND;
}