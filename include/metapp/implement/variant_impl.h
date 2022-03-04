#ifndef METAPP_VARIANT_IMPL_H_969872685611
#define METAPP_VARIANT_IMPL_H_969872685611

#include "metapp/interfaces/metastreaming.h"

namespace metapp {

template <typename T>
inline Variant Variant::create(T value, const bool copyVariant,
	typename std::enable_if<
	std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
	>::type *)
{
	if(copyVariant) {
		return value;
	}
	return Variant(metapp::getMetaType<T>(), &value);
}

template <typename T>
inline Variant Variant::create(T value, const bool /*copyVariant*/,
	typename std::enable_if<
	! std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
	>::type *)
{
	return Variant(metapp::getMetaType<T>(), &value);
}

inline Variant Variant::takeFrom(const MetaType * metaType, void * object)
{
	Variant result;

	result.metaType = metaType;
	result.data.constructObject(std::shared_ptr<void>(object, [metaType](void * p) {
		metaType->destroy(p);
	}));

	return result;
}

inline Variant Variant::takeFrom(const Variant & object)
{
	const MetaType * metaType = object.getMetaType();
	if(metaType->getTypeKind() == tkPointer) {
		metaType = metaType->getUpType();
	}
	return takeFrom(metaType, object.get<void *>());
}

inline Variant::Variant() noexcept
	: 
		metaType(metapp::getMetaType<void>()),
		data()
{
}

template <typename T>
inline Variant::Variant(T value) noexcept
	:
		metaType(metapp::getMetaType<T>())
{
	metaType->constructData(&data, &value);
}

inline Variant::Variant(const MetaType * metaType) noexcept
	:
		metaType(metaType)
{
	metaType->constructData(&data, nullptr);
}

inline Variant::Variant(const MetaType * metaType, const void * copyFrom) noexcept
	:
		metaType(metaType)
{
	metaType->constructData(&data, copyFrom);
}

inline Variant::Variant(const Variant & other) noexcept
	:
		metaType(other.metaType),
		data(other.data)
{
}

inline Variant::Variant(Variant && other) noexcept
	:
		metaType(std::move(other.metaType)),
		data(std::move(other.data))
{
}

template <typename T>
inline Variant & Variant::operator = (T value) noexcept
{
	*this = Variant::create<T>(value);
	
	return *this;
}

inline Variant & Variant::operator = (const Variant & other) noexcept
{
	metaType = other.metaType;
	data = other.data;

	return *this;
}

inline Variant & Variant::operator = (Variant && other) noexcept
{
	metaType = std::move(other.metaType);
	data = std::move(other.data);

	return *this;
}

inline Variant Variant::clone() const
{
	Variant result;
	result.metaType = metaType;
	result.metaType->constructData(&result.data, metaType->getAddress(data));
	return result;
}

template <typename T>
inline bool Variant::canGet() const
{
	return areMetaTypesMatched(metaType, metapp::getMetaType<T>(), false);
}

template <typename T>
inline auto Variant::get() const -> typename internal_::VariantReturnType<T>::Type
{
	using U = typename internal_::VariantReturnType<T>::Type;
	return (U)(*(typename std::remove_reference<U>::type *)(metaType->getAddress(data)));
}

inline void * Variant::getAddress() const
{
	return metaType->getAddress(data);
}

inline Variant Variant::dereference() const
{
	const MetaType * mt = metaType;
	void * address = nullptr;
	if(mt->getTypeKind() == tkPointer) {
		mt = mt->getUpType();
		address = get<void *>();
	}
	else if(mt->getTypeKind() == tkReference) {
		mt = mt->getUpType();
		address = getAddress();
	}
	if(address != nullptr) {
		return Variant(mt, address);
	}
	return *this;
}

inline bool Variant::canCast(const MetaType * toMetaType) const
{
	return metaType->canCast(*this, toMetaType);
}

template <typename T>
inline bool Variant::canCast() const
{
	return canCast(metapp::getMetaType<T>());
}

inline Variant Variant::cast(const MetaType * toMetaType) const
{
	assert(canCast(toMetaType));
	return metaType->cast(*this, toMetaType);
}

template <typename T>
inline Variant Variant::cast() const
{
	return cast(metapp::getMetaType<T>());
}

inline const MetaType * Variant::getMetaType() const noexcept
{
	return metaType;
}

inline std::istream & operator >> (std::istream & stream, Variant & value)
{
	auto metaStreaming = value.metaType->getMetaStreaming();
	if(metaStreaming == nullptr) {
		errorNotSupported("No << input streaming operator.");
	}
	metaStreaming->streamIn(stream, value);
	return stream;
}

inline std::ostream & operator << (std::ostream & stream, const Variant & value)
{
	auto metaStreaming = value.metaType->getMetaStreaming();
	if(metaStreaming == nullptr) {
		errorNotSupported("No >> output streaming operator.");
	}
	metaStreaming->streamOut(stream, value);
	return stream;
}

inline TypeKind getTypeKind(const Variant & v)
{
	return v.getMetaType()->getTypeKind();
}

template <typename T, typename U>
inline Variant variantCast(const Variant & value)
{
	return (U)(value.get<T>());
}


} // namespace metapp


#endif
