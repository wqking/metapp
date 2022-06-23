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

#ifndef METAPP_TYPEUTIL_I_H_969872685611
#define METAPP_TYPEUTIL_I_H_969872685611

#include "metapp/utilities/typelist.h"

#include <type_traits>
#include <istream>
#include <ostream>

namespace metapp_top_internal_ {

// In HasInputStreamOperator_ and HasOutputStreamOperator_, we have to use std::declval<std::istream &>()
// instead of std::declval<std::istream>(), because C++11 adds a generic function that accepts
// "std::istream &&" even if the right value doesn't apply to the << operator.
// See https://stackoverflow.com/questions/37480778/declval-expression-for-sfinae-with-stdostream
template <typename T>
struct HasInputStreamOperator_
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<std::istream &>() >> std::declval<C &>())>::type *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasOutputStreamOperator_
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<std::ostream &>() << std::declval<C>())>::type *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};


} // namespace metapp_top_internal_

namespace metapp {

class Variant;

namespace internal_ {

template <typename T, T ...values>
struct ConstantList
{
};

template <std::size_t ...values>
using SizeConstantList = ConstantList<std::size_t, values...>;

template <int ...values>
using IntConstantList = ConstantList<int, values...>;

template <bool ...values>
using BoolConstantList = ConstantList<bool, values...>;

template <typename T, std::size_t N, T ...Indexes>
struct MakeSequence : MakeSequence <T, N - 1, N - 1, Indexes...>
{
};

template <typename T, T ...Indexes>
struct MakeSequence <T, 0, Indexes...>
{
	using Type = ConstantList<T, Indexes...>;
};

template <int ...Indexes>
using MakeIntSequence = MakeSequence <int, Indexes...>;

template <std::size_t ...Indexes>
using MakeSizeSequence = MakeSequence <std::size_t, Indexes...>;

template <typename T>
struct VariantReturnType
{
	using Type = typename std::conditional<
		std::is_reference<T>::value,
		T,
		typename std::conditional<
			std::is_array<T>::value,
			T &,
			typename std::decay<T>::type
		>::type
	>::type;
};

template <typename T>
using HasInputStreamOperator = metapp_top_internal_::HasInputStreamOperator_<T>;
template <typename T>
using HasOutputStreamOperator = metapp_top_internal_::HasOutputStreamOperator_<T>;

#define METAPP_HAS_MEMBER(member) \
	template <typename T> struct HasMember_ ## member { \
		template <typename C> static std::true_type test(decltype(C::member) *); \
		template <typename C> static std::false_type test(...); \
		enum { value = !! decltype(test<T>(0))() }; \
	}

METAPP_HAS_MEMBER(getMetaClass);
METAPP_HAS_MEMBER(getMetaCallable);
METAPP_HAS_MEMBER(getMetaAccessible);
METAPP_HAS_MEMBER(getMetaEnum);
METAPP_HAS_MEMBER(getMetaIndexable);
METAPP_HAS_MEMBER(getMetaIterable);
METAPP_HAS_MEMBER(getMetaStreamable);
METAPP_HAS_MEMBER(getMetaMappable);
METAPP_HAS_MEMBER(getMetaPointerWrapper);
METAPP_HAS_MEMBER(getMetaUser);

METAPP_HAS_MEMBER(constructVariantData);
METAPP_HAS_MEMBER(constructData);
METAPP_HAS_MEMBER(destroy);
METAPP_HAS_MEMBER(cast);
METAPP_HAS_MEMBER(castFrom);
METAPP_HAS_MEMBER(typeKind);
METAPP_HAS_MEMBER(typeFlags);
METAPP_HAS_MEMBER(setup);

#undef METAPP_HAS_MEMBER

template <typename T>
struct HasMember_UpType
{
	template <typename C> static std::true_type test(typename C::UpType *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename From, typename To>
struct CanStaticCast
{
	template <typename F, typename T> static std::true_type test(decltype(static_cast<T>(std::declval<F>())) *);
	template <typename F, typename T> static std::false_type test(...);

	enum { value = ! std::is_void<From>::value && ! std::is_void<To>::value && !! decltype(test<From, To>(0))() };
};

template <typename From, typename To>
struct CanDynamicCast
{
	template <typename F, typename T> static std::true_type test(decltype(dynamic_cast<T>(std::declval<F>())) *);
	template <typename F, typename T> static std::false_type test(...);

	enum { value = ! std::is_void<From>::value && ! std::is_void<To>::value && !! decltype(test<From, To>(0))() };
};

template <typename Result, typename TL, typename BoolList>
struct HelperFilterTypes
{
};

template <typename Result>
struct HelperFilterTypes <Result, TypeList<>, BoolConstantList<> >
{
	using Type = Result;
};

template <typename Result, typename Type0, typename ...Types, bool has0, bool ...hasList>
struct HelperFilterTypes <Result, TypeList<Type0, Types...>, BoolConstantList<has0, hasList...> >
{
	using Temp = typename std::conditional<
		has0,
		typename TypeListAppend<Result, Type0>::Type,
		Result
	>::type;

	using Type = typename HelperFilterTypes<
		Temp,
		TypeList<Types...>,
		BoolConstantList<hasList...>
	>::Type;
};

// Give FilterTypes<TypeList<T1, T2, ..., Tn>, BoolConstantList<bool1, bool2, ..., booln> >
// The result Type is TypeList<Tx, Ty,...>, where boolx is true, booly is true, etc.
template <typename TL, typename BoolList>
struct FilterTypes
{
	using Type = typename HelperFilterTypes<TypeList<>, TL, BoolList>::Type;
};

template <bool b>
struct TrueFalse : std::false_type
{
};

template <>
struct TrueFalse <true> : std::true_type
{
};

template <typename T>
constexpr T maxOf(T a, T b)
{
	return a > b ? a : b;
}

template <typename T>
struct IsVariant
{
	static constexpr bool value = std::is_same<
		Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value;
};


} // namespace internal_

} // namespace metapp

#endif
