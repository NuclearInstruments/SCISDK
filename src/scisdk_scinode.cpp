#include "scisdk_scinode.h"

NI_RESULT SciSDK_Node::SetParam(string name, uint32_t value) {
	SciSDK_Paramcb *p = NULL;
	if (FindParameterByName(name, p)) {
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
				return p->caller->ISetParamDouble(name, (double) value); break;
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
	if (FindParameterByName(name, p)) {
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
	if (FindParameterByName(name, p)) {
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
	if (FindParameterByName(name, p)) {
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
	if (FindParameterByName(name, p)) {
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

	if (FindParameterByName(name, p)) {
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

NI_RESULT SciSDK_Node::GetParam(string name, uint32_t *value) {
	SciSDK_Paramcb *p = NULL;
	uint32_t u32;
	int32_t i32;
	uint64_t u64;
	int64_t i64;
	double d;
	string s;
	if (FindParameterByName(name, p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->IGetParamI32(name, &i32); *value = (uint32_t) i32; break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->IGetParamU32(name, &u32); *value = (uint32_t) u32; break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->IGetParamI64(name, &i64); *value = (uint32_t) i64;  break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->IGetParamU64(name, &u64); *value = (uint32_t) u64; break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->IGetParamDouble(name, &d); *value = (uint32_t) d; break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->IGetParamString(name, &s); *value = (uint32_t) std::stoul(s); break;
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
	if (FindParameterByName(name, p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->IGetParamI32(name, &i32); *value = (int32_t)i32; break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->IGetParamU32(name, &u32); *value = (int32_t)u32; break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->IGetParamI64(name, &i64); *value = (int32_t)i64;  break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->IGetParamU64(name, &u64); *value = (int32_t)u64; break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->IGetParamDouble(name, &d); *value = (int32_t)d; break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->IGetParamString(name, &s); *value = (int32_t)std::stol(s); break;
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
	if (FindParameterByName(name, p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->IGetParamI32(name, &i32); *value = (uint64_t)i32; break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->IGetParamU32(name, &u32); *value = (uint64_t)u32; break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->IGetParamI64(name, &i64); *value = (uint64_t)i64;  break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->IGetParamU64(name, &u64); *value = (uint64_t)u64; break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->IGetParamDouble(name, &d); *value = (uint64_t)d; break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->IGetParamString(name, &s); *value = (uint64_t)std::stoull(s); break;
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
	if (FindParameterByName(name, p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->IGetParamI32(name, &i32); *value = (int64_t)i32; break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->IGetParamU32(name, &u32); *value = (int64_t)u32; break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->IGetParamI64(name, &i64); *value = (int64_t)i64;  break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->IGetParamU64(name, &u64); *value = (int64_t)u64; break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->IGetParamDouble(name, &d); *value = (int64_t)d; break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->IGetParamString(name, &s); *value = (int64_t)std::stoll(s); break;
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
	if (FindParameterByName(name, p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->IGetParamI32(name, &i32); *value = (double)i32; break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->IGetParamU32(name, &u32); *value = (double)u32; break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->IGetParamI64(name, &i64); *value = (double)i64;  break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->IGetParamU64(name, &u64); *value = (double)u64; break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->IGetParamDouble(name, &d); *value = (double)d; break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->IGetParamString(name, &s); *value = (double)std::stod(s); break;
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
	if (FindParameterByName(name, p)) {
		switch (p->type) {
		case SciSDK_Paramcb::Type::I32:
			return p->caller->IGetParamI32(name, &i32); *value = std::to_string(i32); break;
		case SciSDK_Paramcb::Type::U32:
			return p->caller->IGetParamU32(name, &u32); *value = std::to_string(u32); break;
		case SciSDK_Paramcb::Type::I64:
			return p->caller->IGetParamI64(name, &i64); *value = std::to_string(i64);  break;
		case SciSDK_Paramcb::Type::U64:
			return p->caller->IGetParamU64(name, &u64); *value = std::to_string(u64); break;
		case SciSDK_Paramcb::Type::d:
			return p->caller->IGetParamDouble(name, &d); *value = std::to_string(d); break;
		case SciSDK_Paramcb::Type::str:
			return p->caller->IGetParamString(name, &s); *value = s; break;
		default: return NI_ERROR; break;
		}
	}
	else {
		return NI_NOT_FOUND;
	}
}
bool SciSDK_Node::FindParameterByName(string name, SciSDK_Paramcb *p) {
	auto f = std::find(params.begin(), params.end(), name);
	if (f != params.end()) {
		p = &(*f);
		return true;
	}
	else {
		return false;
	}
}