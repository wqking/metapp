#ifndef METAPP_TYPEUTIL_I_H_969872685611
#define METAPP_TYPEUTIL_I_H_969872685611

#include "metapp/utils/typelist.h"

#include <type_traits>
#include <istream>
#include <ostream>

namespace metapp {

namespace internal_ {

template <typename T, T ...values>
struct ConstantList
{
};

template <size_t ...values>
using SizeConstantList = ConstantList<size_t, values...>;

template <bool ...values>
using BoolConstantList = ConstantList<bool, values...>;

template <size_t N, size_t ...Indexes>
struct MakeSizeSequence : MakeSizeSequence <N - 1, N - 1, Indexes...>
{
};

template <size_t ...Indexes>
struct MakeSizeSequence <0, Indexes...>
{
	using Type = SizeConstantList<Indexes...>;
};


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
// "std::istream &&" even if the right value doesn't apply to the << operator.
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
struct HasFunctionGetMetaEnum
{
	template <typename C> static std::true_type test(decltype(C::getMetaEnum) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaIndexable
{
	template <typename C> static std::true_type test(decltype(C::getMetaIndexable) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaIterable
{
	template <typename C> static std::true_type test(decltype(C::getMetaIterable) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaStreaming
{
	template <typename C> static std::true_type test(decltype(C::getMetaStreaming) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaMap
{
	template <typename C> static std::true_type test(decltype(C::getMetaMap) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaMember
{
	template <typename C> static std::true_type test(decltype(C::getMetaMember) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasFunctionGetMetaUser
{
	template <typename C> static std::true_type test(decltype(C::getMetaUser) *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

#define METAPP_HAS_MEMBER(member) \
	template <typename T> struct HasMember_ ## member { \
		template <typename C> static std::true_type test(decltype(C::member) *); \
		template <typename C> static std::false_type test(...); \
		enum { value = !! decltype(test<T>(0))() }; \
	}

METAPP_HAS_MEMBER(constructData);
METAPP_HAS_MEMBER(destroy);
METAPP_HAS_MEMBER(getAddress);
METAPP_HAS_MEMBER(canCast);
METAPP_HAS_MEMBER(cast);
METAPP_HAS_MEMBER(canCastFrom);
METAPP_HAS_MEMBER(castFrom);
METAPP_HAS_MEMBER(typeKind);
METAPP_HAS_MEMBER(typeFlags);

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


} // namespace internal_

} // namespace metapp

#endif
