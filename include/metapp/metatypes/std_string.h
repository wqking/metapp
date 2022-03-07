#ifndef METAPP_STD_STRING_H_969872685611
#define METAPP_STD_STRING_H_969872685611

#include "metapp/metatype.h"

#include <string>

namespace metapp {

template <>
struct DeclareMetaTypeBase <std::string>
{
	using Fallback = DeclareMetaTypeObject<std::string>;
	static constexpr TypeKind typeKind = tkStdString;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return Fallback::canCast(value, toMetaType)
			|| canCastToCharPtr(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToCharPtr(toMetaType)) {
			return value.get<const std::string &>().c_str();
		}
		else {
			return Fallback::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToCharPtr(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkCharPtr
			|| (toMetaType->getTypeKind() == tkPointer && toMetaType->getUpType()->getTypeKind() == tkChar)
		;
	}
};

template <>
struct DeclareMetaTypeBase <std::wstring>
{
	using Fallback = DeclareMetaTypeObject<std::wstring>;

	static constexpr TypeKind typeKind = tkStdWideString;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return Fallback::canCast(value, toMetaType)
			|| canCastToWideCharPtr(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToWideCharPtr(toMetaType)) {
			return value.get<const std::wstring &>().c_str();
		}
		else {
			return Fallback::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToWideCharPtr(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkWideCharPtr
			|| (toMetaType->getTypeKind() == tkPointer && toMetaType->getUpType()->getTypeKind() == tkWideChar)
		;
	}
};


} // namespace metapp


#endif

