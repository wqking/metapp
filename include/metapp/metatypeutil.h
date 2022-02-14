#ifndef METATYPEUTIL_H_969872685611
#define METATYPEUTIL_H_969872685611

#include "metapp/typekind.h"
#include "metapp/typelist.h"
#include "metapp/metatype.h"
#include "metapp/metatypedata.h"
#include "metapp/exception.h"
#include "metapp/internal/typeutil_i.h"

namespace metapp {

template <typename T>
inline bool matchUpTypeKinds(const MetaType * metaType, const std::initializer_list<T> & typeKindList)
{
	auto begin = std::begin(typeKindList);
	auto end = std::end(typeKindList);
	while(begin != end) {
		if(metaType == nullptr) {
			return false;
		}
		if(metaType->getTypeKind() != *begin) {
			return false;
		}
		metaType = metaType->getUpType();
		++begin;
	}
	return true;
}

inline bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode)
{
	if(toMetaType->getTypeKind() == tkReference && fromMetaType->getTypeKind() != tkReference) {
		toMetaType = toMetaType->getUpType();
	}
	if(strictMode) {
		if(toMetaType == fromMetaType) {
			return true;
		}
		if(toMetaType == nullptr || fromMetaType == nullptr) {
			return false;
		}
		return toMetaType->getUnifiedType() == fromMetaType->getUnifiedType();
	}
	else {
		if(toMetaType->getTypeKind() == tkReference && fromMetaType->getTypeKind() == tkReference) {
			return true;
		}
		if(toMetaType->getTypeKind() == tkPointer && fromMetaType->getTypeKind() == tkPointer) {
			return true;
		}
		return toMetaType->getUnifiedType() == fromMetaType->getUnifiedType();
	}
}


} // namespace metapp

#endif
