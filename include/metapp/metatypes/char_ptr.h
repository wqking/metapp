#ifndef METAPP_CHAR_PTR_H_969872685611
#define METAPP_CHAR_PTR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/declareutil.h"
// Need the specialization to be included first
#include "metapp/metatypes/pointer.h"

#include <string>

namespace metapp {

template <typename T>
struct DeclareCharPtrBase
{
	using Common = CommonDeclareMetaType <T>;
	using UpType = typename std::remove_pointer<typename std::decay<T>::type>::type;
	static constexpr TypeKind typeKind = tkCharPtr;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return Common::canCast(value, toMetaType)
			|| canCastToStdString(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToStdString(toMetaType)) {
			return std::string(value.get<const char *>());
		}
		else {
			return Common::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToStdString(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkStdString;
	}
};

template <>
struct DeclareMetaType <const char *> : public DeclareCharPtrBase <const char *>
{
};

template <>
struct DeclareMetaType <char *> : public DeclareCharPtrBase <char *>
{
};

template <>
struct DeclareMetaType <char[]> : public DeclareMetaTypeBase <char[]>
{
};

template <>
struct DeclareMetaType <const char[]> : public DeclareMetaTypeBase <const char[]>
{
};

template <size_t length>
struct DeclareMetaType <char[length]> : public DeclareMetaTypeBase <char[length]>
{
};

template <size_t length>
struct DeclareMetaType <const char[length]> : public DeclareMetaTypeBase <const char[length]>
{
};


template <typename T>
struct DeclareWideCharPtrBase
{
private:
	using Common = CommonDeclareMetaType <T>;

public:
	using UpType = typename std::remove_pointer<typename std::decay<T>::type>::type;
	static constexpr TypeKind typeKind = tkWideCharPtr;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return Common::canCast(value, toMetaType)
			|| canCastToStdWstring(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToStdWstring(toMetaType)) {
			return std::wstring(value.get<const wchar_t *>());
		}
		else {
			return Common::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToStdWstring(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkStdWideString;
	}
};

template <>
struct DeclareMetaType <const wchar_t *> : public DeclareWideCharPtrBase <const wchar_t *>
{
};

template <>
struct DeclareMetaType <wchar_t *> : public DeclareWideCharPtrBase <wchar_t *>
{
};

template <>
struct DeclareMetaType <wchar_t[]> : public DeclareMetaTypeBase <wchar_t[]>
{
};

template <>
struct DeclareMetaType <const wchar_t[]> : public DeclareMetaTypeBase <const wchar_t[]>
{
};

template <size_t length>
struct DeclareMetaType <wchar_t[length]> : public DeclareMetaTypeBase <wchar_t[length]>
{
};

template <size_t length>
struct DeclareMetaType <const wchar_t[length]> : public DeclareMetaTypeBase <const wchar_t[length]>
{
};


} // namespace metapp


#endif

