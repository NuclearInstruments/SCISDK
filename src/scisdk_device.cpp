#include "scisdk_device.h"


#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
	
#include "scisdk_register.h"

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
				BuildTree(jcfg, "");
				_hal = new SciSDK_HAL();
				NI_RESULT res = _hal->Connect(_DevicePath, _DeviceModel);

				if (res == NI_OK) {
					connected = true;
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

		SetParameter("Page0/Registers.error", "10");
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
NI_RESULT SciSDK_Device::SetParameter(string Path, string value) {
	/*SciElement spm;
	NI_RESULT res = FindElement(Path, &spm);
	if (res != NI_OK) return res;

	if (spm.type == MMC_ELEMENT::REGISTER) {
		uint32_t v = stoul(value);
		SciSDK_Register r(spm, _hal);
		return r.SetRegister(v);
	} else {
			return NI_INVALID_TYPE;
	}*/

	return NI_OK;
}
NI_RESULT SciSDK_Device::GetParameter(string Path, string value) {

	return NI_OK;
}
NI_RESULT SciSDK_Device::ExecuteCommand(string Path) {

	return NI_OK;
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
				for each (json r in rs.at(it.key()))
				{
					cout << parent << "/" << it.key() << "/" << (string) r.at("Name")<< endl;

					mmcs.push_back(new SciSDK_Register(_hal, r, parent + "/" + (string)it.key()));
				}
			}
			else {
				if (StartWith(ToUpper(it.key()), ToUpper("MMCComponents"))==true) {
					for each (json r in rs.at(it.key()))
					{
						cout << parent << "/" << it.key() << "/" << (string)r.at("Name") << "<" << (string) r.at("Type") << ">" << endl;
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
					for (int i = 1; i < qPP.size(); i++){
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