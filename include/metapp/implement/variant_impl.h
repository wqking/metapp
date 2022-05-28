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

template <typename T>
inline Variant Variant::reference(T && value)
{
	return create<T &>((T &)value);
}

template <typename T>
inline Variant::Variant(T value)
	:
		metaType(metapp::getMetaType<T>())
{
	metaType->constructData(&data, &value, nullptr);
}

template <typename T>
inline Variant & Variant::operator = (T value)
{
	*this = Variant::create<T>(value);
	
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
		errorBadCast("Can't get from Variant");
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
	if(canGet<T>()) {
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


} // namespace metapp


#endif
