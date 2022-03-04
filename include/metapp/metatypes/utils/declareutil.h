#ifndef METAPP_DECLAREUTIL_H_969872685611
#define METAPP_DECLAREUTIL_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"

#include <type_traits>
#include <numeric>

namespace metapp {

namespace internal_ {

template <typename TL, size_t N, typename Args>
bool canCastArgument(const Args & arguments, const size_t argumentCount)
{
	if(N >= argumentCount) {
		return true;
	}
	return arguments[N].template canCast<typename TypeListGetAt<TL, N>::Type>();
}

template <typename TL, size_t N, typename Args>
int rankArgumentMatching(const Args & arguments, const size_t argumentCount)
{
	if(N >= argumentCount) {
		return 1000;
	}
	using To = typename TypeListGetAt<TL, N>::Type;
	if(arguments[N].template canGet<To>()) {
		return 1000;
	}
	if(arguments[N].template canCast<To>()) {
		return 1;
	}
	return 0;
}

template <typename TL, size_t N, typename Args>
Variant castArgument(const Args & arguments)
{
	return arguments[N].template cast<typename TypeListGetAt<TL, N>::Type>();
}

template <typename TL, size_t N, typename Args>
auto getArgument(const Args & arguments) -> typename TypeListGetAt<TL, N>::Type
{
	return arguments[N].template get<typename TypeListGetAt<TL, N>::Type>();
}

} // namespace internal_

template <typename T>
struct AccessibleIsAssignable
{
	static constexpr bool value = true;
};

template <typename T>
struct AccessibleIsAssignable <T *>
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(*std::declval<C>() = *std::declval<C>())>::type *);
	template <typename C> static std::false_type test(...);

	//static constexpr bool value = !! decltype(test<T *>(0))();
	enum { value = !! decltype(test<T *>(0))() };
};

template <typename T>
struct AccessibleIsAssignable <T &>
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<C>() = std::declval<C>())>::type *);
	template <typename C> static std::false_type test(...);

	//static constexpr bool value = !! decltype(test<T *>(0))();
	enum { value = !! decltype(test<T &>(0))() };
};

template <typename Class, typename T>
struct AccessibleIsAssignable <T Class::*>
{
	template <typename C, typename U> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<C *>()->*std::declval<U C::*>() = std::declval<U>())>::type *);
	template <typename C, typename U> static std::false_type test(...);

	//static constexpr bool value = !! decltype(test<T *>(0))();
	enum { value = !! decltype(test<Class, T>(0))() };
};

template <typename ArgList>
struct MetaCallableInvokeChecker
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename Args>
	static bool canInvoke(const Args & arguments, const size_t argumentCount) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doCanInvoke(arguments, argumentCount, IS());
	}

	template <size_t ...Indexes, typename Args>
	static bool doCanInvoke(const Args & arguments, const size_t argumentCount, internal_::IndexSequence<Indexes...>) {
		std::array<bool, argCount> canCastList {
			internal_::canCastArgument<ArgumentTypeList, Indexes>(arguments, argumentCount)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)argumentCount;
		(void)canCastList;
		return std::find(std::begin(canCastList), std::end(canCastList), false) == std::end(canCastList);
	}

	template <typename Args>
	static int rankInvoke(const Args & arguments, const size_t argumentCount) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doRankInvoke(arguments, argumentCount, IS());
	}

	template <size_t ...Indexes, typename Args>
	static int doRankInvoke(const Args & arguments, const size_t argumentCount, internal_::IndexSequence<Indexes...>) {
		std::array<int, argCount> canCastList {
			internal_::rankArgumentMatching<ArgumentTypeList, Indexes>(arguments, argumentCount)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)argumentCount;
		(void)canCastList;
		return std::accumulate(std::begin(canCastList), std::end(canCastList), 0);
	}
};

template <typename Class, typename RT, typename ArgList>
struct MetaCallableInvoker;

template <typename RT, typename ArgList>
struct MetaCallableInvoker <void, RT, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT, typename Args>
	static Variant invoke(FT && func, void * instance, const Args & arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, IS());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * /*instance*/, const Args & arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant(func(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...));
	}
};

template <typename ArgList>
struct MetaCallableInvoker <void, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT, typename Args>
	static Variant invoke(FT && func, void * instance, const Args & arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, IS());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * /*instance*/, const Args & arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		func(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...);
		return Variant();
	}
};

template <typename Class, typename RT, typename ArgList>
struct MetaCallableInvoker
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT, typename Args>
	static Variant invoke(FT && func, void * instance, const Args & arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, IS());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * instance, const Args & arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant((static_cast<Class *>(instance)->*func)(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...));
	}
};

template <typename Class, typename ArgList>
struct MetaCallableInvoker <Class, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT, typename Args>
	static Variant invoke(FT && func, void * instance, const Args & arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, IS());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * instance, const Args & arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		(static_cast<Class *>(instance)->*func)(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...);
		return Variant();
	}
};


} // namespace metapp

#endif
