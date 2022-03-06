#ifndef METAPP_VARIANT_H_969872685611
#define METAPP_VARIANT_H_969872685611

#include "metapp/metatypedata.h"
#include "metapp/typekind.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/exception.h"

#include <type_traits>
#include <cassert>

namespace metapp {

class MetaType;

template <typename T>
const MetaType * getMetaType();

class Variant
{
public:
	template <typename T>
	static Variant create(T value, const bool copyVariant = true,
		typename std::enable_if<
			std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
	>::type * = 0);
	template <typename T>
	static Variant create(T value, const bool copyVariant = true,
		typename std::enable_if<
			! std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
	>::type * = 0);
	static Variant retype(const MetaType * metaType, const Variant & var);
	static Variant takeFrom(const MetaType * metaType, void * object);
	static Variant takeFrom(const Variant & object);

	Variant() noexcept;
	~Variant() = default;

	template <typename T>
	Variant(T value) noexcept;

	explicit Variant(const MetaType * metaType) noexcept;
	Variant(const MetaType * metaType, const void * copyFrom) noexcept;

	Variant(const Variant & other) noexcept;
	Variant(Variant && other) noexcept;

	template <typename T>
	Variant & operator = (T value) noexcept;
	Variant & operator = (const Variant & other) noexcept;
	Variant & operator = (Variant && other) noexcept;

	Variant clone() const;

	template <typename T>
	bool canGet() const;

	template <typename T>
	auto get() const -> typename internal_::VariantReturnType<T>::Type;

	void * getAddress() const;
	Variant dereference() const;

	bool canCast(const MetaType * toMetaType) const;
	template <typename T>
	bool canCast() const;

	Variant cast(const MetaType * toMetaType) const;
	template <typename T>
	Variant cast() const;

	bool isEmpty() const noexcept;

	const MetaType * getMetaType() const noexcept;

	friend std::istream & operator >> (std::istream & stream, Variant & v);
	friend std::ostream & operator << (std::ostream & stream, const Variant & v);

private:
	const MetaType * metaType;
	MetaTypeData data;
};

TypeKind getTypeKind(const Variant & v);

template <typename T, typename U>
Variant variantCast(const Variant & value);


} // namespace metapp


#endif

#include "metapp/metatype.h"

#include "metapp/implement/variant_impl.h"

