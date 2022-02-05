#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "varpp/common.h"
#include "varpp/typelist.h"

#include <memory>
#include <string>
#include <array>

namespace varpp {

template <typename T, typename Enabled = void>
struct MetaType;
struct MetaTypeData;

inline void emptyCopy(const VariantData & /*fromData*/, VariantData & /*toData*/)
{
}

using FuncConstruct = void (*)(VariantData & data, const void * value);
using FuncDestruct = void (*)(VariantData & data);
using FuncCopy = void (*)(const VariantData & fromData, VariantData & toData);
using FuncGetAddress = void * (*)(const VariantData & data);
using FuncCanCast = bool (*)(const MetaTypeData * toMetaTypeData);
using FuncCast = void (*)(const VariantData & data, const MetaTypeData * toMetaTypeData, void * toData);

struct MetaTypeData
{
public:
	constexpr MetaTypeData() :
		construct(),
		destruct(),
		copy(&emptyCopy),
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
		FuncDestruct destruct,
		FuncCopy copy,
		FuncGetAddress getAddress,
		FuncCanCast canCast,
		FuncCast cast
	) :
		construct(construct),
		destruct(destruct),
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
	FuncDestruct destruct;
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
		&M::destruct,
		&M::copy,
		&M::getAddress,
		&M::canCast,
		&M::cast
	);
	return &metaTypeData;
}

template <typename T_>
struct MetaTypeBase
{
	using T = T_;
	
	static constexpr VarType varType = vtObject;
	static constexpr ExtendType extendType = etNone;

	static void construct(VariantData & data, const void * value) {
		data.object = std::make_shared<T>(*(T *)value);
	}

	static void destruct(VariantData & /*data*/) {
	}

	static void copy(const VariantData & fromData, VariantData & toData) {
		toData = fromData;
	}

	static void * getAddress(const VariantData & data) {
		return data.object.get();
	}

	static bool canCast(const MetaTypeData * /*toMetaTypeData*/) {
		return false;
	}

	static void cast(const VariantData & /*data*/ , const MetaTypeData * /*toMetaTypeData*/, void * /*toData*/) {
	}
};

template <typename T, typename Enabled>
struct MetaType : public MetaTypeBase<T>
{
};

template <typename T>
struct PodMetaType : public MetaTypeBase<T>
{
	static void construct(VariantData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static void * getAddress(const VariantData & data) {
		return &data.podAs<T>();
	}

};

using FundamentalTypeList = TypeList<
	bool,
	char, wchar_t,
	signed char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long,
	float, double, long double
>;

template <typename T, typename U>
void fundamentalCast(const VariantData & data, void * toData) {
	*(U *)toData = (U)(data.podAs<T>());
}

template <typename T>
struct MetaType <T,
	typename std::enable_if<TypeListIn<T, FundamentalTypeList>::value>::type> : public PodMetaType<T>
{
	using CastFunc = void (*)(const VariantData & data, void * toData);
	static constexpr std::array<CastFunc, TypeListCount<FundamentalTypeList>::value> castFunctions {
		&fundamentalCast<T, bool>,
		&fundamentalCast<T, char>, &fundamentalCast<T, wchar_t>,
		&fundamentalCast<T, signed char>, &fundamentalCast<T, unsigned char>,
		&fundamentalCast<T, int>, &fundamentalCast<T, unsigned int>,
		&fundamentalCast<T, long>, &fundamentalCast<T, unsigned long>,
		&fundamentalCast<T, long long>, &fundamentalCast<T, unsigned long long>,
		&fundamentalCast<T, float>, &fundamentalCast<T, double>, &fundamentalCast<T, long double>
	};

	static constexpr VarType varType = VarType(vtFundamentalBegin + TypeListIndexOf<T, FundamentalTypeList>::value);

	static bool canCast(const MetaTypeData * toMetaTypeData) {
		return ! isAnyExtension(toMetaTypeData->getExtendType())
			&& toMetaTypeData->getVarType() >= vtArithmeticBegin
			&& toMetaTypeData->getVarType() <= vtArithmeticEnd;
	}

	static void cast(const VariantData & data, const MetaTypeData * toMetaTypeData, void * toData) {
		castFunctions[toMetaTypeData->getVarType() - vtArithmeticBegin](data, toData);
	}
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
struct MetaType <T *> : public MetaType<T>
{
private:
	using super = MetaType<T>;

public:
	static constexpr VarType varType = super::varType;
	static constexpr ExtendType extendType = super::extendType | etPointer;

	static void construct(VariantData & data, const void * value) {
		data.podAs<T *>() = *(T **)value;
	}

	static void * getAddress(const VariantData & data) {
		return &data.podAs<T *>();
	}

	static bool canCast(const MetaTypeData * toMetaTypeData) {
		return isPointer(toMetaTypeData->getExtendType());
	}

	static void cast(const VariantData & data, const MetaTypeData * /*toMetaTypeData*/, void * toData) {
		fundamentalCast<T *, void *>(data, toData);
	}
};

template <typename T>
struct MetaType <T &> : public MetaType<T>
{
private:
	using super = MetaType<T>;

public:
	static constexpr VarType varType = super::varType;
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
		fundamentalCast<T *, void *>(data, toData);
	}
};

template <>
struct MetaType <std::string> : public MetaTypeBase<std::string>
{
	static constexpr VarType varType = vtString;
};

template <>
struct MetaType <std::wstring> : public MetaTypeBase<std::wstring>
{
	static constexpr VarType varType = vtWideString;
};


} // namespace varpp

#endif
