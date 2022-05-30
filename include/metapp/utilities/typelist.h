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

#ifndef METAPP_TYPELIST_H_969872685611
#define METAPP_TYPELIST_H_969872685611

#include <type_traits>
#include <cstddef>

namespace metapp {

template <typename ...Args>
struct TypeList
{
};

template <typename TL, int N, typename T>
struct HelperTypeListIndexOf
{
};

template <int N, typename T>
struct HelperTypeListIndexOf <TypeList<>, N, T>
{
	static constexpr int value = -1;
};

template <int N, typename T, typename Arg0, typename ...Args>
struct HelperTypeListIndexOf <TypeList<Arg0, Args...>, N, T>
{
	static constexpr int value = std::is_same<T, Arg0>::value ?
		N :
		HelperTypeListIndexOf<TypeList<Args...>, N + 1, T>::value
	;
};

template <typename TL, typename T>
struct TypeListIndexOf
{
	static constexpr int value = HelperTypeListIndexOf<TL, 0, T>::value;
};

template <typename TL, typename T>
struct TypeListIn
{
	static constexpr bool value = (TypeListIndexOf<TL, T>::value >= 0);
};

template <typename TL>
struct TypeListCount
{
	static constexpr std::size_t value = 0;
};

template <typename ...Args>
struct TypeListCount <TypeList<Args...> >
{
	static constexpr std::size_t value = sizeof...(Args);
};

template <std::size_t I, std::size_t N, typename TL>
struct HelperTypeListGetAt
{
};

template <std::size_t N, typename Arg0, typename ...Args>
struct HelperTypeListGetAt <N, N, TypeList<Arg0, Args...> >
{
	using Type = Arg0;
};

template <std::size_t I, std::size_t N, typename Arg0, typename ...Args>
struct HelperTypeListGetAt<I, N, TypeList<Arg0, Args...> >
{
	using Type = typename HelperTypeListGetAt<I + 1, N, TypeList<Args...> >::Type;
};

template <typename TL, std::size_t N>
struct TypeListGetAt
{
	using Type = typename HelperTypeListGetAt<0, N, TL>::Type;
};

template <typename TL, typename T>
struct TypeListAppend;

template <typename T, typename ...Args>
struct TypeListAppend <TypeList<Args...>, T>
{
	using Type = TypeList<Args..., T>;
};

template <typename TL, typename T>
struct TypeListPreppend;

template <typename T, typename ...Args>
struct TypeListPreppend <TypeList<Args...>, T>
{
	using Type = TypeList<T, Args...>;
};

template <typename TL1, typename TL2>
struct TypeListConcat
{
};

template <typename ...A, typename ...B>
struct TypeListConcat <TypeList<A...>, TypeList<B...> >
{
	using Type = TypeList<A..., B...>;
};

template <typename Result, typename TL1, typename TL2, typename FullTL2>
struct HelperTypeListProduct;

template <typename Result, typename A0, typename ...A, typename B0, typename ...B, typename FullTL2>
struct HelperTypeListProduct <Result, TypeList<A0, A...>, TypeList<B0, B...>, FullTL2>
{
	using Item = TypeList<A0, B0>;
	using Next = typename HelperTypeListProduct<Result, TypeList<A0, A...>, TypeList<B...>, FullTL2>::Type;
	using Type = typename TypeListPreppend<Next, Item>::Type;
};

template <typename Result, typename A0, typename ...A, typename FullTL2>
struct HelperTypeListProduct <Result, TypeList<A0, A...>, TypeList<>, FullTL2>
{
	using Type = typename HelperTypeListProduct<Result, TypeList<A...>, FullTL2, FullTL2>::Type;
};

template <typename Result, typename ...B, typename FullTL2>
struct HelperTypeListProduct <Result, TypeList<>, TypeList<B...>, FullTL2>
{
	using Type = Result;
};

// TL1 * TL2, Used by unit tests
template <typename TL1, typename TL2>
struct TypeListProduct
{
	using Type = typename HelperTypeListProduct<TypeList<>, TL1, TL2, TL2>::Type;
};


} // namespace metapp

#endif
