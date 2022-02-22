#ifndef UTILITY_H_969872685611
#define UTILITY_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"

#include <type_traits>
#include <numeric>

namespace metapp {

namespace internal_ {

template <size_t ...Indexes>
struct IndexSequence
{
};

template <size_t N, size_t ...Indexes>
struct MakeIndexSequence : MakeIndexSequence <N - 1, N - 1, Indexes...>
{
};

template <std::size_t ...Indexes>
struct MakeIndexSequence<0, Indexes...>
{
	using Type = IndexSequence<Indexes...>;
};

template <typename TL, size_t N>
bool canCastArgument(const Variant * arguments)
{
	return arguments[N].canCast<typename TypeListGetAt<TL, N>::Type>();
}

template <typename TL, size_t N>
int rankArgumentMatching(const Variant * arguments)
{
	using To = typename TypeListGetAt<TL, N>::Type;
	if(arguments[N].canGet<To>()) {
		return 1000;
	}
	if(arguments[N].canCast<To>()) {
		return 1;
	}
	return 0;
}

template <typename TL, size_t N>
Variant castArgument(const Variant * arguments)
{
	return arguments[N].cast<typename TypeListGetAt<TL, N>::Type>();
}

template <typename TL, size_t N>
auto getArgument(const Variant * arguments) -> typename TypeListGetAt<TL, N>::Type
{
	return arguments[N].get<typename TypeListGetAt<TL, N>::Type>();
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
struct MetaFunctionInvokeChecker
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	static bool canInvoke(const Variant * arguments, const size_t argumentCount) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doCanInvoke(arguments, argumentCount, IS());
	}

	template <size_t ...Indexes>
	static bool doCanInvoke(const Variant * arguments, const size_t argumentCount, internal_::IndexSequence<Indexes...>) {
		if(argumentCount != argCount) {
			return false;
		}
		std::array<bool, argCount> canCastList {
			internal_::canCastArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)canCastList;
		return std::find(std::begin(canCastList), std::end(canCastList), false) == std::end(canCastList);
	}

	static int rankInvoke(const Variant * arguments, const size_t argumentCount) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doRankInvoke(arguments, argumentCount, IS());
	}

	template <size_t ...Indexes>
	static int doRankInvoke(const Variant * arguments, const size_t argumentCount, internal_::IndexSequence<Indexes...>) {
		if(argumentCount != argCount) {
			return 0;
		}
		std::array<int, argCount> canCastList {
			internal_::rankArgumentMatching<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)canCastList;
		return std::accumulate(std::begin(canCastList), std::end(canCastList), 0);
	}
};

template <typename Class, typename RT, typename ArgList>
struct MetaFunctionInvoker;

template <typename RT, typename ArgList>
struct MetaFunctionInvoker <void, RT, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * /*instance*/, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
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
struct MetaFunctionInvoker <void, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * /*instance*/, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
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
struct MetaFunctionInvoker
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * instance, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
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
struct MetaFunctionInvoker <Class, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<argCount>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * instance, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
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
