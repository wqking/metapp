// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef METAPP_VARIANT_IMPL_H_969872685611
#define METAPP_VARIANT_IMPL_H_969872685611

#include "metapp/interfaces/metastreaming.h"

namespace metapp {

template <typename T>
inline Variant Variant::create(T value,
	typename std::enable_if<
		std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
	>::type *)
{
	// The ugly pointer castings are to avoid compile error
	// when value is const volatile Variant &, or any type involved volatile
	return *(Variant *)(void *)&value;
}

template <typename T>
inline Variant Variant::create(T value,
	typename std::enable_if<
		! std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
	>::type *)
{
	return Variant(metapp::getMetaType<T>(), (const void *)&value);
}

inline Variant Variant::retype(const MetaType * metaType, const Variant & var)
{
	Variant result;
	result.metaType = metaType;
	result.data = var.data;
	return result;
}

inline Variant Variant::takeFrom(const MetaType * metaType, void * instance)
{
	Variant result;

	result.metaType = metaType;
	result.data.constructObject(std::shared_ptr<void>(instance, [metaType](void * p) {
		metaType->destroy(p);
	}));

	return result;
}

inline Variant Variant::takeFrom(const Variant & var)
{
	const MetaType * metaType = var.getMetaType();
	if(metaType->isPointer()) {
		metaType = metaType->getUpType();
	}
	return takeFrom(metaType, var.get<void *>());
}

inline Variant::Variant() noexcept
	: 
		metaType(metapp::getMetaType<void>()),
		data()
{
}

template <typename T>
inline Variant::Variant(T value)
	:
		metaType(metapp::getMetaType<T>())
{
	metaType->constructData(&data, &value);
}

inline Variant::Variant(const MetaType * metaType)
	:
		metaType(metaType)
{
	metaType->constructData(&data, nullptr);
}

inline Variant::Variant(const MetaType * metaType, const void * copyFrom)
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
inline Variant & Variant::operator = (T value)
{
	*this = Variant::create<T>(value);
	
	return *this;
}

inline Variant & Variant::operator = (const Variant & other) noexcept
{
	if(this != &other) {
		metaType = other.metaType;
		data = other.data;
	}

	return *this;
}

inline Variant & Variant::operator = (Variant && other) noexcept
{
	if(this != &other) {
		metaType = std::move(other.metaType);
		data = std::move(other.data);
	}

	return *this;
}

inline Variant Variant::clone() const
{
	Variant result;
	result.metaType = metaType;
	result.metaType->constructData(&result.data, getAddress());
	return result;
}

template <typename T>
inline bool Variant::canGet() const
{
	using U = typename internal_::VariantReturnType<T>::Type;
	const MetaType * toMetaType = metapp::getMetaType<U>();
	const MetaType * fromMetaType = metaType;
	if(fromMetaType->isReference() && toMetaType->isReference()) {
		return true;
	}
	fromMetaType = getNonReferenceMetaType(fromMetaType);
	toMetaType = getNonReferenceMetaType(toMetaType);
	if(fromMetaType->isPointer() && toMetaType->isPointer()) {
		return true;
	}
	if(fromMetaType->isArray() && toMetaType->isArray()) {
		return true;
	}
	return fromMetaType->getUnifiedType() == toMetaType->getUnifiedType();
}

template <typename T>
inline auto Variant::get() const -> typename internal_::VariantReturnType<T>::Type
{
	if(! canGet<T>()) {
		errorBadCast("Can't get from Variant");
	}

	using U = typename internal_::VariantReturnType<T>::Type;
	return (U)(*(typename std::remove_reference<U>::type *)(getAddress()));
}

inline void * Variant::getAddress() const
{
	return data.getAddress();
}

inline Variant Variant::toReference() const
{
	return metaType->toReference(*this);
}

inline Variant Variant::dereference() const
{
	const MetaType * mt = metaType;
	void * address = nullptr;
	if(mt->isPointer()) {
		mt = mt->getUpType();
		address = get<void *>();
	}
	else if(mt->isReference()) {
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
	Variant result = metaType->cast(*this, toMetaType);
	if(result.isEmpty()) {
		errorBadCast();
	}
	return result;
}

template <typename T>
inline Variant Variant::cast() const
{
	return cast(metapp::getMetaType<T>());
}

inline Variant Variant::castSilently(const MetaType * toMetaType) const
{
	return metaType->cast(*this, toMetaType);
}

template <typename T>
inline Variant Variant::castSilently() const
{
	return castSilently(metapp::getMetaType<T>());
}

inline bool Variant::isEmpty() const noexcept
{
	return metaType->getTypeKind() == tkVoid;
}

inline const MetaType * Variant::getMetaType() const noexcept
{
	return metaType;
}

inline void Variant::swap(Variant & other) noexcept
{
	using std::swap;

	swap(metaType, other.metaType);
	swap(data, other.data);
}

inline std::istream & operator >> (std::istream & stream, Variant & value)
{
	auto metaStreaming = value.metaType->getMetaStreaming();
	if(metaStreaming == nullptr) {
		errorUnsupported("No << input streaming operator.");
		return stream;
	}
	metaStreaming->streamIn(stream, value);
	return stream;
}

inline std::ostream & operator << (std::ostream & stream, const Variant & value)
{
	auto metaStreaming = value.metaType->getMetaStreaming();
	if(metaStreaming == nullptr) {
		errorUnsupported("No >> output streaming operator.");
		return stream;
	}
	metaStreaming->streamOut(stream, value);
	return stream;
}

inline void swap(Variant & a, Variant & b) noexcept
{
	a.swap(b);
}

inline TypeKind getTypeKind(const Variant & v)
{
	return v.getMetaType()->getTypeKind();
}

inline const Variant & getEmptyVariant()
{
	static Variant emptyVariant {};
	return emptyVariant;
}


} // namespace metapp


#endif
