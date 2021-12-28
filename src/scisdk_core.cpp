#include "scisdk_core.h"


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

	return NI_OK;
}

NI_RESULT SciSDK::GetRegister(string Path, uint32_t *value) {

	return NI_OK;
}

NI_RESULT SciSDK::StrobeRegister(string Path, string strobe_polarity) {

	return NI_OK;
}

NI_RESULT SciSDK::SetParameter(string Path, uint32_t value) {

	return NI_OK;
}

NI_RESULT SciSDK::GetParameter(string Path, uint32_t *value) {

	return NI_OK;
}

NI_RESULT SciSDK::SetParameter(string Path, string value) {

	return NI_OK;
}

NI_RESULT SciSDK::GetParameter(string Path, string *value) {

	return NI_OK;
}


NI_RESULT SciSDK::ExecuteCommand(string Path, string parameter) {

	return NI_OK;
}

NI_RESULT SciSDK::AllocateBuffer(string Path, string parameter, void **buffer) {

	return NI_OK;
}

NI_RESULT SciSDK::ReleaseBuffer(void *buffer) {

	return NI_OK;
}

NI_RESULT SciSDK::ReadData(string Path,  void *buffer) {

	return NI_OK;
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