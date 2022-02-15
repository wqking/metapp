#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "metapp/typekind.h"
#include "metapp/metatypedata.h"
#include "metapp/metatypedatautil.h"
#include "metapp/exception.h"
#include "metapp/internal/metatype_i.h"

#include <type_traits>
#include <initializer_list>
#include <istream>
#include <ostream>

namespace metapp {

class MetaType;

template <typename T, typename Enabled = void>
struct BaseDeclareMetaType;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

template <typename T>
const MetaType * getMetaType();

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
		internal_::FuncMetaMethod metaMethod
	) :
		typeKind(typeKind),
		metaMethod(metaMethod)
	{
	}

	TypeKind getTypeKind() const {
		return typeKind;
	}

	void constructDefault(MetaTypeData & data) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::constructDefault;
		param.paramConstruct = { &data };
		metaMethod(param);
	}

	void constructWith(MetaTypeData & data, const void * value) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::constructWith;
		param.paramConstructWith = { &data, value };
		metaMethod(param);
	}

	const void * getAddress(const MetaTypeData & data) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::getAddress;
		param.paramGetAddress = { &data, nullptr };
		metaMethod(param);
		return param.paramGetAddress.value;
	}

	bool canCast(const MetaType * toMetaType) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::canCast;
		param.paramCanCast = { toMetaType, false };
		metaMethod(param);
		return param.paramCanCast.result;
	}

	void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::cast;
		param.paramCast = { &data, toMetaType, toData };
		metaMethod(param);
	}

	void streamIn(std::istream & stream, MetaTypeData & data) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::streamIn;
		param.paramStreamIn = { &stream, &data };
		metaMethod(param);
	}

	void streamOut(std::ostream & stream, const MetaTypeData & data) const {
		internal_::MetaMethodParam param;
		param.action = internal_::MetaMethodAction::streamOut;
		param.paramStreamOut = { &stream, &data };
		metaMethod(param);
	}

private:
	TypeKind typeKind;
	internal_::FuncMetaMethod metaMethod;
};

constexpr UnifiedType emptyUnifiedType;

template <typename T>
struct DeclareMetaTypeBase
{
	using UpType = internal_::NoneUpType;

	static constexpr TypeFlags typeFlags = 0;

	static const void * getAddress(const MetaTypeData & /*data*/) {
		return nullptr;
	}

	static bool canCast(const MetaType * toMetaType) {
		return isPossibleSame(getMetaType<T>(), toMetaType, true);
	}

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/ , void * toData) {
		const void * value = getMetaType<T>()->getAddress(data);
		using U = typename std::remove_reference<T>::type;
		doCast((const U *)value, (U *)toData);
	}

	static void streamIn(std::istream & /*stream*/, MetaTypeData & /*data*/) {
		errorNoStreamIn();
	}

	static void streamOut(std::ostream & /*stream*/, const MetaTypeData & /*data*/) {
		errorNoStreamOut();
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

	void streamIn(std::istream & stream, MetaTypeData & data) const {
		unifiedType->streamIn(stream, data);
	}

	void streamOut(std::ostream & stream, const MetaTypeData & data) const {
		unifiedType->streamOut(stream, data);
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
		&internal_::commonMetaMethod<M>
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

	static void streamIn(std::istream & stream, MetaTypeData & data) {
		podStreamIn<T>(stream, data);
	}

	static void streamOut(std::ostream & stream, const MetaTypeData & data) {
		podStreamOut<T>(stream, data);
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

	static void streamIn(std::istream & stream, MetaTypeData & data) {
		objectStreamIn<T>(stream, data);
	}

	static void streamOut(std::ostream & stream, const MetaTypeData & data) {
		objectStreamOut<T>(stream, data);
	}

};

template <typename T, typename Enabled>
struct BaseDeclareMetaType : public DeclareObjectMetaType<T>
{
};

} // namespace metapp

#endif
