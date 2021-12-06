#include "scisdk_scinode.h"

NI_RESULT SciSDK_Node::SetParamU32(string name, uint32_t value) {
	SciSDK_Paramcb *p;
	if (FindParameterByName(name, p)) {
		switch (p->type) {
			case SciSDK_Paramcb::Type::I32:
				p->caller->ISetParamI32(name, value); break;
			case SciSDK_Paramcb::Type::U32:
				p->caller->ISetParamU32(name, value); break;
			case SciSDK_Paramcb::Type::I64:
			case SciSDK_Paramcb::Type::U64:
			case SciSDK_Paramcb::Type::d:
				//p->
			break;
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