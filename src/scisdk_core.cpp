#include "scisdk_core.h"

#include <chrono>
#include <thread>
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
	int ret ;
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

	if ((strobe_polarity=="positive") || (strobe_polarity=="pos")) {
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
	return dev->AllocateBuffer(subpath, bt, buffer,size);
}
NI_RESULT SciSDK::FreeBuffer(string Path, T_BUFFER_TYPE bt, void **buffer) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->FreeBuffer(subpath, bt, buffer);
}

NI_RESULT SciSDK::ReadData(string Path,  void *buffer) {
	SciSDK_Device * dev;
	string subpath;
	int ret;
	if ((ret = LocateDevice(Path, &dev, &subpath)) != 0) return ret;
	return dev->ReadData(subpath, buffer);
}

NI_RESULT SciSDK::DecodeData(string Path, void *buffer_in, void *buffer_out) {

	return NI_OK;
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