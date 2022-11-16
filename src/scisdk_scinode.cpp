#include "scisdk_scinode.h"

bool SciSDK_Node::OutOfRange(SciSDK_Paramcb *p, double value) {
	if ((p->minmax) && ((value > p->max) || (value < p->min))) {
		return true;
	}
	if (p->lintpar.size()) {
		bool found = (std::find(p->lintpar.begin(), p->lintpar.end(), value) != p->lintpar.end());
		if (!found) return true;
	}
	if (p->lstrpar.size()) {
		bool found = (std::find(p->lstrpar.begin(), p->lstrpar.end(), std::to_string(value)) != p->lstrpar.end());
		if (!found) return true;
	}
	return false;
}

bool SciSDK_Node::OutOfRange(SciSDK_Paramcb *p, string value) {
	if ((p->minmax) && ((std::stol(value) > p->max) || (std::stol(value) < p->min))) {
		return true;
	}
	if (p->lintpar.size()) {
		bool found = (std::find(p->lintpar.begin(), p->lintpar.end(), std::stol(value)) != p->lintpar.end());
		if (!found) return true;
	}
	if (p->lstrpar.size()) {
		bool found = (std::find(p->lstrpar.begin(), p->lstrpar.end(), value) != p->lstrpar.end());
		if (!found) return true;
	}
	return false;
}
NI_RESULT SciSDK_Node::SetParam(string name, uint32_t value) {
	SciSDK_Paramcb *p = NULL;
	if (FindParameterByName(name, &p)) {
		if (OutOfRange(p, (double)value)) return NI_PARAMETER_OUT_OF_RANGE;
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->ISetParamI32(name, value); break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->ISetParamU32(name, value); break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->ISetParamI64(name, value); break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->ISetParamU64(name, value); break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->ISetParamDouble(name, (double)value); break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->ISetParamString(name, std::to_string(value)); break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::SetParam(string name, int32_t value) {
	SciSDK_Paramcb *p = NULL;
	if (FindParameterByName(name, &p)) {
		if (OutOfRange(p, (double)value)) return NI_PARAMETER_OUT_OF_RANGE;
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->ISetParamI32(name, value); break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->ISetParamU32(name, value); break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->ISetParamI64(name, value); break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->ISetParamU64(name, value); break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->ISetParamDouble(name, (double)value); break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->ISetParamString(name, std::to_string(value)); break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::SetParam(string name, uint64_t value) {
	SciSDK_Paramcb *p = NULL;
	if (FindParameterByName(name, &p)) {
		if (OutOfRange(p, (double)value)) return NI_PARAMETER_OUT_OF_RANGE;
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->ISetParamI32(name, value); break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->ISetParamU32(name, value); break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->ISetParamI64(name, value); break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->ISetParamU64(name, value); break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->ISetParamDouble(name, (double)value); break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->ISetParamString(name, std::to_string(value)); break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::SetParam(string name, int64_t value) {
	SciSDK_Paramcb *p = NULL;
	if (FindParameterByName(name, &p)) {
		if (OutOfRange(p, (double)value)) return NI_PARAMETER_OUT_OF_RANGE;
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->ISetParamI32(name, value); break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->ISetParamU32(name, value); break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->ISetParamI64(name, value); break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->ISetParamU64(name, value); break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->ISetParamDouble(name, (double)value); break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->ISetParamString(name, std::to_string(value)); break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::SetParam(string name, double value) {
	SciSDK_Paramcb *p = NULL;
	if (FindParameterByName(name, &p)) {
		if (OutOfRange(p, (double)value)) return NI_PARAMETER_OUT_OF_RANGE;
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->ISetParamI32(name, value); break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->ISetParamU32(name, value); break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->ISetParamI64(name, value); break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->ISetParamU64(name, value); break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->ISetParamDouble(name, (double)value); break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->ISetParamString(name, std::to_string(value)); break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::SetParam(string name, string value) {
	SciSDK_Paramcb *p = NULL;
	if (type == "boardapi") {

		if (FindBoardApiParameterByName(name, &p)) {
			return p->caller->ISetParamString(name, value);
		} else {
			return NI_NOT_FOUND;
		}
	}
	else {
		if (FindParameterByName(name, &p)) {
			if (OutOfRange(p, value)) return NI_PARAMETER_OUT_OF_RANGE;
			switch (p->type) {
			case SciSDK_Paramcb::Type::I32:
				return p->caller->ISetParamI32(name, std::stol(value)); break;
			case SciSDK_Paramcb::Type::U32:
				return p->caller->ISetParamU32(name, std::stoul(value)); break;
			case SciSDK_Paramcb::Type::I64:
				return p->caller->ISetParamI64(name, std::stoll(value)); break;
			case SciSDK_Paramcb::Type::U64:
				return p->caller->ISetParamU64(name, std::stoull(value)); break;
			case SciSDK_Paramcb::Type::d:
				return p->caller->ISetParamDouble(name, std::stod(value)); break;
			case SciSDK_Paramcb::Type::str:
				return p->caller->ISetParamString(name, value); break;
			default: return NI_ERROR; break;
			}
		}
		else {
			return NI_NOT_FOUND;
		}
	}
}

NI_RESULT SciSDK_Node::GetParam(string name, uint32_t *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	NI_RESULT res;

	if (FindParameterByName(name, &p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			res = p->caller->IGetParamI32(name, &i32);
			*value = (uint32_t)i32;
			return res;
			break;
		case SciSDK_Paramcb::Type::U32:
			res = p->caller->IGetParamU32(name, &u32);
			*value = (uint32_t)u32;
			return res;
			break;
		case SciSDK_Paramcb::Type::I64:
			res = p->caller->IGetParamI64(name, &i64);
			*value = (uint32_t)i64;
			return res;
			break;
		case SciSDK_Paramcb::Type::U64:
			res = p->caller->IGetParamU64(name, &u64);
			*value = (uint32_t)u64;
			return res;
			break;
		case SciSDK_Paramcb::Type::d:
			res = p->caller->IGetParamDouble(name, &d);
			*value = (uint32_t)d;
			return res;
			break;
		case SciSDK_Paramcb::Type::str:
			res = p->caller->IGetParamString(name, &s);
			*value = (uint32_t)std::stoul(s);
			return res;
			break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
	
}

NI_RESULT SciSDK_Node::GetParam(string name, int32_t *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	NI_RESULT res;
	if (FindParameterByName(name, &p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			res = p->caller->IGetParamI32(name, &i32);
			*value = (int32_t)i32;
			return res;
			break;
		case SciSDK_Paramcb::Type::U32:
			res = p->caller->IGetParamU32(name, &u32);
			*value = (int32_t)u32;
			return res;
			break;
		case SciSDK_Paramcb::Type::I64:
			res = p->caller->IGetParamI64(name, &i64);
			*value = (int32_t)i64;
			return res;
			break;
		case SciSDK_Paramcb::Type::U64:
			res = p->caller->IGetParamU64(name, &u64);
			*value = (int32_t)u64;
			return res;
			break;
		case SciSDK_Paramcb::Type::d:
			res = p->caller->IGetParamDouble(name, &d);
			*value = (int32_t)d;
			return res;
			break;
		case SciSDK_Paramcb::Type::str:
			res = p->caller->IGetParamString(name, &s);
			*value = (int32_t)std::stol(s);
			return res;
			break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::GetParam(string name, uint64_t *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	NI_RESULT res;
	if (FindParameterByName(name, &p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			res = p->caller->IGetParamI32(name, &i32);
			*value = (uint64_t)i32;
			return res;
			break;
		case SciSDK_Paramcb::Type::U32:
			res = p->caller->IGetParamU32(name, &u32);
			*value = (uint64_t)u32;
			return res;
			break;
		case SciSDK_Paramcb::Type::I64:
			res = p->caller->IGetParamI64(name, &i64);
			*value = (uint64_t)i64;
			return res;
			break;
		case SciSDK_Paramcb::Type::U64:
			res = p->caller->IGetParamU64(name, &u64);
			*value = (uint64_t)u64;
			return res;
			break;
		case SciSDK_Paramcb::Type::d:
			res = p->caller->IGetParamDouble(name, &d);
			*value = (uint64_t)d;
			return res;
			break;
		case SciSDK_Paramcb::Type::str:
			res = p->caller->IGetParamString(name, &s);
			*value = (uint64_t)std::stoull(s);
			return res;
			break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::GetParam(string name, int64_t *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	NI_RESULT res;
	if (FindParameterByName(name, &p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			res = p->caller->IGetParamI32(name, &i32);
			*value = (int64_t)i32;
			return res;
			break;
		case SciSDK_Paramcb::Type::U32:
			res = p->caller->IGetParamU32(name, &u32);
			*value = (int64_t)u32;
			return res;
			break;
		case SciSDK_Paramcb::Type::I64:
			res = p->caller->IGetParamI64(name, &i64);
			*value = (int64_t)i64;
			return res;
			break;
		case SciSDK_Paramcb::Type::U64:
			res = p->caller->IGetParamU64(name, &u64);
			*value = (int64_t)u64;
			return res;
			break;
		case SciSDK_Paramcb::Type::d:
			res = p->caller->IGetParamDouble(name, &d);
			*value = (int64_t)d;
			return res;
			break;
		case SciSDK_Paramcb::Type::str:
			res = p->caller->IGetParamString(name, &s);
			*value = (int64_t)std::stoll(s);
			return res;
			break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::GetParam(string name, double *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	NI_RESULT res;
	if (FindParameterByName(name, &p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			res = p->caller->IGetParamI32(name, &i32);
			*value = (double)i32;
			return res;
			break;
		case SciSDK_Paramcb::Type::U32:
			res = p->caller->IGetParamU32(name, &u32);
			*value = (double)u32;
			return res;
			break;
		case SciSDK_Paramcb::Type::I64:
			res = p->caller->IGetParamI64(name, &i64);
			*value = (double)i64;
			return res;
			break;
		case SciSDK_Paramcb::Type::U64:
			res = p->caller->IGetParamU64(name, &u64);
			*value = (double)u64;
			return res;
			break;
		case SciSDK_Paramcb::Type::d:
			res = p->caller->IGetParamDouble(name, &d);
			*value = (double)d;
			return res;
			break;
		case SciSDK_Paramcb::Type::str:
			res = p->caller->IGetParamString(name, &s);
			*value = (double)std::stod(s);
			return res;
			break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}

NI_RESULT SciSDK_Node::GetParam(string name, string *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	NI_RESULT res;
	if (type == "boardapi") {
		if (FindBoardApiParameterByName(name, &p)) {
			return p->caller->IGetParamString(name, value);
		}
		else {
			return NI_NOT_FOUND;
		}
	}
	else {
		if (FindParameterByName(name, &p)) {
			switch (p->type) {
			case SciSDK_Paramcb::Type::I32:
				res = p->caller->IGetParamI32(name, &i32);
				*value = std::to_string(i32);
				return res;
				break;
			case SciSDK_Paramcb::Type::U32:
				res = p->caller->IGetParamU32(name, &u32);
				*value = std::to_string(u32);
				return res;
				break;
			case SciSDK_Paramcb::Type::I64:
				res = p->caller->IGetParamI64(name, &i64);
				*value = std::to_string(i64);
				return res;
				break;
			case SciSDK_Paramcb::Type::U64:
				res = p->caller->IGetParamU64(name, &u64);
				*value = std::to_string(u64);
				return res;
				break;
			case SciSDK_Paramcb::Type::d:
				res = p->caller->IGetParamDouble(name, &d);
				*value = std::to_string(d);
				return res;
				break;
			case SciSDK_Paramcb::Type::str:
				res = p->caller->IGetParamString(name, &s);
				*value = s;
				return res;
				break;
			default: return NI_ERROR; break;
			}
		}
		else {
			return NI_NOT_FOUND;
		}
	}
}

NI_RESULT SciSDK_Node::GetAllParameters(string * ret)
{
	json json_array = json::array();

	for (int i = 0; i < params.size(); i++) {
		json json_object = json::object();

		json_object["name"] = params.at(i).Name;

		if (params.at(i).type == SciSDK_Paramcb::Type::I32) {
			int32_t value;
			int res = GetParam(params.at(i).Name, &value);
			if (res == 0) {
				json_object["value"] = value;
			}
		}
		else if (params.at(i).type == SciSDK_Paramcb::Type::U32) {
			uint32_t value;
			int res = GetParam(params.at(i).Name, &value);
			if (res == 0) {
				json_object["value"] = value;
			}
		}
		else if (params.at(i).type == SciSDK_Paramcb::Type::I64) {
			int64_t value;
			int res = GetParam(params.at(i).Name, &value);
			if (res == 0) {
				json_object["value"] = value;
			}
		}
		else if (params.at(i).type == SciSDK_Paramcb::Type::U64) {
			uint64_t value;
			string name = params.at(i).Name;
			int res = GetParam(params.at(i).Name, &value);
			if (res == 0) {
				json_object["value"] = value;
			}
		}
		else if (params.at(i).type == SciSDK_Paramcb::Type::str) {
			string value;
			int res = GetParam(params.at(i).Name, &value);
			if (res == 0) {
				json_object["value"] = value;
			}
		}
		else if (params.at(i).type == SciSDK_Paramcb::Type::d) {
			double value;
			int res = GetParam(params.at(i).Name, &value);
			if (res == 0) {
				json_object["value"] = value;
			}
		}
		else {
			return NI_ERROR;
		}

		json_array.push_back(json_object);
	}

	*ret = json_array.dump();
	return NI_OK;
}

NI_RESULT SciSDK_Node::GetParameterDescription(string param_name, string * ret)
{
	for (int i = 0; i < params.size(); i++) {
		if (params.at(i).Name == param_name) {
			*ret = params.at(i).Description;
			return NI_OK;
		}
	}

	return NI_ERROR;
}

NI_RESULT SciSDK_Node::GetParameterListOfValues(string param_name, string * ret)
{
	for (int i = 0; i < params.size(); i++) {
		if (params.at(i).Name == param_name) {
			json json_array = json::array();

			if (params.at(i).type == SciSDK_Paramcb::Type::str) {
				list<string>::iterator li = params.at(i).lstrpar.begin();
				for (int j = 0; j < params.at(i).lstrpar.size(); j++) {
					json_array.push_back(*li);
					advance(li, 1);
				}
			} else {
				list<int>::iterator li = params.at(i).lintpar.begin();
				for (int j = 0; j < params.at(i).lintpar.size(); j++) {
					json_array.push_back(*li);
					advance(li, 1);
				}
			}
			
			*ret = json_array.dump();
			return NI_OK;
		}
	}

	return NI_ERROR;
}

NI_RESULT SciSDK_Node::GetParameterMinimumValue(string param_name, double * ret)
{
	for (int i = 0; i < params.size(); i++) {
		if (params.at(i).Name == param_name) {
			if (params.at(i).minmax) {
				*ret = params.at(i).min;
				return NI_OK;
			}
			else {
				return NI_PARAMETER_MIN_MAX_NOT_SET;
			}
		}
	}
	return NI_ERROR;
}

NI_RESULT SciSDK_Node::GetParameterMaximumValue(string param_name, double * ret)
{
	for (int i = 0; i < params.size(); i++) {
		if (params.at(i).Name == param_name) {
			if (params.at(i).minmax) {
				*ret = params.at(i).max;
				return NI_OK;
			}
			else {
				return NI_PARAMETER_MIN_MAX_NOT_SET;
			}
		}
	}
	return NI_ERROR;
}


NI_RESULT SciSDK_Node::GetParametersProperties(string * ret)
{
	json json_array = json::array();
	

	for (int i = 0; i < params.size(); i++) {
		json json_object = json::object();
		
		json_object["name"] = params.at(i).Name;
		json_object["description"] = params.at(i).Description;
		
		if (params.at(i).type == SciSDK_Paramcb::Type::I32) {
			json_object["type"] = "int32";
		} else if (params.at(i).type == SciSDK_Paramcb::Type::U32) {
			json_object["type"] = "uint32";
		} else if (params.at(i).type == SciSDK_Paramcb::Type::I64) {
			json_object["type"] = "int64";
		} else if (params.at(i).type == SciSDK_Paramcb::Type::U64) {
			json_object["type"] = "uint64";
		} else if (params.at(i).type == SciSDK_Paramcb::Type::str) {
			json_object["type"] = "string";
		} else if (params.at(i).type == SciSDK_Paramcb::Type::d) {
			json_object["type"] = "double";
		}
		
		json limits_obj = json::object();

		limits_obj["isminmax"] = params.at(i).minmax;
		limits_obj["min"] = params.at(i).min;
		limits_obj["max"] = params.at(i).max;
		
		json values_array = json::array();
		if (params.at(i).type == SciSDK_Paramcb::Type::str) {
			list<string>::iterator li = params.at(i).lstrpar.begin();
			for (int j = 0; j < params.at(i).lstrpar.size(); j++) {
				values_array.push_back(*li);
				advance(li, 1);
			}
		}
		else {
			list<int>::iterator li = params.at(i).lintpar.begin();
			for (int j = 0; j < params.at(i).lintpar.size(); j++) {
				values_array.push_back(*li);
				advance(li, 1);
			}
		}
		limits_obj["list_of_values"] = values_array;
		json_object["limits"] = limits_obj;
		json_array.push_back(json_object);
	}

	*ret = json_array.dump();
	return NI_OK;
}


bool SciSDK_Node::FindParameterByName(string name, SciSDK_Paramcb **p) {
	auto f = std::find(params.begin(), params.end(), name);
	if (f != params.end()) {
		*p = &(*f);
		return true;
	}
	else {	
		return false;
	}
}

bool SciSDK_Node::FindBoardApiParameterByName(string name,  SciSDK_Paramcb** p) {
	if (FindParameterByName("boardapi/" + name, p)) return true;
	vector<string> substrings;
	stringstream ss(name);
	string item;
	while (getline(ss, item, '/')) {
		substrings.push_back(item);
	}
	if (substrings.size() < 1) return NI_NOT_FOUND;
	return FindParameterByName("boardapi/" + substrings[0] + "/**", p);
}
