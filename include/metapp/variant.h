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
			metaType(metapp::getMetaType<void>()),
			data()
	{
	}

	template <typename T>
	explicit Variant(T value) noexcept
		:
			metaType(metapp::getMetaType<T>())
	{
		metaType->constructWith(data, &value);
	}

	explicit Variant(const MetaType * metaType) noexcept
		:
		metaType(metaType)
	{
		metaType->constructDefault(data);
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
	Variant & operator = (T value) noexcept
	{
		return set<T>(value);
	}

	Variant & operator = (const Variant & other) noexcept
	{
		metaType = other.metaType;
		data = other.data;

		return *this;
	}

	Variant & operator = (Variant && other) noexcept
	{
		metaType = std::move(other.metaType);
		data = std::move(other.data);

		return *this;
	}

	template <typename T>
	Variant & set(T value)
	{
		metaType = metapp::getMetaType<T>();
		metaType->constructWith(data, &value);

		return *this;
	}

	template <typename T>
	bool canGet(const bool strictMode = false) const {
		return isPossibleSame(metaType, metapp::getMetaType<T>(), strictMode);
	}

	template <typename T>
	T get() const {
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

	friend std::istream & operator >> (std::istream & stream, Variant & v) {
		v.metaType->streamIn(stream, v.data);
		return stream;
	}

	friend std::ostream & operator << (std::ostream & stream, const Variant & v) {
		v.metaType->streamOut(stream, v.data);
		return stream;
	}

private:
	const MetaType * metaType;
	MetaTypeData data;
};


} // namespace metapp

#endif
