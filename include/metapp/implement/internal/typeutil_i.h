#ifndef TYPEUTIL_I_H_969872685611
#define TYPEUTIL_I_H_969872685611

#include "metapp/utilities/typelist.h"

#include <type_traits>
#include <istream>
#include <ostream>

namespace metapp {

namespace internal_ {

template <size_t ...Ns>
struct MaxOfInt
{
};

template <size_t N1, size_t N2, size_t ...Ns>
struct MaxOfInt <N1, N2, Ns...>
{
	static constexpr size_t r = MaxOfInt<N2, Ns...>::value;
	static constexpr size_t value = (N1 > r ? N1 : r);
};

template <size_t N1>
struct MaxOfInt <N1>
{
	static constexpr size_t value = N1;
};

template <typename T>
struct VariantReturnType
{
	using Type = typename std::conditional<
		std::is_reference<T>::value,
		T,
		typename std::decay<T>::type
	>::type;
};

// In HasInputStreamOperator and HasOutputStreamOperator, we have to use std::declval<std::istream &>()
// instead of std::declval<std::istream>(), because C++11 adds a generic function that accepts
// std::istream && even if the right value doesn't apply to the << operator.
// See https://stackoverflow.com/questions/37480778/declval-expression-for-sfinae-with-stdostream
template <typename T>
struct HasInputStreamOperator
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<std::istream &>() >> std::declval<C &>())>::type *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasOutputStreamOperator
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<std::ostream &>() << std::declval<C>())>::type *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionAccessibleGet
{
	template <typename C> static std::true_type test(decltype(C::accessibleGet) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaClass
{
	template <typename C> static std::true_type test(decltype(C::getMetaClass) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaCallable
{
	template <typename C> static std::true_type test(decltype(C::getMetaCallable) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaAccessible
{
	template <typename C> static std::true_type test(decltype(C::getMetaAccessible) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaArray
{
	template <typename C> static std::true_type test(decltype(C::getMetaArray) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaEnum
{
	template <typename C> static std::true_type test(decltype(C::getMetaEnum) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename Result, typename TL, bool ...hasList>
struct HelperFilterTypes
{
};

template <typename Result>
struct HelperFilterTypes <Result, TypeList<> >
{
	using Type = Result;
};

template <typename Result, typename Arg0, typename ...Args, bool has0, bool ...hasList>
struct HelperFilterTypes <Result, TypeList<Arg0, Args...>, has0, hasList...>
{
	using Temp = typename std::conditional<
		has0,
		typename TypeListAppend<Result, Arg0>::Type,
		Result
	>::type;

	using Type = typename HelperFilterTypes<
		Temp,
		TypeList<Args...>,
		hasList...
	>::Type;
};

// Give FilterTypes<TypeList<T1, T2, ..., Tn>, bool1, bool2, ..., booln>
// The result Type is TypeList<Tx, Ty,...>, where boolx is true, booly is true, etc.
template <typename TL, bool ...hasList>
struct FilterTypes
{
	using Type = typename HelperFilterTypes<TypeList<>, TL, hasList...>::Type;
};


} // namespace internal_

} // namespace metapp

#endif
