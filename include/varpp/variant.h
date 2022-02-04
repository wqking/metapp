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
			metaType(getMetaTypeData<MetaType<typename MakeVariantType<T>::Type> >())
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
		metaType = getMetaTypeData<MetaType<typename MakeVariantType<T>::Type> >();
		metaType->construct(data, &value);
		return *this;
	}

	template <typename T>
	T getAs() const {
		return *(typename std::remove_reference<T>::type *)(metaType->getAddress(data));
	}

	VarType getVarType() const {
		return metaType->getVarType();
	}

	ExtendType getExtendType() const {
		return metaType->getExtendType();
	}

	bool isPointer() const {
		return (getExtendType() & etPointer);
	}

	bool isReference() const {
		return (getExtendType() & etReference);
	}

private:
	const MetaTypeData * metaType;
	VariantData data;
};


} // namespace varpp

#endif
