#ifndef VARIANT_H_969872685611
#define VARIANT_H_969872685611

#include "varpp/metatype.h"
#include "varpp/common.h"

#include <type_traits>

namespace varpp {

class Variant
{
public:
	Variant() noexcept
		:
			metaType(&emptyMetaType),
			data()
	{
	}

	template <typename T>
	explicit Variant(T value) noexcept
		:
			metaType(varpp::getMetaType<T>())
	{
		metaType->construct(data, &value);
	}

	Variant(const Variant & other) noexcept
		:
			metaType(other.metaType),
			data(other.data)
	{
	}

	Variant(Variant && other) noexcept
		:
			metaType(std::move(other.metaType)),
			data(std::move(other.data))
	{
	}

	template <typename T>
	Variant & set(T value)
	{
		metaType = varpp::getMetaType<T>();
		metaType->construct(data, &value);
		return *this;
	}

	template <typename T>
	T get() const {
		return *(typename std::remove_reference<T>::type *)(metaType->getAddress(data));
	}

	template <typename T>
	bool canCast() const {
		const MetaType * toMetaType = varpp::getMetaType<T>();
		return metaType->canCast(toMetaType);
	}

	template <typename T>
	T cast() const {
		const MetaType * toMetaType = varpp::getMetaType<T>();
		assert(metaType->canCast(toMetaType));
		T result = T();
		metaType->cast(data, toMetaType, &result);
		return result;
	}

	TypeKind getVarType() const {
		return metaType->getVarType();
	}

	const MetaType * getMetaType() const {
		return metaType;
	}

private:
	const MetaType * metaType;
	VariantData data;
};


} // namespace varpp

#endif
