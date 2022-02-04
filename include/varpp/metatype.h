#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "varpp/common.h"
#include "varpp/typelist.h"

#include <memory>

namespace varpp {

template <typename T, typename Enabled = void>
struct MetaType;

inline void emptyCopy(const VariantData & /*fromData*/, VariantData & /*toData*/)
{
}

struct MetaTypeData
{
public:
	constexpr MetaTypeData() :
		construct(),
		destruct(),
		copy(&emptyCopy),
		getAddress(),
		varType(vtEmpty),
		extendType(0)
	{
	}

	constexpr MetaTypeData(
		const VarType varType,
		const ExtendType extendType,
		void (*construct)(VariantData & data, const void * value),
		void (*destruct)(VariantData & data),
		void (*copy)(const VariantData & fromData, VariantData & toData),
		void * (*getAddress)(const VariantData & data)
	) :
		construct(construct),
		destruct(destruct),
		copy(copy),
		getAddress(getAddress),
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

	void (*construct)(VariantData & data, const void * value);
	void (*destruct)(VariantData & data);
	void (*copy)(const VariantData & fromData, VariantData & toData);
	void * (*getAddress)(const VariantData & data);

private:
	VarType varType;
	ExtendType extendType;
};

static constexpr MetaTypeData emptyMetaTypeData;

template <typename M>
const MetaTypeData * getMetaTypeData() {
	static constexpr MetaTypeData metaTypeData (
		M::varType,
		M::extendType,
		&M::construct,
		&M::destruct,
		&M::copy,
		&M::getAddress
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
};

template <typename T, typename Enabled>
struct MetaType : public MetaTypeBase<T>
{
};

using FundamentalTypeList = TypeList<
	void,
	bool,
	char, wchar_t,
	signed char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long,
	float, double, long double
>;

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

template <typename T>
struct MetaType <T, typename std::enable_if<TypeListIn<T, FundamentalTypeList>::value>::type> : public PodMetaType<T>
{
	static constexpr VarType varType = vtFundamentalBegin + TypeListIndexOf<T, FundamentalTypeList>::value;
};

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
};


} // namespace varpp

#endif
