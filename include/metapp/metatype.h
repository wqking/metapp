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

enum class MetaMethodAction
{
	constructDefault,
	constructWith,
	getAddress,
	canCast,
	cast,
};

struct MetaMethodParam
{
	MetaMethodAction action;
	const MetaType * metaType;
	MetaTypeData * data;
	const void * constValue;
	void * writableValue;
	bool result;
};

using FuncMetaMethod = void (*)(MetaMethodParam & param);

class UnifiedType
{
public:
	constexpr UnifiedType() :
		typeKind(tkEmpty),
		metaMethod()
	{
	}

	constexpr UnifiedType(
		const TypeKind typeKind,
		FuncMetaMethod metaMethod
	) :
		typeKind(typeKind),
		metaMethod(metaMethod)
	{
	}

	TypeKind getTypeKind() const {
		return typeKind;
	}

	void constructDefault(MetaTypeData & data) const {
		MetaMethodParam param {
			MetaMethodAction::constructDefault,
			nullptr,
			&data,
			nullptr,
			nullptr,
			false
		};
		metaMethod(param);
	}

	void constructWith(MetaTypeData & data, const void * value) const {
		MetaMethodParam param {
			MetaMethodAction::constructWith,
			nullptr,
			&data,
			value,
			nullptr,
			false
		};
		metaMethod(param);
	}

	const void * getAddress(const MetaTypeData & data) const {
		MetaMethodParam param {
			MetaMethodAction::getAddress,
			nullptr,
			const_cast<MetaTypeData *>(&data),
			nullptr,
			nullptr,
			false
		};
		metaMethod(param);
		return param.constValue;
	}

	bool canCast(const MetaType * toMetaType) const {
		MetaMethodParam param {
			MetaMethodAction::canCast,
			toMetaType,
			nullptr,
			nullptr,
			nullptr,
			false
		};
		metaMethod(param);
		return param.result;
	}

	void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const {
		MetaMethodParam param {
			MetaMethodAction::cast,
			toMetaType,
			const_cast<MetaTypeData *>(&data),
			nullptr,
			toData,
			false
		};
		metaMethod(param);
	}

private:
	TypeKind typeKind;
	FuncMetaMethod metaMethod;
};

template <typename M>
void commonMetaMethod(MetaMethodParam & param)
{
	switch(param.action) {
	case MetaMethodAction::constructDefault:
		M::constructDefault(*(param.data));
		break;

	case MetaMethodAction::constructWith:
		M::constructWith(*(param.data), param.constValue);
		break;

	case MetaMethodAction::getAddress:
		param.constValue = M::getAddress(*(param.data));
		break;

	case MetaMethodAction::canCast:
		param.result = M::canCast(param.metaType);
		break;

	case MetaMethodAction::cast:
		M::cast(*(param.data), param.metaType, param.writableValue);
		break;
	}
}

constexpr UnifiedType emptyUnifiedType;

struct NoneUpType {};

template <typename T>
struct DeclareMetaTypeBase
{
	using UpType = NoneUpType;

	static constexpr TypeFlags typeFlags = 0;

	static const void * getAddress(const MetaTypeData & /*data*/) {
		return nullptr;
	}

	static bool canCast(const MetaType * toMetaType) {
		return probablySame(getMetaType<T>(), toMetaType, true);
	}

	static void cast(const MetaTypeData & data , const MetaType * /*toMetaType*/ , void * toData) {
		const void * value = getMetaType<T>()->getAddress(data);
		using U = typename std::remove_reference<T>::type;
		doCast((const U *)value, (U *)toData);
	}

private:
	template <typename U>
	static void doCast(const U * /*value*/, U * /*toData*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
	}

	template <typename U>
	static void doCast(const U * value , U * toData, typename std::enable_if<! std::is_void<U>::value>::type * = nullptr) {
		*toData = *value;
	}
};

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

	void constructDefault(MetaTypeData & data) const {
		unifiedType->constructDefault(data);
	}

	void constructWith(MetaTypeData & data, const void * value) const {
		unifiedType->constructWith(data, value);
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
		&commonMetaMethod<M>
	);
	return &unifiedType;
}

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<std::is_same<T, NoneUpType>::value, const MetaType *>::type
{
	return nullptr;
}

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<! std::is_same<T, NoneUpType>::value, const MetaType *>::type
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
struct DeclarePodMetaType : public DeclareMetaTypeBase<T>
{
	static constexpr TypeFlags typeFlags = tfPodStorage;

	static void constructDefault(MetaTypeData & data) {
		data.podAs<T>() = T();
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static const void * getAddress(const MetaTypeData & data) {
		return &data.podAs<T>();
	}

};

template <typename T>
struct DeclareObjectMetaType : public DeclareMetaTypeBase<T>
{
	static constexpr TypeKind typeKind = tkObject;

	using U = typename std::remove_reference<T>::type;

	static void constructDefault(MetaTypeData & data) {
		data.object = std::make_shared<U>();
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.object = std::make_shared<U>(*(U *)value);
	}

	static const void * getAddress(const MetaTypeData & data) {
		return data.object.get();
	}

};

template <typename T, typename Enabled>
struct BaseDeclareMetaType : public DeclareObjectMetaType<T>
{
};

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

} // namespace metapp

#endif
