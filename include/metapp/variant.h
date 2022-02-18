#ifndef VARIANT_H_969872685611
#define VARIANT_H_969872685611

#include "metapp/metatypedata.h"

#include <type_traits>
#include <cassert>

namespace metapp {

class MetaType;

class Variant
{
public:
	Variant() noexcept;
	~Variant() = default;

	template <typename T>
	Variant(T value) noexcept;

	explicit Variant(const MetaType * metaType) noexcept;

	Variant(const Variant & other) noexcept;
	Variant(Variant && other) noexcept;

	template <typename T>
	Variant & operator = (T value) noexcept;
	Variant & operator = (const Variant & other) noexcept;
	Variant & operator = (Variant && other) noexcept;

	template <typename T>
	Variant & set(T value);

	template <typename T>
	bool canGet(const bool strictMode = false) const;

	template <typename T>
	T get() const;

	template <typename T>
	const T * getAddress() const;

	template <typename T>
	T * getAddress();

	template <typename T>
	bool canCast() const;

	template <typename T>
	Variant cast() const;

	const MetaType * getMetaType() const noexcept;

	friend std::istream & operator >> (std::istream & stream, Variant & v);
	friend std::ostream & operator << (std::ostream & stream, const Variant & v);

private:
	const MetaType * metaType;
	MetaTypeData data;
};

TypeKind getTypeKind(const Variant & v);

template <typename T>
auto variantStreamIn(std::istream & stream, Variant & value)
	-> typename std::enable_if<internal_::HasInputStreamOperator<T>::value, void>::type;

template <typename T>
auto variantStreamIn(std::istream & /*stream*/, Variant & /*value*/)
	-> typename std::enable_if<! internal_::HasInputStreamOperator<T>::value, void>::type;

template <typename T>
auto variantStreamOut(std::ostream & stream, const Variant & value)
	-> typename std::enable_if<internal_::HasOutputStreamOperator<T>::value, void>::type;

template <typename T>
auto variantStreamOut(std::ostream & /*stream*/, const Variant & /*value*/)
	-> typename std::enable_if<! internal_::HasOutputStreamOperator<T>::value, void>::type;

} // namespace metapp


#endif

#include "metapp/implement/variant_impl.h"

#include "metapp/metatype.h"

