#ifndef STRING_H_969872685611
#define STRING_H_969872685611

#include "metapp/metatype.h"

#include <string>

namespace metapp {

template <>
struct DeclareMetaType <std::string> : public DeclareMetaTypeBase <std::string>
{
	using super = DeclareObjectMetaType<std::string>;

	static constexpr TypeKind typeKind = tkString;

	static bool canCast(const MetaType * toMetaType) {
		return super::canCast(toMetaType)
			|| canCastToCharPtr(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToCharPtr(toMetaType)) {
			return value.get<const std::string &>().c_str();
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToCharPtr(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer
			&& toMetaType->getUpType()->getTypeKind() == tkChar;
	}
};

template <>
struct DeclareMetaType <std::wstring> : public DeclareMetaTypeBase <std::wstring>
{
	using super = DeclareObjectMetaType<std::wstring>;

	static constexpr TypeKind typeKind = tkWideString;

	static bool canCast(const MetaType * toMetaType) {
		return super::canCast(toMetaType)
			|| canCastToWideCharPtr(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToWideCharPtr(toMetaType)) {
			return value.get<const std::wstring &>().c_str();
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToWideCharPtr(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer
			&& toMetaType->getUpType()->getTypeKind() == tkWideChar;
	}
};


} // namespace metapp


#endif

