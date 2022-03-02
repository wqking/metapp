#ifndef TYPELIST_H_969872685611
#define TYPELIST_H_969872685611

#include <type_traits>

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
	static constexpr size_t value = 0;
};

template <typename ...Args>
struct TypeListCount <TypeList<Args...> >
{
	static constexpr size_t value = sizeof...(Args);
};

template <size_t I, size_t N, typename TL>
struct HelperTypeListGetAt
{
};

template <size_t N, typename Arg0, typename ...Args>
struct HelperTypeListGetAt <N, N, TypeList<Arg0, Args...> >
{
	using Type = Arg0;
};

template <size_t I, size_t N, typename Arg0, typename ...Args>
struct HelperTypeListGetAt<I, N, TypeList<Arg0, Args...> >
{
	using Type = typename HelperTypeListGetAt<I + 1, N, TypeList<Args...> >::Type;
};

template <typename TL, size_t N>
struct TypeListGetAt
{
	using Type = typename HelperTypeListGetAt<0, N, TL>::Type;
};

template <typename TL, typename T>
struct TypeListAppend
{
};

template <typename T, typename ...Args>
struct TypeListAppend <TypeList<Args...>, T>
{
	using Type = TypeList<Args..., T>;
};


} // namespace metapp

#endif
