#ifndef HEADER_H_SCISDK_NODE
#define HEADER_H_SCISDK_NODE
#include <iostream>
#include <algorithm>
#include <list>
#include "NIErrorCode.h"
#include "scisdk_SciElement.h"
#include "scisdk_hal.h"
#include "json.hpp"
#include "scisdk_defines.h"
#include "magic_numbers.h"


using json = nlohmann::json;

using namespace std;



class SciSDK_Node {
public:
	class SciSDK_Paramcb {
	public:
		bool minmax = false;
		double min, max;
		list<string> lstrpar;
		list<int> lintpar;
		string Name;
		string Description;
		enum class Type {
			I32, U32, I64, U64, d, str
		} type;
		SciSDK_Node *caller;

		SciSDK_Paramcb(string _name, string _desc, Type _type, SciSDK_Node *_caller) {
			Name = _name;
			Description = _desc;
			type = _type;
			caller = _caller;
			minmax = false;
		}

		SciSDK_Paramcb(string _name, string _desc, Type _type, list<string> _lstrpar, SciSDK_Node *_caller) {
			Name = _name;
			Description = _desc;
			type = _type;
			caller = _caller;
			lstrpar = _lstrpar;
			minmax = false;
		}

		SciSDK_Paramcb(string _name, string _desc, Type _type, list<int> _lintpar, SciSDK_Node *_caller) {
			Name = _name;
			Description = _desc;
			type = _type;
			caller = _caller;
			lintpar = _lintpar;
			minmax = false;
		}

		SciSDK_Paramcb(string _name, string _desc, Type _type, double _min, double _max, SciSDK_Node *_caller) {
			Name = _name;
			Description = _desc;
			type = _type;
			caller = _caller;
			min = _min;
			max = _max;
			minmax = true;
		}

		bool operator==(const string rhs) const { return this->Name == rhs; }
	};

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

	//bool operator==(const string rhs) const { return (this->_path + "/" + this->name) == rhs; }

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
	virtual NI_RESULT GetValueU32(uint32_t *value)  { return NI_NOT_IMPLEMENTED; }
	NI_RESULT SetParam(string name, uint32_t value);
	NI_RESULT GetParam(string name, uint32_t *value);
	NI_RESULT SetParam(string name, uint64_t value) ;
	NI_RESULT GetParam(string name, uint64_t *value);
	NI_RESULT SetParam(string name, int32_t value)  ;
	NI_RESULT GetParam(string name, int32_t *value) ;
	NI_RESULT SetParam(string name, int64_t value) ;
	NI_RESULT GetParam(string name, int64_t *value) ;
	NI_RESULT SetParam(string name, double value);
	NI_RESULT SetParam(string name, string value);
	NI_RESULT GetParam(string name, double *value);
	NI_RESULT GetParam(string name, string *value);
	NI_RESULT SetConfig(string cfg) { return NI_NOT_IMPLEMENTED; }
	NI_RESULT GetConfig(string *cfg) { return NI_NOT_IMPLEMENTED; }

	virtual NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT AllocateBuffer(T_BUFFER_TYPE bt, void **buffer, int size) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT FreeBuffer(T_BUFFER_TYPE bt, void **buffer) { return NI_NOT_IMPLEMENTED; }

	virtual NI_RESULT ReadData(void *buffer) { return NI_NOT_IMPLEMENTED; }

	virtual NI_RESULT ExecuteCommand(string cmd, string param) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ReadStatus(void *buffer) { return NI_NOT_IMPLEMENTED; }

protected:
	string type;
	string name;
	string _path;
	SciSDK_HAL *_hal;

	void RegisterParameter(string name, string description, SciSDK_Paramcb::Type type, SciSDK_Node *caller) {
		params.push_back(SciSDK_Paramcb(name, description, type, caller));
	}

	void RegisterParameter(string name, string description, SciSDK_Paramcb::Type type, list<string> _lstrpar, SciSDK_Node *caller) {
		params.push_back(SciSDK_Paramcb(name, description, type, _lstrpar, caller));
	}

	void RegisterParameter(string name, string description, SciSDK_Paramcb::Type type, list<int> _lintpar, SciSDK_Node *caller) {
		params.push_back(SciSDK_Paramcb(name, description, type, _lintpar, caller));
	}

	void RegisterParameter(string name, string description, SciSDK_Paramcb::Type type, int min, int max, SciSDK_Node *caller) {
		params.push_back(SciSDK_Paramcb(name, description, type, min, max, caller));
	}

	virtual NI_RESULT ISetParamI32(string name, int32_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamI32(string name, int32_t *value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT ISetParamU32(string name, uint32_t value) { return NI_NOT_IMPLEMENTED; }
	virtual NI_RESULT IGetParamU32(string name, uint32_t *value) { return NI_NOT_IMPLEMENTED; }
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
	bool FindParameterByName(string name, SciSDK_Paramcb **p);
	bool SciSDK_Node::OutOfRange(SciSDK_Paramcb *p, double value);
	bool SciSDK_Node::OutOfRange(SciSDK_Paramcb *p, string value);
};
#endif 