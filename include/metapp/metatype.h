#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "metapp/typekind.h"
#include "metapp/internal/metatype_i.h"

#include <type_traits>
#include <initializer_list>

namespace metapp {

class MetaType;

template <typename T, typename Enabled = void>
struct BaseDeclareMetaType;
template <typename T, typename Enabled = void>
struct DeclareMetaType;

using FuncConstruct = void (*)(MetaTypeData & data, const void * value);
using FuncCanCast = bool (*)(const MetaType * toMetaType);
using FuncCast = void (*)(const MetaTypeData & data, const MetaType * toMetaType, void * toData);

class UnifiedType
{
public:
	constexpr UnifiedType() :
		construct(),
		canCast(),
		cast(),
		typeKind(tkEmpty)
	{
	}

	constexpr UnifiedType(
		const TypeKind typeKind,
		FuncConstruct construct,
		FuncCanCast canCast,
		FuncCast cast
	) :
		construct(construct),
		canCast(canCast),
		cast(cast),
		typeKind(typeKind)
	{
	}

	TypeKind getTypeKind() const {
		return typeKind;
	}

	FuncConstruct construct;
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
		typeFlags()
	{
	}

	constexpr MetaType(
		const UnifiedType * unifiedType,
		const MetaType * upType,
		const TypeFlags typeFlags
	) :
		unifiedType(unifiedType),
		upType(upType),
		typeFlags(typeFlags)
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
		return typeFlags & tfConst;
	}

	bool isVolatile() const {
		return typeFlags & tfVolatile;
	}

	bool isPodStorage() const {
		return typeFlags & tfPodStorage;
	}

	bool isSharedPtrStorage() const {
		return typeFlags & tfSharedPtrStorage;
	}

	void construct(MetaTypeData & data, const void * value) const {
		unifiedType->construct(data, value);
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
	TypeFlags typeFlags;
};

template <typename T, typename Enabled>
struct DeclareMetaType : public BaseDeclareMetaType<T>
{
};

constexpr MetaType emptyMetaType;

template <typename T>
const UnifiedType * getUnifiedType()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType (
		M::typeKind,
		&M::construct,
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
		M::typeFlags
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
	static constexpr TypeFlags typeFlags = tfPodStorage;

	static void construct(MetaTypeData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
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

};

template <typename T>
struct DeclareSharedPtrMetaType : public internal_::DeclareMetaTypeBase<T>
{
public:
	static constexpr TypeFlags typeFlags = tfSharedPtrStorage;

	static void construct(MetaTypeData & data, const void * value) {
		data.object = std::static_pointer_cast<void>(*(T *)value);
	}

};

template <typename T>
struct DeclareReferenceMetaType : public internal_::DeclareMetaTypeBase<typename std::remove_reference<T>::type>
{
private:
	using U = typename std::remove_reference<T>::type;

public:
	static constexpr TypeFlags typeFlags = tfReferenceStorage;

	static void construct(MetaTypeData & data, const void * value) {
		data.podAs<U *>() = (U *)value;
	}

};

template <typename T, typename Enabled>
struct BaseDeclareMetaType : public DeclareObjectMetaType<T>
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

inline std::vector<TypeKind> getUpTypeTypeKinds(const MetaType * metaType)
{
	std::vector<TypeKind> result;
	result.reserve(8);
	while(metaType != nullptr) {
		result.push_back(metaType->getTypeKind());
		metaType = metaType->getUpType();
	}
	return result;
}

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

inline bool probablySame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode)
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

inline const void * getDataAddress(const MetaType * metaType, const MetaTypeData & data)
{
	if(metaType->getTypeKind() == tkReference) {
		return data.podAs<void *>();
	}
	if(metaType->isPodStorage()) {
		return &data.podAs<char>();
	}
	if(metaType->isSharedPtrStorage()) {
		return &data.object;
	}
	return data.object.get();
}



} // namespace metapp

#endif
