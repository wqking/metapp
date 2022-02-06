#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "varpp/common.h"
#include "varpp/internal/metatype_i.h"

#include <memory>
#include <string>
#include <vector>
#include <array>

namespace varpp {

template <typename T, typename Enabled = void>
struct MetaType;
struct MetaTypeData;

using FuncConstruct = void (*)(VariantData & data, const void * value);
using FuncCopy = void (*)(const VariantData & fromData, VariantData & toData);
using FuncGetAddress = void * (*)(const VariantData & data);
using FuncCanCast = bool (*)(const MetaTypeData * toMetaTypeData);
using FuncCast = void (*)(const VariantData & data, const MetaTypeData * toMetaTypeData, void * toData);

struct MetaTypeData
{
public:
	constexpr MetaTypeData() :
		construct(),
		copy(&internal_::emptyCopy),
		getAddress(),
		canCast(),
		cast(),
		varType(vtEmpty),
		extendType()
	{
	}

	constexpr MetaTypeData(
		const VarType varType,
		const ExtendType extendType,
		FuncConstruct construct,
		FuncCopy copy,
		FuncGetAddress getAddress,
		FuncCanCast canCast,
		FuncCast cast
	) :
		construct(construct),
		copy(copy),
		getAddress(getAddress),
		canCast(canCast),
		cast(cast),
		varType(varType),
		extendType(extendType)
	{
	}

	VarType getVarType() const {
		return varType;
	}

	ExtendType getExtendType() const {
		return extendType;
	}

	FuncConstruct construct;
	FuncCopy copy;
	FuncGetAddress getAddress;
	FuncCanCast canCast;
	FuncCast cast;

private:
	VarType varType;
	ExtendType extendType;
};

constexpr MetaTypeData emptyMetaTypeData;

template <typename M>
const MetaTypeData * getMetaTypeData() {
	static constexpr MetaTypeData metaTypeData (
		M::varType,
		M::extendType,
		&M::construct,
		&M::copy,
		&M::getAddress,
		&M::canCast,
		&M::cast
	);
	return &metaTypeData;
}

template <typename T>
struct PodMetaType : public internal_::MetaTypeBase<T>
{
	static void construct(VariantData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static void * getAddress(const VariantData & data) {
		return &data.podAs<T>();
	}

};

template <typename T>
struct ObjectMetaType : public internal_::MetaTypeBase<T>
{
	static constexpr VarType varType = vtObject;

	static void construct(VariantData & data, const void * value) {
		data.object = std::make_shared<T>(*(T *)value);
	}

	static void * getAddress(const VariantData & data) {
		return data.object.get();
	}
};

template <typename T, typename Enabled>
struct MetaType : public ObjectMetaType<T>
{
};

template <typename T>
struct MetaType <T,
	typename std::enable_if<TypeListIn<T, internal_::FundamentalTypeList>::value>::type> : public PodMetaType<T>
{
	//using CastFunc = void (*)(const VariantData & data, void * toData);
	// this array requires either c++17 or definition in souce file
	/*static constexpr std::array<CastFunc, TypeListCount<internal_::FundamentalTypeList>::value> castFunctions{
		&internal_::podCast<T, bool>,
		&internal_::podCast<T, char>, &internal_::podCast<T, wchar_t>,
		&internal_::podCast<T, signed char>, &internal_::podCast<T, unsigned char>,
		&internal_::podCast<T, short>, &internal_::podCast<T, unsigned short>,
		&internal_::podCast<T, int>, &internal_::podCast<T, unsigned int>,
		&internal_::podCast<T, long>, &internal_::podCast<T, unsigned long>,
		&internal_::podCast<T, long long>, &internal_::podCast<T, unsigned long long>,
		&internal_::podCast<T, float>, &internal_::podCast<T, double>, &internal_::podCast<T, long double>
	};*/

	static constexpr VarType varType = VarType(vtFundamentalBegin + TypeListIndexOf<T, internal_::FundamentalTypeList>::value);

	static bool canCast(const MetaTypeData * toMetaTypeData) {
		return ! isAnyExtension(toMetaTypeData->getExtendType())
			&& toMetaTypeData->getVarType() >= vtArithmeticBegin
			&& toMetaTypeData->getVarType() <= vtArithmeticEnd;
	}

	static void cast(const VariantData & data, const MetaTypeData * toMetaTypeData, void * toData) {
		//castFunctions[toMetaTypeData->getVarType() - vtArithmeticBegin](data, toData);
		switch(toMetaTypeData->getVarType()) {
		case vtBool: internal_::podCast<T, bool>(data, toData); break;
		case vtChar: internal_::podCast<T, char>(data, toData); break;
		case vtWideChar: internal_::podCast<T, wchar_t>(data, toData); break;
		case vtSignedChar: internal_::podCast<T, signed char>(data, toData); break;
		case vtUnsignedChar: internal_::podCast<T, unsigned char>(data, toData); break;
		case vtShort: internal_::podCast<T, short>(data, toData); break;
		case vtUnsignedShort: internal_::podCast<T, unsigned short>(data, toData); break;
		case vtInt: internal_::podCast<T, int>(data, toData); break;
		case vtUnsignedInt: internal_::podCast<T, unsigned int>(data, toData); break;
		case vtLong: internal_::podCast<T, long>(data, toData); break;
		case vtUnsignedLong: internal_::podCast<T, unsigned long>(data, toData); break;
		case vtLongLong: internal_::podCast<T, long long>(data, toData); break;
		case vtUnsignedLongLong: internal_::podCast<T, unsigned long long>(data, toData); break;
		case vtFloat: internal_::podCast<T, float>(data, toData); break;
		case vtDouble: internal_::podCast<T, double>(data, toData); break;
		case vtLongDouble: internal_::podCast<T, long double>(data, toData); break;
		}
	}
};

template <>
struct MetaType <void> : public PodMetaType<void>
{
	static constexpr VarType varType = vtVoid;
};

template <typename T>
struct MetaType <const T> : public MetaType<T> {};
template <typename T>
struct MetaType <volatile T> : public MetaType<T> {};
template <typename T>
struct MetaType <const volatile T> : public MetaType<T> {};

template <typename T>
struct MetaType <T,
	typename std::enable_if<std::is_array<T>::value>::type> : public MetaType<typename std::decay<T>::type> {};

template <typename T>
struct MetaType <T,
	typename std::enable_if<std::is_function<T>::value>::type> : public MetaType<typename std::decay<T>::type> {};

template <typename T>
struct MetaType <T *> : public PodMetaType<T *>
{
private:
	// Seems MSVC treats T as pointer, so we need to remove the pointer.
	// All we need to remove all pointers, if there are more than one.
	using Underlying = MetaType<typename std::remove_pointer<T>::type>;

public:
	static constexpr VarType varType = Underlying::varType;
	static constexpr ExtendType extendType = Underlying::extendType | etPointer;

	static bool canCast(const MetaTypeData * toMetaTypeData) {
		return isPointer(toMetaTypeData->getExtendType());
	}

	static void cast(const VariantData & data, const MetaTypeData * /*toMetaTypeData*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};

template <typename T>
struct MetaType <T &> : public MetaType<T>
{
private:
	using super = MetaType<T>;

public:
	static constexpr VarType varType = MetaType<T>::varType;
	static constexpr ExtendType extendType = super::extendType | etReference;

	static void construct(VariantData & data, const void * value) {
		data.podAs<T *>() = *(T **)value;
	}

	static void * getAddress(const VariantData & data) {
		return &data.podAs<T *>();
	}

	static bool canCast(const MetaTypeData * toMetaTypeData) {
		return isReference(toMetaTypeData->getExtendType());
	}

	static void cast(const VariantData & data, const MetaTypeData * /*toMetaTypeData*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};

template <>
struct MetaType <std::string> : public ObjectMetaType<std::string>
{
	static constexpr VarType varType = vtString;
};

template <>
struct MetaType <std::wstring> : public ObjectMetaType<std::wstring>
{
	static constexpr VarType varType = vtWideString;
};

template <>
struct MetaType <std::nullptr_t> : public MetaType<void *>
{
};

template <typename T, typename Alloc>
struct MetaType <std::vector<T, Alloc> > : public MetaType<T>
{
private:
	using super = MetaType<T>;

public:
	static constexpr ExtendType extendType = super::extendType | etVector;

};


} // namespace varpp

#endif
