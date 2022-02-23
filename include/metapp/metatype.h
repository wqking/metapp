#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "metapp/compiler.h"
#include "metapp/typekind.h"
#include "metapp/metatypedata.h"
#include "metapp/exception.h"
#include "metapp/typelist.h"

// The ugly macro trick is to bypass cyclic include variant.h and metatype.h
#define VARIANT_IMPL_H_969872685611
#include "metapp/variant.h"
#undef VARIANT_IMPL_H_969872685611

#include "metapp/internal/metatype_i.h"

#include <type_traits>
#include <initializer_list>
#include <istream>
#include <ostream>

namespace metapp {

class MetaType;
class MetaClass;

template <typename T, typename Enabled = void>
struct DeclareMetaTypeBase;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

template <typename T>
const MetaType * getMetaType();

class UnifiedType
{
public:
	constexpr UnifiedType(
		const TypeKind typeKind,
		const internal_::MetaMethodTable & metaMethodTable
	) noexcept;
	~UnifiedType() = default;

	TypeKind getTypeKind() const noexcept;

	constexpr bool isCallable() const noexcept;
	constexpr bool isAccessible() const noexcept;

	void * construct(MetaTypeData * data, const void * copyFrom) const;
	
	void destroy(void * instance) const;

	const MetaClass * getMetaClass() const;

	void * getAddress(const MetaTypeData & data) const;
	
	bool canCast(const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;

	void streamIn(std::istream & stream, Variant & value) const;
	void streamOut(std::ostream & stream, const Variant & value) const;

	size_t getParameterCount() const;
	int rankInvoke(const Variant * arguments, const size_t argumentCount) const;
	bool canInvoke(const Variant * arguments, const size_t argumentCount) const;
	Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount) const;

	Variant accessibleGet(const Variant & accessible, const void * instance) const;
	void accessibleSet(const Variant & accessible, void * instance, const Variant & value) const;

private:
	UnifiedType() = delete;
	UnifiedType(const UnifiedType &) = delete;
	UnifiedType(UnifiedType &&) = delete;

private:
	TypeKind typeKind;
	internal_::MetaMethodTable metaMethodTable;
};

class MetaType
{
public:
	constexpr MetaType(
		const UnifiedType * unifiedType,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept;
	~MetaType() = default;

	const UnifiedType * getUnifiedType() const noexcept;

	const MetaType * getUpType() const noexcept;
	const MetaType * getUpType(const size_t i) const;
	size_t getUpTypeCount() const noexcept;

	TypeKind getTypeKind() const noexcept;

	constexpr bool isConst() const noexcept;
	constexpr bool isVolatile() const noexcept;
	constexpr bool isCallable() const noexcept;
	constexpr bool isAccessible() const noexcept;
	constexpr bool isPodStorage() const noexcept;
	constexpr bool isObjectStorage() const noexcept;

	// meta methods
	void * construct(MetaTypeData * data, const void * copyFrom) const;

	void destroy(void * instance) const;

	const MetaClass * getMetaClass() const;

	void * getAddress(const MetaTypeData & data) const;
	
	bool canCast(const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;

	void streamIn(std::istream & stream, Variant & value) const;
	void streamOut(std::ostream & stream, const Variant & value) const;

	size_t getParameterCount() const;
	int rankInvoke(const Variant * arguments, const size_t argumentCount) const;
	bool canInvoke(const Variant * arguments, const size_t argumentCount) const;
	Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount) const;

	Variant accessibleGet(const Variant & accessible, const void * instance) const;
	void accessibleSet(const Variant & accessible, void * instance, const Variant & value) const;

private:
	MetaType() = delete;
	MetaType(const MetaType &) = delete;
	MetaType(MetaType &&) = delete;

private:
	const UnifiedType * unifiedType;
	internal_::UpTypeData upTypeData;
	TypeFlags typeFlags;
};

template <typename T>
struct DeclareMetaTypeRoot
{
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkObject;
	static constexpr TypeFlags typeFlags = 0;

	static void destroy(void * instance)
	{
		using U = typename std::remove_reference<T>::type;
		doDestroy(static_cast<U *>(instance));
	}

	static const MetaClass * getMetaClass()
	{
		return nullptr;
	}

	static void * getAddress(const MetaTypeData & /*data*/)
	{
		return nullptr;
	}

	static bool canCast(const MetaType * toMetaType)
	{
		using U = typename std::remove_reference<T>::type;
		return (! std::is_void<U>::value) && isPossibleSame(getMetaType<T>(), toMetaType, true);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType)
	{
		using U = typename std::remove_reference<T>::type;
		return doCast<U>(value, toMetaType);
	}

	static void streamIn(std::istream & stream, Variant & value) {
		variantStreamIn<T>(stream, value);
	}

	static void streamOut(std::ostream & stream, const Variant & value) {
		variantStreamOut<T>(stream, value);
	}

private:
	template <typename U>
	static void doDestroy(U * /*instance*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
	}

	template <typename U>
	static void doDestroy(U * instance, typename std::enable_if<! std::is_void<U>::value>::type * = nullptr) {
		#if defined(METAPP_COMPILER_GCC) || defined(METAPP_COMPILER_CLANG)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
		#endif
				delete instance;
		#if defined(METAPP_COMPILER_GCC) || defined(METAPP_COMPILER_CLANG)
		#pragma GCC diagnostic pop
		#endif
	}

	template <typename U>
	static Variant doCast(const Variant & /*value*/, const MetaType * /*toMetaType*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
		errorBadCast();
		return Variant();
	}

	template <typename U>
	static Variant doCast(const Variant & value, const MetaType * /*toMetaType*/, typename std::enable_if<! std::is_void<U>::value>::type * = nullptr) {
		return value;
	}
};

template <typename T, typename Enabled>
struct DeclareMetaType : public DeclareMetaTypeBase<T>
{
};

template <typename T>
const UnifiedType * getUnifiedType()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType (
		M::typeKind,
		internal_::MetaMethodTable {
			&M::construct,

			&M::destroy,

			&M::getMetaClass,

			&M::getAddress,

			&M::canCast,
			&M::cast,

			&M::streamIn,
			&M::streamOut,

			internal_::getInvokeMethdTable<M>(),
			internal_::getAccessibleMethodTable<M>(),
		}
	);
	return &unifiedType;
}

template <typename T>
const MetaType * getMetaType();

template <typename T>
struct DeclarePodMetaType : public DeclareMetaTypeRoot<T>
{
	using U = typename std::remove_reference<T>::type;

	static constexpr TypeFlags typeFlags = tfPodStorage;

	static void * construct(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			if(copyFrom == nullptr) {
				data->podAs<U>() = U();
			}
			else {
				data->podAs<U>() = *(U *)copyFrom;
			}
			return nullptr;
		}
		else {
			if(copyFrom == nullptr) {
				return new U();
			}
			else {
				return new U(*(U *)copyFrom);
			}
		}
	}

	static void * getAddress(const MetaTypeData & data) {
		return &data.podAs<T>();
	}

};

template <typename T>
struct DeclareObjectMetaType : public DeclareMetaTypeRoot<T>
{
	using U = typename std::remove_reference<T>::type;

	static void * construct(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			if(copyFrom == nullptr) {
				data->object = std::make_shared<U>();
			}
			else {
				data->object = std::make_shared<U>(*(U *)copyFrom);
			}
			return nullptr;
		}
		else {
			if(copyFrom == nullptr) {
				return new U();
			}
			else {
				return new U(*(U *)copyFrom);
			}
		}
	}

	static void * getAddress(const MetaTypeData & data) {
		return data.object.get();
	}

};

template <typename T, typename Enabled>
struct DeclareMetaTypeBase : 
	public std::conditional<
		! std::is_const<T>::value
		&& ! std::is_volatile<T>::value
		&& std::is_trivial<T>::value
		&& std::is_standard_layout<T>::value
		&& sizeof(T) <= podSize,
		DeclarePodMetaType<T>,
		DeclareObjectMetaType<T>
	>::type
{
};

template <>
struct DeclareMetaTypeBase <void> : public DeclareMetaTypeRoot<void>
{
	static constexpr TypeKind typeKind = tkVoid;

	static void * construct(MetaTypeData * /*data*/, const void * /*value*/) {
		return nullptr;
	}

};

template <typename T, typename U>
Variant podCast(const MetaTypeData & data)
{
	return (U)(data.podAs<T>());
}

bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode);

} // namespace metapp

#include "metapp/variant.h"

#include "metapp/implement/metatype_impl.h"

#endif
