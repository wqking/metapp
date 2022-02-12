#ifndef VARIANT_H_969872685611
#define VARIANT_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypedata.h"

#include <type_traits>

namespace metapp {

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
			metaType(metapp::getMetaType<T>())
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
		metaType = metapp::getMetaType<T>();
		metaType->construct(data, &value);
		return *this;
	}

	template <typename T>
	bool canGet(const bool strictMode = false) const {
		return probablySame(metaType, metapp::getMetaType<T>(), strictMode);
	}

	template <typename T>
	T get() const {
		//assert(this->canGet<typename std::remove_reference<T>::type>());
		//if(! this->canGet<typename std::remove_reference<T>::type>()) {
		//	throw std::runtime_error("Can't get");
		//}
		return *(typename std::remove_reference<T>::type *)(metaType->getAddress(data));
	}

	template <typename T>
	bool canCast() const {
		const MetaType * toMetaType = metapp::getMetaType<T>();
		return metaType->canCast(toMetaType);
	}

	template <typename T>
	T cast() const {
		const MetaType * toMetaType = metapp::getMetaType<T>();
		assert(metaType->canCast(toMetaType));
		T result = T();
		metaType->cast(data, toMetaType, &result);
		return result;
	}

	TypeKind getTypeKind() const {
		return metaType->getTypeKind();
	}

	const MetaType * getMetaType() const {
		return metaType;
	}

private:
	const MetaType * metaType;
	MetaTypeData data;
};


} // namespace metapp

#endif