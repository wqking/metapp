#ifndef STRING_H_969872685611
#define STRING_H_969872685611

#include "metapp/metatype.h"

#include <string>

namespace metapp {

template <>
struct DeclareMetaTypeBase <std::string> : public DeclareObjectMetaType<std::string>
{
	using super = DeclareObjectMetaType<std::string>;

	static constexpr TypeKind typeKind = tkString;

	static bool canCast(const MetaType * toMetaType) {
		return super::canCast(toMetaType)
			|| canCastToCharPtr(toMetaType);
	}

	static void cast(const MetaTypeData & data , const MetaType * toMetaType , MetaTypeData * toData) {
		if(canCastToCharPtr(toMetaType)) {
			const char * value = std::static_pointer_cast<std::string>(data.object)->c_str();
			toMetaType->constructWith(*toData, &value);
		}
		else {
			super::cast(data, toMetaType, toData);
		}
	}

private:
	static bool canCastToCharPtr(const MetaType * toMetaType) {
		return matchUpTypeKinds(toMetaType, { tkPointer, tkChar });
	}
};

template <>
struct DeclareMetaTypeBase <std::wstring> : public DeclareObjectMetaType<std::wstring>
{
	using super = DeclareObjectMetaType<std::wstring>;

	static constexpr TypeKind typeKind = tkWideString;

	static bool canCast(const MetaType * toMetaType) {
		return super::canCast(toMetaType)
			|| canCastToWideCharPtr(toMetaType);
	}

	static void cast(const MetaTypeData & data , const MetaType * toMetaType , MetaTypeData * toData) {
		if(canCastToWideCharPtr(toMetaType)) {
			const wchar_t * value = std::static_pointer_cast<std::wstring>(data.object)->c_str();
			toMetaType->constructWith(*toData, &value);
		}
		else {
			super::cast(data, toMetaType, toData);
		}
	}

private:
	static bool canCastToWideCharPtr(const MetaType * toMetaType) {
		return matchUpTypeKinds(toMetaType, { tkPointer, tkWideChar });
	}
};


} // namespace metapp


#endif

