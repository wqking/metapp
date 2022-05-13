// accessorpp library
// Copyright (C) 2022 Wang Qi (wqking)
// Github: https://github.com/wqking/accessorpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ACCESSORPP_TYPEUTIL_I_H_582750282985
#define ACCESSORPP_TYPEUTIL_I_H_582750282985

#include "metapp/thirdparty/accessorpp/compiler.h"

#include <utility>
#include <type_traits>

namespace accessorpp {

namespace private_ {

template <typename F, typename ...Args>
struct CanInvoke
{
	template <typename U, typename ...X>
	static auto invoke(int) -> decltype(std::declval<U>()(std::declval<X>()...), std::true_type());

	template <typename U, typename ...X>
	static auto invoke(...)->std::false_type;

	enum {
		value = !! decltype(invoke<F, Args...>(0))()
	};
};

template <typename T>
struct HelperCallableTypeChecker
{
	static constexpr bool isFunction = false;
	static constexpr bool isClassMember = false;
	using ClassType = void;
	using ResultType = void;
};

template <typename RT, typename ...Args>
struct HelperCallableTypeChecker <RT (*)(Args...)>
{
	static constexpr bool isFunction = true;
	static constexpr bool isClassMember = false;
	using ClassType = void;
	using ResultType = RT;
};

template <typename RT, typename ...Args>
struct HelperCallableTypeChecker <RT (Args...)> : HelperCallableTypeChecker <RT (*)(Args...)>
{
};

template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...)>
{
	static constexpr bool isFunction = true;
	static constexpr bool isClassMember = true;
	using ClassType = C;
	using ResultType = RT;
};

template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...) const> : HelperCallableTypeChecker <RT (C::*)(Args...)> {};
template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...) volatile> : HelperCallableTypeChecker <RT (C::*)(Args...)> {};
template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...) const volatile> : HelperCallableTypeChecker <RT (C::*)(Args...)> {};

#ifdef ACCESSORPP_SUPPORT_STANDARD_17
template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...) const noexcept> : HelperCallableTypeChecker <RT (C::*)(Args...)> {};
template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...) volatile noexcept> : HelperCallableTypeChecker <RT (C::*)(Args...)> {};
template <typename RT, typename C, typename ...Args>
struct HelperCallableTypeChecker <RT (C::*)(Args...) const volatile noexcept> : HelperCallableTypeChecker <RT (C::*)(Args...)> {};
#endif

template <typename T>
struct CallableTypeChecker : HelperCallableTypeChecker<T>
{
};

template <typename T>
struct DetectValueType;

template <typename T>
struct DetectValueType <T *>
{
	using Type = T;
};
template <typename T> struct DetectValueType <T * const> : DetectValueType <T *> {};
template <typename T> struct DetectValueType <T * volatile> : DetectValueType <T *> {};
template <typename T> struct DetectValueType <T * const volatile> : DetectValueType <T *> {};

template <typename T, typename ...Args>
struct DetectValueType <T (Args...)>
{
	using Type = T;
};

template <typename T, typename ...Args>
struct DetectValueType <T (*)(Args...)>
{
	using Type = T;
};

template <typename T, typename C>
struct DetectValueType <T C::*>
{
	using Type = T;
};
template <typename T, typename C> struct DetectValueType <T C::* const> : DetectValueType <T C::*> {};
template <typename T, typename C> struct DetectValueType <T C::* volatile> : DetectValueType <T C::*> {};
template <typename T, typename C> struct DetectValueType <T C::* const volatile> : DetectValueType <T C::*> {};

template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...)>
{
	using Type = T;
};
template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...) const> : DetectValueType <T (C::*)(Args...)> {};
template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...) volatile> : DetectValueType <T (C::*)(Args...)> {};
template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...) const volatile> : DetectValueType <T (C::*)(Args...)> {};

#ifdef ACCESSORPP_SUPPORT_STANDARD_17
template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...) const noexcept> : DetectValueType <T (C::*)(Args...)> {};
template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...) volatile noexcept> : DetectValueType <T (C::*)(Args...)> {};
template <typename T, typename C, typename ...Args>
struct DetectValueType <T (C::*)(Args...) const volatile noexcept> : DetectValueType <T (C::*)(Args...)> {};
#endif

template <typename T>
struct GetUnderlyingType
{
	using Type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

template <typename T>
struct HasTypeOnChangingCallback
{
	template <typename C> static std::true_type test(typename C::OnChangingCallback *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool> struct SelectOnChangingCallback { using Type = typename T::OnChangingCallback; };
template <typename T> struct SelectOnChangingCallback <T, false> { using Type = void; };

template <typename T>
struct HasTypeOnChangedCallback
{
	template <typename C> static std::true_type test(typename C::OnChangedCallback *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool> struct SelectOnChangedCallback { using Type = typename T::OnChangedCallback; };
template <typename T> struct SelectOnChangedCallback <T, false> { using Type = void; };

template <typename T>
struct HasTypeCallbackData
{
	template <typename C> static std::true_type test(typename C::CallbackData *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool> struct SelectCallbackData { using Type = typename T::CallbackData; };
template <typename T> struct SelectCallbackData <T, false> { using Type = void; };

template <typename T>
struct HasTypeStorage
{
	template <typename C> static std::true_type test(typename C::Storage *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool, typename Default> struct SelectStorage { using Type = typename T::Storage; };
template <typename T, typename Default> struct SelectStorage <T, false, Default> { using Type = Default; };

template <typename T>
struct HasTypeClassTypeSetter
{
	template <typename C> static std::true_type test(typename C::ClassTypeSetter *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool, typename Default> struct SelectClassTypeSetter { using Type = typename T::ClassTypeSetter; };
template <typename T, typename Default> struct SelectClassTypeSetter <T, false, Default> { using Type = Default; };


} // namespace private_

} // namespace accessorpp

#endif
