#ifndef HEADER_H_SCISDK_NODE
#define HEADER_H_SCISDK_NODE
#include <iostream>
#include <algorithm>
#include <list>
#include "NIErrorCode.h"
#include "scisdk_SciElement.h"
#include "scisdk_hal.h"
#include "json.hpp"


using json = nlohmann::json;

using namespace std;

class SciSDK_Paramcb {
public:

	string Name;
	string Description;
	enum class Type{
		I32, U32, I64, U64, d, str
	} type;
	SciSDK_Node *caller;

	SciSDK_Paramcb(string _name, string _desc, Type _type, SciSDK_Node *_caller) {
		Name = _name;
		Description = _desc;
		type = _type;
		caller = _caller;
	}
	bool operator==(const string rhs) const { return this->Name == rhs; }
};

class SciSDK_Node {
public:
	SciSDK_Node(SciSDK_HAL *hal, json j, string path)  {
		_path = path;
		_hal = hal;
		name = (string)j.at("Name");
		try {
			type = (string)j.at("Type");
		} catch (json::exception& e) {
			type = "Register";
		}
	}

	string GetPath() {
		return _path + "/" + name;
	}

	string GetRootPath() {
		return _path;
	}


	string GetName() {
		return _path;
	}

	virtual NI_RESULT SetValueU32(uint32_t value)  {return NI_NOT_IMPLEMENTED;}
	virtual NI_RESULT GetValueU23(uint32_t *value)  { return NI_NOT_IMPLEMENTED; }
	NI_RESULT SetParamU32(string name, uint32_t value);
	virtual NI_RESULT GetParamU32(string name, uint32_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT SetParamU64(string name, uint64_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT GetParamU64(string name, uint64_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT SetParamI32(string name, int32_t value)  { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT GetParamI32(string name, int32_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT SetParamI64(string name, int64_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT GetParamI64(string name, int64_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT SetParamDouble(string name, double value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT SetParamString(string name, string value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT GetParamDouble(string name, double *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT GetParamString(string name, string *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT SetConfig(string cfg) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT GetConfig(string *cfg) { return NI_NOT_IMPLEMENTED; }


protected:
	string type;
	string name;
	string _path;
	SciSDK_HAL *_hal;

	void RegisterParameter(string name, string description, SciSDK_Paramcb::Type type, SciSDK_Node *caller) {
		params.push_back(SciSDK_Paramcb(name, description, type, caller));
	}

	virtual NI_RESULT ISetParamI32(string name, int32_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamI32(string name, int32_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ISetParamU32(string name, int32_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamU32(string name, int32_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ISetParamU64(string name, uint64_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamU64(string name, uint64_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ISetParamI64(string name, int64_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamI64(string name, int64_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ISetParamDouble(string name, double value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ISetParamString(string name, string value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamDouble(string name, double *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamString(string name, string *value) { return NI_NOT_IMPLEMENTED; }

private:
	vector<SciSDK_Paramcb> params;
	bool FindParameterByName(string name, SciSDK_Paramcb *p);
};
#endif 