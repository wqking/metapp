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
struct CommonDeclareMetaType;

} // namespace metapp
 
namespace metapp {

class MetaType;

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

} // namespace metapp

// Must be included after previous macro hack
#include "metapp/implement/internal/metatype_i.h"

namespace metapp {

template <typename T>
const MetaType * getMetaType();

namespace internal_ {

bool areMetaTypesMatched(const MetaType * fromMetaType, const MetaType * toMetaType);

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

	const void * getUnifiedType() const noexcept;

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

	// re-implementable meta methods, begin
	void * constructData(MetaTypeData * data, const void * copyFrom) const;

	void destroy(void * instance) const;

	Variant toReference(const Variant & value) const;
	
	bool canCast(const Variant & value, const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;

	bool canCastFrom(const Variant & value, const MetaType * fromMetaType) const;
	Variant castFrom(const Variant & value, const MetaType * fromMetaType) const;
	// re-implementable meta methods, end

private:
	constexpr MetaType(
		const internal_::UnifiedType * (*doGetUnifiedType)(),
		const internal_::MetaTable & metaTable,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept;

	template <typename T>
	friend auto internal_::doGetMetaType()
		-> typename std::enable_if<! std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type;

private:
	const internal_::UnifiedType * (*doGetUnifiedType)();

#ifdef METAPP_DEBUG_ENABLED
	// need this in debug window to ease debugging
	const internal_::UnifiedType * debugUnifiedType;
#endif

	internal_::MetaTable metaTable;
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
struct CommonDeclareMetaTypeBase
{
protected:
	template <typename U>
	static void * doConstructDefault(
		typename std::enable_if<std::is_default_constructible<U>::value>::type * = nullptr
	);

	template <typename U>
	static void * doConstructDefault(
		typename std::enable_if<! (std::is_default_constructible<U>::value)>::type * = nullptr
	);

	template <typename U>
	static void * doConstructCopy(
		const void * copyFrom,
		typename std::enable_if<std::is_copy_assignable<U>::value>::type * = nullptr
	);

	template <typename U>
	static void * doConstructCopy(
		const void * /*copyFrom*/,
		typename std::enable_if<! std::is_copy_assignable<U>::value>::type * = nullptr
	);

	template <typename P>
	static Variant doToReference(const Variant & value, typename std::enable_if<! std::is_void<P>::value>::type * = nullptr);

	template <typename P>
	static Variant doToReference(const Variant & value, typename std::enable_if<std::is_void<P>::value>::type * = nullptr);

	static void checkCanToReference(const MetaType * fromMetaType, const MetaType * myMetaType);

	static bool doCanCast(const MetaType * fromMetaType, const Variant & value, const MetaType * toMetaType);
	static bool doCast(Variant & result, const MetaType * fromMetaType, const Variant & value, const MetaType * toMetaType);

};


} // namespace internal_

template <typename T>
struct CommonDeclareMetaType
	: public internal_::CommonDeclareMetaTypeBase,
		public SelectMetaStreamingBase<T>
{
private:
	using Underlying = typename std::decay<typename std::remove_reference<T>::type>::type;
	using NoCV = typename std::remove_cv<T>::type;
	using Decayed = typename std::decay<NoCV>::type;

	static_assert(! std::is_void<T>::value, "CommonDeclareMetaType can't be initialized with void.");

public:
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkObject;
	static constexpr TypeFlags typeFlags = 0
		| (std::is_pointer<Decayed>::value ? tfPointer : 0)
		| (std::is_reference<T>::value ? tfReference : 0)
	;

	static void * constructData(MetaTypeData * data, const void * copyFrom);
	static void destroy(void * instance);

	static Variant toReference(const Variant & value);

	static bool canCast(const Variant & value, const MetaType * toMetaType);
	static Variant cast(const Variant & value, const MetaType * toMetaType);

	static bool canCastFrom(const Variant & value, const MetaType * fromMetaType);
	static Variant castFrom(const Variant & value, const MetaType * fromMetaType);

};

template <typename T, typename Enabled = void>
struct DeclareMetaTypeRoot
{
};

template <typename T, typename Enabled = void>
struct DeclareMetaTypeBase : public DeclareMetaTypeRoot<T>
{
};

template <typename T, typename Enabled>
struct DeclareMetaType : public DeclareMetaTypeBase<T>
{
};

struct DeclareMetaTypeVoidBase
{
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkVoid;
	static constexpr TypeFlags typeFlags = 0;

	static void * constructData(MetaTypeData * data, const void * copyFrom);
	static void destroy(void * instance);

	static Variant toReference(const Variant & value);

	static bool canCast(const Variant & value, const MetaType * toMetaType);
	static Variant cast(const Variant & value, const MetaType * toMetaType);

	static bool canCastFrom(const Variant & value, const MetaType * fromMetaType);
	static Variant castFrom(const Variant & value, const MetaType * fromMetaType);

};

template<> struct CommonDeclareMetaType<void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<const void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<volatile void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<const volatile void> : DeclareMetaTypeVoidBase {};

} // namespace metapp

#include "metapp/implement/metatype_impl.h"


#endif
