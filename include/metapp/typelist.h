#ifndef TYPELIST_H_969872685611
#define TYPELIST_H_969872685611

#include <type_traits>

namespace metapp {

template <typename ...Args>
struct TypeList
{
};

template <int N, typename T, typename TL>
struct HelperTypeListIndexOf
{
};

template <int N, typename T>
struct HelperTypeListIndexOf <N, T, TypeList<> >
{
	static constexpr int value = -1;
};

template <int N, typename T, typename Arg0, typename ...Args>
struct HelperTypeListIndexOf <N, T, TypeList<Arg0, Args...> >
{
	static constexpr int value = std::is_same<T, Arg0>::value ?
		N :
		HelperTypeListIndexOf<N + 1, T, TypeList<Args...> >::value
	;
};

template <typename T, typename TL>
struct TypeListIndexOf
{
	static constexpr int value = HelperTypeListIndexOf<0, T, TL>::value;
};

template <typename T, typename TL>
struct TypeListIn
{
	static constexpr bool value = (TypeListIndexOf<T, TL>::value >= 0);
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


} // namespace metapp

#endif
