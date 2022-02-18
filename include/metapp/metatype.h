#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "metapp/typekind.h"
#include "metapp/metatypedata.h"
#include "metapp/exception.h"
#include "metapp/typelist.h"

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

template <typename T, typename Enabled = void>
struct DeclareMetaTypeBase;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

template <typename T>
const MetaType * getMetaType();

class UnifiedType
{
public:
	constexpr UnifiedType(const char * name, const TypeKind typeKind, internal_::FuncMetaMethod metaMethod) noexcept;
	~UnifiedType() = default;

	const char * getName() const noexcept;
	TypeKind getTypeKind() const noexcept;

	void constructDefault(MetaTypeData & data) const;
	void constructWith(MetaTypeData & data, const void * value) const;
	
	void * getAddress(const MetaTypeData & data) const;
	
	bool canCast(const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;
	
	bool canInvoke(const Variant * arguments) const;
	Variant invoke(void * instance, const Variant & func, const Variant * arguments) const;

	void streamIn(std::istream & stream, Variant & value) const;
	void streamOut(std::ostream & stream, const Variant & value) const;

private:
	UnifiedType() = delete;
	UnifiedType(const UnifiedType &) = delete;
	UnifiedType(UnifiedType &&) = delete;

private:
	const char * name;
	TypeKind typeKind;
	internal_::FuncMetaMethod metaMethod;
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

	const char * getName() const noexcept;
	TypeKind getTypeKind() const noexcept;

	constexpr bool isConst() const noexcept;
	constexpr bool isVolatile() const noexcept;
	constexpr bool isCallable() const noexcept;
	constexpr bool isPodStorage() const noexcept;

	// meta methods
	void constructDefault(MetaTypeData & data) const;
	void constructWith(MetaTypeData & data, const void * value) const;

	void * getAddress(const MetaTypeData & data) const;
	
	bool canCast(const MetaType * toMetaType) const;
	Variant cast(const Variant & value, const MetaType * toMetaType) const;
	
	bool canInvoke(const Variant * arguments) const;
	Variant invoke(void * instance, const Variant & func, const Variant * arguments) const;

	void streamIn(std::istream & stream, Variant & value) const;
	void streamOut(std::ostream & stream, const Variant & value) const;

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

	static const char * getName() {
		return "";
	}

	static void * getAddress(const MetaTypeData & /*data*/) {
		return nullptr;
	}

	static bool canCast(const MetaType * toMetaType) {
		return isPossibleSame(getMetaType<T>(), toMetaType, true);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		using U = typename std::remove_reference<T>::type;
		return doCast<U>(value, toMetaType);
	}

	static bool canInvoke(const Variant * /*arguments*/)
	{
		return false;
	}

	static Variant invoke(void * /*instance*/, const Variant & /*func*/, const Variant * /*arguments*/)
	{
		throw NotSupportedException("Invoke is not supported.");
	}

	static void streamIn(std::istream & /*stream*/, Variant & /*value*/) {
		errorNoStreamIn();
	}

	static void streamOut(std::ostream & /*stream*/, const Variant & /*value*/) {
		errorNoStreamOut();
	}

private:
	template <typename U>
	static Variant doCast(const Variant & /*value*/, const MetaType * /*toMetaType*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
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
		M::getName(),
		M::typeKind,
		&internal_::commonMetaMethod<M>
	);
	return &unifiedType;
}

template <typename T>
const MetaType * getMetaType();

template <typename T>
struct DeclarePodMetaType : public DeclareMetaTypeRoot<T>
{
	static constexpr TypeFlags typeFlags = tfPodStorage;

	static void constructDefault(MetaTypeData & data) {
		data.podAs<T>() = T();
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static void * getAddress(const MetaTypeData & data) {
		return &data.podAs<T>();
	}

	static void streamIn(std::istream & stream, Variant & value) {
		variantStreamIn<T>(stream, value);
	}

	static void streamOut(std::ostream & stream, const Variant & value) {
		variantStreamOut<T>(stream, value);
	}

};

template <typename T>
struct DeclareObjectMetaType : public DeclareMetaTypeRoot<T>
{
	using U = typename std::remove_reference<T>::type;

	static void constructDefault(MetaTypeData & data) {
		data.object = std::make_shared<U>();
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.object = std::make_shared<U>(*(U *)value);
	}

	static void * getAddress(const MetaTypeData & data) {
		return data.object.get();
	}

	static void streamIn(std::istream & stream, Variant & value) {
		variantStreamIn<T>(stream, value);
	}

	static void streamOut(std::ostream & stream, const Variant & value) {
		variantStreamOut<T>(stream, value);
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

	static const char * getName() {
		return "void";
	}

	static void constructDefault(MetaTypeData & /*data*/) {
	}

	static void constructWith(MetaTypeData & /*data*/, const void * /*value*/) {
	}

};

template <typename T, typename U>
Variant podCast(const MetaTypeData & data)
{
	return (U)(data.podAs<T>());
}


template <typename Iterator>
bool matchUpTypeKinds(const MetaType * metaType, Iterator begin, Iterator end);

template <typename T>
bool matchUpTypeKinds(const MetaType * metaType, const std::initializer_list<T> & typeKindList);

template <typename T, typename U>
bool matchUpTypeKinds(const MetaType * metaType, const U & typeKindList);

bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode);

} // namespace metapp

#include "metapp/variant.h"

#include "metapp/implement/metatype_impl.h"

#endif
