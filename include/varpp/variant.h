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
			metaType(&emptyMetaTypeData),
			data()
	{
	}

	template <typename T>
	explicit Variant(T value) noexcept
		:
			metaType(getMetaTypeData<MetaType<typename std::remove_cv<T>::type> >())
	{
		metaType->construct(data, &value);
	}

	Variant(const Variant & other) noexcept
		:
			metaType(other.metaType)
	{
		metaType->copy(other.data, data);
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
		metaType = getMetaTypeData<MetaType<typename std::remove_cv<T>::type> >();
		metaType->construct(data, &value);
		return *this;
	}

	template <typename T>
	T get() const {
		return *(typename std::remove_reference<T>::type *)(metaType->getAddress(data));
	}

	template <typename T>
	bool canCast() const {
		const MetaTypeData * toMetaTypeData = getMetaTypeData<MetaType<typename std::remove_cv<T>::type> >();
		return metaType->canCast(toMetaTypeData);
	}

	template <typename T>
	T cast() const {
		const MetaTypeData * toMetaTypeData = getMetaTypeData<MetaType<typename std::remove_cv<T>::type> >();
		assert(metaType->canCast(toMetaTypeData));
		T result = T();
		metaType->cast(data, toMetaTypeData, &result);
		return result;
	}

	VarType getVarType() const {
		return metaType->getVarType();
	}

	ExtendType getExtendType() const {
		return metaType->getExtendType();
	}

private:
	const MetaTypeData * metaType;
	VariantData data;
};

inline bool isPointer(const Variant & v)
{
	return isPointer(v.getExtendType());
}

inline bool isReference(const Variant & v)
{
	return isReference(v.getExtendType());
}

inline bool isAnyExtension(const Variant & v)
{
	return isAnyExtension(v.getExtendType());
}


} // namespace varpp

#endif
