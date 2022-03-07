#ifndef METAPP_METATYPE_H_969872685611
#define METAPP_METATYPE_H_969872685611

#include "metapp/compiler.h"
#include "metapp/typekind.h"
#include "metapp/metatypedata.h"
#include "metapp/exception.h"
#include "metapp/utils/typelist.h"
#include "metapp/utils/knowntypes.h"

// The ugly macro trick is to bypass cyclic include variant.h and metatype.h
#define METAPP_VARIANT_IMPL_H_969872685611
#include "metapp/variant.h"
#undef METAPP_VARIANT_IMPL_H_969872685611

#include <type_traits>
#include <initializer_list>
#include <istream>
#include <ostream>

namespace metapp {

template <typename T, typename Enabled = void>
struct DeclareMetaType;
template <typename T>
struct DeclareMetaTypeObject;

} // namespace metapp
 
// Must be included after previous macro hack
#include "metapp/implement/internal/metatype_i.h"

namespace metapp {

class MetaType;
class UnifiedType;

class MetaClass;
class MetaCallable;
class MetaAccessible;
class MetaArray;
class MetaEnum;
class MetaIndexable;
class MetaIterable;
class MetaStreaming;
class MetaMap;
class MetaMember;

template <typename T>
const MetaType * getMetaType();

template <typename T>
const UnifiedType * getUnifiedType();

class UnifiedType
{
public:
	UnifiedType() = delete;
	UnifiedType(const UnifiedType &) = delete;
	UnifiedType(UnifiedType &&) = delete;

	~UnifiedType() = default;

	TypeKind getTypeKind() const noexcept;

	const MetaClass * getMetaClass() const;
	const MetaCallable * getMetaCallable() const;
	const MetaAccessible * getMetaAccessible() const;
	const MetaEnum * getMetaEnum() const;
	const MetaIndexable * getMetaIndexable() const;
	const MetaIterable * getMetaIterable() const;
	const MetaStreaming * getMetaStreaming() const;
	const MetaMap * getMetaMap() const;
	const MetaMember * getMetaMember() const;
	const void * getMetaUser() const;

	void * constructData(MetaTypeData * data, const void * copyFrom) const;
	
	void destroy(void * instance) const;

	void * getAddress(const MetaTypeData & data) const;
	
	bool canCast(const Variant & value, const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;
	
	bool canCastFrom(const Variant & value, const MetaType * fromMetaType) const;
	Variant castFrom(const Variant & value, const MetaType * fromMetaType) const;

private:
	constexpr UnifiedType(
		const TypeKind typeKind,
		const internal_::MetaMethodTable & metaMethodTable
	) noexcept;

	template <typename T>
	friend const UnifiedType * getUnifiedType();

	const void * doGetMetaInterface(const internal_::MetaInterfaceKind kind) const;

private:
	TypeKind typeKind;
	internal_::MetaMethodTable metaMethodTable;
};

namespace internal_ {

bool areMetaTypesMatched(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode);

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<std::is_same<T, NoneUpType>::value, const MetaType *>::type;
template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<! std::is_same<T, NoneUpType>::value, const MetaType *>::type;
} // namespace internal_

class MetaType
{
public:
	MetaType() = delete;
	MetaType(const MetaType &) = delete;
	MetaType(MetaType &&) = delete;

	~MetaType() = default;

	const UnifiedType * getUnifiedType() const noexcept;

	const MetaType * getUpType() const noexcept;
	const MetaType * getUpType(const size_t i) const;
	size_t getUpTypeCount() const noexcept;

	TypeKind getTypeKind() const noexcept;

	constexpr bool isConst() const noexcept;
	constexpr bool isVolatile() const noexcept;
	constexpr bool isPointer() const noexcept;
	constexpr bool isReference() const noexcept;
	
	const MetaClass * getMetaClass() const;
	const MetaCallable * getMetaCallable() const;
	const MetaAccessible * getMetaAccessible() const;
	const MetaEnum * getMetaEnum() const;
	const MetaIndexable * getMetaIndexable() const;
	const MetaIterable * getMetaIterable() const;
	const MetaStreaming * getMetaStreaming() const;
	const MetaMap * getMetaMap() const;
	const MetaMember * getMetaMember() const;
	const void * getMetaUser() const;

	void * construct() const;
	void * copyConstruct(const void * copyFrom) const;

	// re-implementable meta methods
	void * constructData(MetaTypeData * data, const void * copyFrom) const;

	void destroy(void * instance) const;

	void * getAddress(const MetaTypeData & data) const;
	
	bool canCast(const Variant & value, const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;

	bool canCastFrom(const Variant & value, const MetaType * fromMetaType) const;
	Variant castFrom(const Variant & value, const MetaType * fromMetaType) const;

private:
	constexpr MetaType(
		const UnifiedType * unifiedType,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept;

	template <typename T>
	friend auto internal_::doGetMetaType()
		-> typename std::enable_if<! std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type;

private:
	const UnifiedType * unifiedType;
	internal_::UpTypeData upTypeData;
	TypeFlags typeFlags;
};

} // namespace metapp

#include "metapp/variant.h"
#include "metapp/interfaces/bases/metastreamingbase.h"
#include "metapp/implement/internal/cast_i.h"

namespace metapp {

namespace internal_ {

// Non-template base class to reduce binary size
struct DeclareMetaTypeObjectBase
{
	static void * getAddress(const MetaTypeData & data)
	{
		return data.getAddress();
	}

};

template <typename U>
static void * doConstructDefault(
	typename std::enable_if<std::is_default_constructible<U>::value>::type * = nullptr
) {
	return new U();
}

template <typename U>
static void * doConstructDefault(
	typename std::enable_if<! (std::is_default_constructible<U>::value)>::type * = nullptr
) {
	return nullptr;
}

template <typename U>
static void * doConstructCopy(
	const void * copyFrom,
	typename std::enable_if<std::is_copy_assignable<U>::value>::type * = nullptr
) {
	return new U(*(U *)copyFrom);
}

template <typename U>
static void * doConstructCopy(
	const void * /*copyFrom*/,
	typename std::enable_if<! std::is_copy_assignable<U>::value>::type * = nullptr
) {
	return nullptr;
}


} // namespace internal_

template <typename T>
struct DeclareMetaTypeObject
	: public internal_::DeclareMetaTypeObjectBase,
		public SelectMetaStreamingBase<T>
{
private:
	using Underlying = typename std::decay<typename std::remove_reference<T>::type>::type;
	using NoCV = typename std::remove_cv<T>::type;
	using Decayed = typename std::decay<NoCV>::type;

	static_assert(! std::is_void<T>::value, "DeclareMetaTypeObject can't be initialized with void.");

public:
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkObject;
	static constexpr TypeFlags typeFlags = 0
		| (std::is_pointer<Decayed>::value ? tfPointer : 0)
		| (std::is_reference<T>::value ? tfReference : 0)
	;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->construct<Underlying>(copyFrom);
			return nullptr;
		}
		else {
			if(copyFrom == nullptr) {
				return internal_::doConstructDefault<Underlying>();
			}
			else {
				return internal_::doConstructCopy<Underlying>(copyFrom);
			}
		}
	}

	static void destroy(void * instance)
	{
#if defined(METAPP_COMPILER_GCC) || defined(METAPP_COMPILER_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
		delete static_cast<Underlying *>(instance);
#if defined(METAPP_COMPILER_GCC) || defined(METAPP_COMPILER_CLANG)
#pragma GCC diagnostic pop
#endif
	}

	static bool canCast(const Variant & value, const MetaType * toMetaType)
	{
		const MetaType * fromMetaType = getMetaType<T>();
		if(internal_::areMetaTypesMatched(fromMetaType, toMetaType, true)) {
			return true;
		}
		if(fromMetaType->getTypeKind() != tkReference
			&& toMetaType->getTypeKind() == tkReference
			&& fromMetaType->canCast(value, toMetaType->getUpType())
			) {
			return true;
		}
		if(internal_::CastTo<Underlying>::canCastTo(value, toMetaType)) {
			return true;
		}
		if(toMetaType->canCastFrom(value, fromMetaType)) {
			return true;
		}
		return false;
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType)
	{
		const MetaType * fromMetaType = getMetaType<T>();
		if(internal_::areMetaTypesMatched(fromMetaType, toMetaType, true)) {
			return Variant::retype(toMetaType, value);
		}
		if(fromMetaType->getTypeKind() != tkReference
			&& toMetaType->getTypeKind() == tkReference
			&& fromMetaType->canCast(value, toMetaType->getUpType())
			) {
			return fromMetaType->cast(value, toMetaType->getUpType());
		}
		if(internal_::CastTo<Underlying>::canCastTo(value, toMetaType)) {
			return internal_::CastTo<Underlying>::castTo(value, toMetaType);
		}
		if(toMetaType->canCastFrom(value, fromMetaType)) {
			return toMetaType->castFrom(value, fromMetaType);
		}
		errorBadCast();
		return Variant();
	}

	static bool canCastFrom(const Variant & value, const MetaType * fromMetaType)
	{
		return internal_::CastFrom<Underlying>::canCastFrom(value, fromMetaType);
	}

	static Variant castFrom(const Variant & value, const MetaType * fromMetaType)
	{
		return internal_::CastFrom<Underlying>::castFrom(value, fromMetaType);
	}

};

template <typename T, typename Enabled = void>
struct DeclareMetaTypeBase : public DeclareMetaTypeObject<T>
{
};

template <typename T, typename Enabled>
struct DeclareMetaType : public DeclareMetaTypeBase<T>
{
};

template <>
struct DeclareMetaTypeObject <void>
{
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkVoid;
	static constexpr TypeFlags typeFlags = 0;

	static void * constructData(MetaTypeData * /*data*/, const void * /*value*/) {
		return nullptr;
	}

	static void destroy(void * /*instance*/)
	{
	}

	static void * getAddress(const MetaTypeData & /*data*/)
	{
		return nullptr;
	}

	static bool canCast(const Variant & /*value*/, const MetaType * /*toMetaType*/)
	{
		return false;
	}

	static Variant cast(const Variant & /*value*/, const MetaType * /*toMetaType*/)
	{
		errorBadCast();
		return Variant();
	}

	static bool canCastFrom(const Variant & /*value*/, const MetaType * /*fromMetaType*/)
	{
		return false;
	}

	static Variant castFrom(const Variant & /*value*/, const MetaType * /*fromMetaType*/)
	{
		errorBadCast();
		return Variant();
	}

};

} // namespace metapp

#include "metapp/implement/metatype_impl.h"

namespace metapp {

template <typename T>
using GetMetaTypeFlags = typename internal_::SelectDeclareClass<T, internal_::HasMember_typeFlags<DeclareMetaType<T> >::value>;

} // namespace metapp


#endif
