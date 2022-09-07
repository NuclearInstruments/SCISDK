#include "scisdk_core.h"

#include <chrono>
#include <thread>
#include <iostream>
SciSDK::SciSDK() {

}
SciSDK::~SciSDK() {

}

NI_RESULT SciSDK::AddNewDevice(string DevicePath, string DeviceModel, string JSONFwFilePath, string Name) {

	if (FindDeviceByName(Name) != NULL)
		return NI_DEVICE_NAME_ALREADY_EXISTS;
	SciSDK_Device * newDev;
	newDev = new SciSDK_Device(DevicePath, DeviceModel, JSONFwFilePath, Name);
	NI_RESULT err_cnt = newDev->Connect();
	if (err_cnt != NI_OK) {
		return err_cnt;
	}
	else {
		devs.push_back(newDev);
		return NI_OK;
	}
}
NI_RESULT SciSDK::DetachDevice(string Name) {

	return NI_OK;
}

NI_RESULT SciSDK::SetRegister(string Path, uint32_t value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetRegister(subpath, value);

}

NI_RESULT SciSDK::GetRegister(string Path, uint32_t *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetRegister(subpath, value);

	return NI_OK;
}

NI_RESULT SciSDK::StrobeRegister(string Path, string strobe_polarity) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;

	if ((strobe_polarity == "positive") || (strobe_polarity == "pos")) {
		ret |= dev->SetRegister(subpath, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		ret = dev->SetRegister(subpath, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		ret |= dev->SetRegister(subpath, 0);
	}
	else
	{
		ret |= dev->SetRegister(subpath, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		ret = dev->SetRegister(subpath, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		ret |= dev->SetRegister(subpath, 1);
	}
	return ret;
}

NI_RESULT SciSDK::SetParameter(string Path, uint32_t value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetParameter(subpath, value);
}

NI_RESULT SciSDK::GetParameter(string Path, uint32_t *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetParameter(subpath, value);
}

NI_RESULT SciSDK::SetParameter(string Path, int32_t value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetParameter(subpath, value);
}

NI_RESULT SciSDK::GetParameter(string Path, int32_t *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetParameter(subpath, value);
}

NI_RESULT SciSDK::SetParameter(string Path, uint64_t value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetParameter(subpath, value);
}

NI_RESULT SciSDK::GetParameter(string Path, uint64_t *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetParameter(subpath, value);
}

NI_RESULT SciSDK::SetParameter(string Path, int64_t value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetParameter(subpath, value);
}

NI_RESULT SciSDK::GetParameter(string Path, int64_t *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetParameter(subpath, value);
}

NI_RESULT SciSDK::SetParameter(string Path, double value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetParameter(subpath, value);
}

NI_RESULT SciSDK::GetParameter(string Path, double *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetParameter(subpath, value);
}

NI_RESULT SciSDK::SetParameter(string Path, string value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->SetParameter(subpath, value);
}

NI_RESULT SciSDK::GetParameter(string Path, string *value) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->GetParameter(subpath, value);
}


NI_RESULT SciSDK::ExecuteCommand(string Path, string parameter) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->ExecuteCommand(subpath);
}

NI_RESULT SciSDK::AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->AllocateBuffer(subpath, bt, buffer);
}

NI_RESULT SciSDK::AllocateBuffer(string Path, T_BUFFER_TYPE bt, void **buffer, int size) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->AllocateBuffer(subpath, bt, buffer, size);
}
NI_RESULT SciSDK::FreeBuffer(string Path, T_BUFFER_TYPE bt, void **buffer) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->FreeBuffer(subpath, bt, buffer);
}

NI_RESULT SciSDK::ReadData(string Path, void *buffer) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->ReadData(subpath, buffer);
}

NI_RESULT SciSDK::DecodeData(string Path, void *buffer_in, void *buffer_out) {

	return NI_OK;
}

NI_RESULT SciSDK::GetComponentList(string name, string Type, string *res, bool return_json)
{
	vector<SciSDK_Device*> dev_vector{ std::begin(devs), std::end(devs) };
	for (int i = 0; i < devs.size(); i++) {
		if (dev_vector.at(i)->GetName() == name) {
			int r =  dev_vector.at(i)->GetComponentList(Type, res, return_json);
			//cout << *res << endl;
			return r;
		}
	}
	return NI_ERROR;
}


SciSDK_Device * SciSDK::FindDeviceByName(string Name) {
	std::list<SciSDK_Device *>::iterator it;
	for (it = devs.begin(); it != devs.end(); ++it) {
		if ((*it)->GetName() == Name) {
			return *it;
		}
	}
	return NULL;
}

std::vector<std::string> SciSDK::SplitPath(string path, char separator) {

	std::stringstream test(path);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, separator))
	{
		seglist.push_back(segment);
	}
	return seglist;

}

NI_RESULT SciSDK::LocateDevice(string path, SciSDK_Device **dev, string *subpath) {
	vector<string> qPP = SplitPath(path, ':');
	if (qPP.size() != 2) return NI_INVALID_PATH;
	string devname = qPP[0];
	*subpath = qPP[1];
	*dev = FindDeviceByName(devname);
	if (!*dev) return NI_NOT_FOUND;
	return NI_OK;
}

string SciSDK::s_error(int err_no) {

	switch (err_no) {
	case NI_OK:
		return "";
	case NI_ERROR_GENERIC:
		return "generic error";
	case NI_ERROR_INTERFACE:
		return "hardware interface error";
	case NI_ERROR_FPGA:
		return "fpga core return an error";
	case NI_ERROR_TRANSFER_MAX_LENGTH:
		return "transfer length exeed maximum limit";
	case NI_ERROR_NOTCONNECTED:
		return "hardware not connected";
	case NI_NO_DATA_AVAILABLE:
		return "no data available";
	case NI_TOO_MANY_DEVICES_CONNECTED:
		return "too many device connected";
	case NI_INVALID_HANDLE:
		return "handle is not valid";
	case NI_INVALID_KEY:
		return "json key not valid, error in json file";
	case NI_INVALID_PARAMETER:
		return "parameter does not exists";
	case NI_PARAMETER_OUT_OF_RANGE:
		return "parameter out of range";
	case NI_INCOMPLETE_READ:
		return "read not completed";
	case NI_INVALID_COMMAND:
		return "invalid command";
	case NI_ALREADY_CONNECTED:
		return "already connected to hardware";
	case NI_ALLOC_FAILED:
		return "alloc failed, out of memory";
	case NI_MEMORY_NOT_ALLOCATED:
		return "memory is not allocated, nullptr";
	case NI_INVALID_BUFFER_TYPE:
		return "invalid buffer type";
	case NI_INVALID_BUFFER_SIZE:
		return "invalid buffer size";
	case NI_INCOMPATIBLE_BUFFER:
		return "incompatible buffer specs";
	case NI_INVALID_BUFFER:
		return "invalid buffer";
	case NI_TIMEOUT:
		return "timeous";
	case NI_INVALID_CFG_JSON:
		return "config json is not valid, error in file";
	case NI_CFG_JSON_NOT_FOUND:
		return "config json not found";
	case NI_DEVICE_NAME_ALREADY_EXISTS:
		return "device name already exists";
	case NI_INVALID_PATH:
		return "invalid path";
	case NI_NOT_FOUND:
		return "not found";
	case NI_INVALID_TYPE:
		return "invalid type";
	case NI_ALREADY_RUNNING:
		return "already running";
	case NI_NOT_RUNNING:
		return "not running";
	case NI_NOT_ARMED:
		return "not armed";
	case NI_SPECIFIC_ERROR:
		return "specific error";
	}



}

void SciSDK::p_error(int err_no) {
	if (err_no != NI_OK) {
		cerr << "0x" << std::hex << err_no;
		cerr << " - " << s_error(err_no) << endl;
	}
}

void SciSDK::p_error(string fnc, int err_no) {
	if (err_no != NI_OK) {
		cerr << fnc << " : ";
		cerr << "0x" << std::hex << err_no;
		cerr << " - " << s_error(err_no) << endl;
	}
}

void SciSDK::p_error(int line, int err_no) {
	if (err_no != NI_OK) {
		cerr << "LINE " << line << " : ";
		cerr << "0x" << std::hex << err_no;
		cerr << " - " << s_error(err_no) << endl;
	}
}

void SciSDK::p_error(string file, int line, int err_no) {
	if (err_no != NI_OK) {
		cerr << file << " : ";
		cerr << "LINE " << line << " : ";
		cerr << "0x" << std::hex << err_no;
		cerr << " - " << s_error(err_no) << endl;
	}
}
