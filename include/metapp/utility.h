#ifndef UTILITY_H_969872685611
#define UTILITY_H_969872685611

#include <type_traits>

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

template <typename ...Args>
struct MetaFunctionInvokeChecker
{
	using ArgumentTypeList = TypeList<Args...>;

	static bool canInvoke(const Variant * arguments, const size_t argumentCount) {
		using IS = typename internal_::MakeIndexSequence<sizeof...(Args)>::Type;
		return doCanInvoke(arguments, argumentCount, IS());
	}

	template <size_t ...Indexes>
	static bool doCanInvoke(const Variant * arguments, const size_t argumentCount, internal_::IndexSequence<Indexes...>) {
		if(argumentCount != sizeof...(Args)) {
			return false;
		}
		std::array<bool, sizeof...(Args)> canCastList {
			internal_::canCastArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)canCastList;
		return std::find(std::begin(canCastList), std::end(canCastList), false) == std::end(canCastList);
	}
};

template <typename Class, typename RT, typename ...Args>
struct MetaFunctionInvoker;

template <typename RT, typename ...Args>
struct MetaFunctionInvoker <void, RT, Args...>
{
	using ArgumentTypeList = TypeList<Args...>;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<sizeof...(Args)>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * /*instance*/, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, sizeof...(Args)> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant(func(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...));
	}
};

template <typename ...Args>
struct MetaFunctionInvoker <void, void, Args...>
{
	using ArgumentTypeList = TypeList<Args...>;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<sizeof...(Args)>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * /*instance*/, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, sizeof...(Args)> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		func(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...);
		return Variant();
	}
};

template <typename Class, typename RT, typename ...Args>
struct MetaFunctionInvoker
{
	using ArgumentTypeList = TypeList<Args...>;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<sizeof...(Args)>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * instance, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, sizeof...(Args)> castedArguments {
			internal_::castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant((static_cast<Class *>(instance)->*func)(internal_::getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...));
	}
};

template <typename Class, typename ...Args>
struct MetaFunctionInvoker <Class, void, Args...>
{
	using ArgumentTypeList = TypeList<Args...>;

	template <typename FT>
	static Variant invoke(void * instance, FT && func, const Variant * arguments, const size_t /*argumentCount*/) {
		using IS = typename internal_::MakeIndexSequence<sizeof...(Args)>::Type;
		return doInvoke(instance, std::forward<FT>(func), arguments, IS());
	}

	template <typename FT, size_t ...Indexes>
	static Variant doInvoke(void * instance, FT func, const Variant * arguments, internal_::IndexSequence<Indexes...>) {
		std::array<Variant, sizeof...(Args)> castedArguments {
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
