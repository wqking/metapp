#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "metapp/typekind.h"
#include "metapp/internal/metatype_i.h"

#include <type_traits>

namespace metapp {

class MetaType;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

using FuncConstruct = void (*)(MetaTypeData & data, const void * value);
using FuncGetAddress = const void * (*)(const MetaTypeData & data);
using FuncCanCast = bool (*)(const MetaType * toMetaType);
using FuncCast = void (*)(const MetaTypeData & data, const MetaType * toMetaType, void * toData);

class UnifiedType
{
public:
	constexpr UnifiedType() :
		construct(),
		getAddress(),
		canCast(),
		cast(),
		typeKind(tkEmpty)
	{
	}

	constexpr UnifiedType(
		const TypeKind typeKind,
		FuncConstruct construct,
		FuncGetAddress getAddress,
		FuncCanCast canCast,
		FuncCast cast
	) :
		construct(construct),
		getAddress(getAddress),
		canCast(canCast),
		cast(cast),
		typeKind(typeKind)
	{
	}

	TypeKind getTypeKind() const {
		return typeKind;
	}

	FuncConstruct construct;
	FuncGetAddress getAddress;
	FuncCanCast canCast;
	FuncCast cast;

private:
	TypeKind typeKind;
};

constexpr UnifiedType emptyUnifiedType;

class MetaType
{
public:
	constexpr MetaType() :
		unifiedType(&emptyUnifiedType),
		upType(),
		qualifiers()
	{
	}

	constexpr MetaType(
		const UnifiedType * unifiedType,
		const MetaType * upType,
		const QualifierKind qualifiers
	) :
		unifiedType(unifiedType),
		upType(upType),
		qualifiers(qualifiers)
	{
	}

	const UnifiedType * getUnifiedType() const {
		return unifiedType;
	}

	const MetaType * getUpType() const {
		return upType;
	}

	TypeKind getTypeKind() const {
		return unifiedType->getTypeKind();
	}

	bool isConst() const {
		return qualifiers & qkVolatile;
	}

	bool isVolatile() const {
		return qualifiers & qkVolatile;
	}

	void construct(MetaTypeData & data, const void * value) const {
		unifiedType->construct(data, value);
	}
	const void * getAddress(const MetaTypeData & data) const {
		return unifiedType->getAddress(data);
	}

	bool canCast(const MetaType * toMetaType) const {
		return unifiedType->canCast(toMetaType);
	}
	
	void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const {
		unifiedType->cast(data, toMetaType, toData);
	}

private:
	const UnifiedType * unifiedType;
	const MetaType * upType;
	QualifierKind qualifiers;
};

constexpr MetaType emptyMetaType;

template <typename T>
const UnifiedType * getUnifiedType()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType (
		M::typeKind,
		&M::construct,
		&M::getAddress,
		&M::canCast,
		&M::cast
	);
	return &unifiedType;
}

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type
{
	return nullptr;
}

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<! std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type
{
	using M = DeclareMetaType<T>;

	static const MetaType metaType (
		getUnifiedType<typename std::remove_cv<T>::type>(),
		doGetMetaType<typename M::UpType>(),
		M::qualifiers
	);
	return &metaType;
}

template <typename T>
const MetaType * getMetaType()
{
	return doGetMetaType<typename std::remove_cv<T>::type>();
}

template <typename T>
struct DeclarePodMetaType : public internal_::DeclareMetaTypeBase<T>
{
	static void construct(MetaTypeData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static const void * getAddress(const MetaTypeData & data) {
		return &data.podAs<T>();
	}

};

template <typename T>
struct DeclareObjectMetaType : public internal_::DeclareMetaTypeBase<T>
{
	static constexpr TypeKind typeKind = tkObject;

	using U = typename std::remove_reference<T>::type;

	static void construct(MetaTypeData & data, const void * value) {
		data.object = std::make_shared<U>(*(U *)value);
	}

	static const void * getAddress(const MetaTypeData & data) {
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
		result.push_back(metaType->getTypeKind());
		metaType = metaType->getUpType();
	}
	return result;
}

inline bool probablySame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode = false)
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
