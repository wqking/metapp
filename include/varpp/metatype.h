#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "varpp/common.h"
#include "varpp/internal/metatype_i.h"

namespace varpp {

class MetaType;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

using FuncConstruct = void (*)(VariantData & data, const void * value);
using FuncGetAddress = const void * (*)(const VariantData & data);
using FuncCanCast = bool (*)(const MetaType * toMetaType);
using FuncCast = void (*)(const VariantData & data, const MetaType * toMetaType, void * toData);

class MetaType
{
public:
	constexpr MetaType() :
		construct(),
		getAddress(),
		canCast(),
		cast(),
		upType(),
		typeKind(tkEmpty),
		qualifiers()
	{
	}

	constexpr MetaType(
		const MetaType * upType,
		const TypeKind typeKind,
		const QualifierKind qualifiers,
		FuncConstruct construct,
		FuncGetAddress getAddress,
		FuncCanCast canCast,
		FuncCast cast
	) :
		construct(construct),
		getAddress(getAddress),
		canCast(canCast),
		cast(cast),
		upType(upType),
		typeKind(typeKind),
		qualifiers(qualifiers)
	{
	}

	const MetaType * getUpType() const {
		return upType;
	}

	TypeKind getVarType() const {
		return typeKind;
	}

	bool isConst() const {
		return qualifiers & qkConst;
	}

	bool isVolatile() const {
		return qualifiers & qkVolatile;
	}

	FuncConstruct construct;
	FuncGetAddress getAddress;
	FuncCanCast canCast;
	FuncCast cast;

private:
	const MetaType * upType;
	TypeKind typeKind;
	QualifierKind qualifiers;
};

constexpr MetaType emptyMetaType;

template <typename T>
auto getMetaType()
	-> typename std::enable_if<std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type
{
	return nullptr;
}

template <typename T>
auto getMetaType()
	-> typename std::enable_if<! std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type
{
	using M = DeclareMetaType<typename std::remove_cv<T>::type>;

	static const MetaType metaType (
		getMetaType<typename M::UpType>(),
		M::typeKind,
		M::qualifiers,
		&M::construct,
		&M::getAddress,
		&M::canCast,
		&M::cast
	);
	return &metaType;
}

template <typename T>
struct DeclarePodMetaType : public internal_::DeclareMetaTypeBase<T>
{
	static void construct(VariantData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static const void * getAddress(const VariantData & data) {
		return &data.podAs<T>();
	}

};

template <typename T>
struct DeclareObjectMetaType : public internal_::DeclareMetaTypeBase<T>
{
	static constexpr TypeKind typeKind = tkObject;

	using U = typename std::remove_reference<T>::type;

	static void construct(VariantData & data, const void * value) {
		data.object = std::make_shared<U>(*(U *)value);
	}

	static const void * getAddress(const VariantData & data) {
		return data.object.get();
	}
};

template <typename T, typename Enabled>
struct DeclareMetaType : public DeclareObjectMetaType<T>
{
};

inline const MetaType * getUpTypeAt(const MetaType * metaType, size_t index)
{
	while(metaType != nullptr && index > 0) {
		metaType = metaType->getUpType();
		--index;
	}
	return metaType;
}

inline std::vector<TypeKind> getUpTypeVarTypes(const MetaType * metaType)
{
	std::vector<TypeKind> result;
	result.reserve(8);
	while(metaType != nullptr) {
		result.push_back(metaType->getVarType());
		metaType = metaType->getUpType();
	}
	return result;
}


} // namespace varpp

#endif
