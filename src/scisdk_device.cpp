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
				BuildTree(jcfg, "");
				NI_RESULT res = _hal->Connect(_DevicePath, _DeviceModel);

				if (res == NI_OK) {
					connected = true;
				}
				else {
					return res;
				}
				//SetRegister("/Registers/res", 1);
				//SetRegister("/Registers/res", 0);
				//SetParameter("/MMCComponents/Oscilloscope_0.trigger_mode", "analog");
				//SetParameter("/MMCComponents/Oscilloscope_0.trigger_level", 3000);
				//SetParameter("/MMCComponents/Oscilloscope_0.pretrigger", 150);
				//SetParameter("/MMCComponents/Oscilloscope_0.decimator", 10);
				//SetParameter("/MMCComponents/Oscilloscope_0.data_processing", "decode");
				////SetParameter("/MMCComponents/Oscilloscope_0.data_processing", "raw");
				//SetParameter("/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
				//SetParameter("/MMCComponents/Oscilloscope_0.timeout", 5000);
				//SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
				//SCISDK_OSCILLOSCOPE_RAW_BUFFER *rb;
				//AllocateBuffer("/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**) &ob);
				//AllocateBuffer("/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);
				//ExecuteCommand("/MMCComponents/Oscilloscope_0.reset_read_valid_flag");
				//
				/*while (1) {
					std::ofstream out("c:/temp/output.txt");
					ReadData("/MMCComponents/Oscilloscope_0", (void *)ob);

					for (int i = 0; i < ob->info.samples_analog; i++) {
						out << ob->analog[i] << endl;
					}
					out.close();
				}*/
				/*while (1) {
					std::ofstream out("c:/temp/output.txt");
					ReadData("/MMCComponents/Oscilloscope_0", (void *)rb);

					for (int i = 0; i < rb->info.buffer_size; i++) {
						out << rb->data[i] << endl;
					}
					out.close();
				}*/

				/*{
				uint32_t v;
				GetRegister("/Registers/cnt", &v);
				std::cout << v << std::endl;
			}*/

			}
			else {
				return NI_CFG_JSON_NOT_FOUND;
			}

		}
		catch (json::exception& e) {
			std::cout << "Configuration file JSON ERROR: " << e.what() << " -- exception id: " << e.id << std::endl;
			return NI_INVALID_CFG_JSON;
		}

		SetParameter("Registers.error", "10");
	}
	return NI_OK;
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

NI_RESULT SciSDK_Device::LocateParameter(string Path, string *name, SciSDK_Node **node) {
	vector<string> qPP = SplitPath(Path, '.');
	if (qPP.size() != 2) return NI_INVALID_PATH;
	string path = qPP[0];
	*name = qPP[1];
	*node = FindMMC(path);
	if (!*node) return NI_NOT_FOUND;
	return NI_OK;
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
	reg = dynamic_cast<SciSDK_Register*> (node);
	reg->SetValueU32(value);
	return NI_OK;
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

			cout << parent << "/" << it.key() << endl;
			if (StartWith(ToUpper(it.key()), ToUpper("Registers"))) {
				for (json r : rs.at(it.key()))
				{
					cout << parent << "/" << it.key() << "/" << (string)r.at("Name") << endl;

					mmcs.push_back(new SciSDK_Register(_hal, r, parent + "/" + (string)it.key()));
				}
			}
			else {
				if (StartWith(ToUpper(it.key()), ToUpper("MMCComponents")) == true) {
					for (json r : rs.at(it.key()))
					{
						cout << parent << "/" << it.key() << "/" << (string)r.at("Name") << "<" << (string)r.at("Type") << ">" << endl;
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
					if (rs.at(it.key()).is_object()) {
						BuildTree(rs.at(it.key()), parent + "/" + it.key());
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