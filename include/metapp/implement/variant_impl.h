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

namespace metapp {

template <typename T>
inline Variant Variant::create(const typename std::remove_reference<T>::type & value,
	typename std::enable_if<internal_::IsVariant<T>::value>::type *)
{
	return *(Variant *)(void *)&value;
}

template <typename T>
inline Variant Variant::create(const typename std::remove_reference<T>::type & value,
	typename std::enable_if<! internal_::IsVariant<T>::value>::type *)
{
	return Variant(
		metapp::getMetaType<T>(),
		(const void *)&value,
		CopyStrategy::autoDetect
	);
}

template <typename T>
inline Variant Variant::create(typename std::remove_reference<T>::type && value,
	typename std::enable_if<internal_::IsVariant<T>::value>::type *)
{
	return *(Variant *)(void *)&value;
}

template <typename T>
inline Variant Variant::create(typename std::remove_reference<T>::type && value,
	typename std::enable_if<! internal_::IsVariant<T>::value>::type *)
{
	return Variant(
		metapp::getMetaType<T>(),
		(const void *)&value,
		std::is_rvalue_reference<T &&>::value ? CopyStrategy::move : CopyStrategy::copy
	);
}

template <typename T>
inline Variant Variant::reference(T && value)
{
	return create<T &>((T &)value);
}

inline Variant::Variant() noexcept
	: 
	metaType(metapp::getMetaType<void>()),
	data()
{
}

inline Variant::Variant(const MetaType * metaType, const void * copyFrom)
	:
	metaType(metaType),
	data()
{
	metaType->constructData(&data, copyFrom, nullptr, CopyStrategy::autoDetect);
}

inline Variant::Variant(const MetaType * metaType, const void * copyFrom, const CopyStrategy copyStrategy)
	:
	metaType(metaType),
	data()
{
	metaType->constructData(&data, copyFrom, nullptr, copyStrategy);
}

template <typename T>
inline Variant::Variant(T && value)
	:
		metaType(),
		data()
{
	doConstruct(std::forward<T>(value));
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

template <typename T>
inline Variant & Variant::operator = (T && value)
{
	doConstruct(std::forward<T>(value));

	return *this;
}

template <typename T>
inline bool Variant::canGet() const
{
	using U = typename internal_::VariantReturnType<T>::Type;
	return canGet(metapp::getMetaType<U>());
}

template <typename T>
inline auto Variant::get(
		typename std::enable_if<! std::is_same<
			typename std::remove_cv<typename std::remove_reference<T>::type>::type, Variant
		>::value>::type *
	) const -> typename internal_::VariantReturnType<T>::Type
{
	if(! canGet<T>()) {
		raiseException<BadCastException>("Can't get from Variant");
	}

	using U = typename internal_::VariantReturnType<T>::Type;
	return (U)(*(typename std::remove_reference<U>::type *)(getAddress()));
}

template <typename T>
inline auto Variant::get(
		typename std::enable_if<std::is_same<
			typename std::remove_cv<typename std::remove_reference<T>::type>::type, Variant
		>::value>::type *
	) const -> typename internal_::VariantReturnType<T>::Type
{
	if(canGet<Variant>()) {
		using U = typename internal_::VariantReturnType<T>::Type;
		return (U)(*(typename std::remove_reference<U>::type *)(getAddress()));
	}

	return (Variant &)*this;
}

template <typename T>
inline bool Variant::canCast() const
{
	return canCast(metapp::getMetaType<T>());
}

template <typename T>
inline Variant Variant::cast() const
{
	return cast(metapp::getMetaType<T>());
}

template <typename T>
inline Variant Variant::castSilently() const
{
	return castSilently(metapp::getMetaType<T>());
}

inline bool Variant::isEmpty() const noexcept
{
	return metaType->isVoid();
}

inline void Variant::swap(Variant & other) noexcept
{
	using std::swap;

	swap(metaType, other.metaType);
	swap(data, other.data);
}

template <typename T>
inline void Variant::doConstruct(T && value,
	typename std::enable_if<internal_::IsVariant<T>::value>::type *)
{
	metaType = value.metaType;
	data = value.data;
}

template <typename T>
inline void Variant::doConstruct(T && value,
	typename std::enable_if<! internal_::IsVariant<T>::value>::type *)
{
	metaType = metapp::getMetaType<typename std::remove_reference<T>::type>();
	metaType->constructData(
		&data,
		(const void *)&value,
		nullptr,
		std::is_rvalue_reference<T &&>::value ? CopyStrategy::move : CopyStrategy::copy
	);
}

inline void swap(Variant & a, Variant & b) noexcept
{
	a.swap(b);
}

inline TypeKind getTypeKind(const Variant & v)
{
	return v.getMetaType()->getTypeKind();
}


} // namespace metapp


#endif
