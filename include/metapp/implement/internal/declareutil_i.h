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

#ifndef METAPP_DECLAREUTIL_I_H_969872685611
#define METAPP_DECLAREUTIL_I_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"

#include <type_traits>
#include <numeric>
#include <algorithm>

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

template <typename ArgList>
struct MetaCallableInvokeChecker
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename Args>
	static bool canInvoke(const Args & arguments, const size_t argumentCount) {
		using Sequence = typename MakeSizeSequence<argCount>::Type;
		return doCanInvoke(arguments, argumentCount, Sequence());
	}

	template <size_t ...Indexes, typename Args>
	static bool doCanInvoke(const Args & arguments, const size_t argumentCount, SizeConstantList<Indexes...>) {
		std::array<bool, argCount> canCastList {
			canCastArgument<ArgumentTypeList, Indexes>(arguments, argumentCount)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)argumentCount;
		(void)canCastList;
		return std::find(std::begin(canCastList), std::end(canCastList), false) == std::end(canCastList);
	}

	template <typename Args>
	static int rankInvoke(const Args & arguments, const size_t argumentCount) {
		using Sequence = typename MakeSizeSequence<argCount>::Type;
		return doRankInvoke(arguments, argumentCount, Sequence());
	}

	template <size_t ...Indexes, typename Args>
	static int doRankInvoke(const Args & arguments, const size_t argumentCount, SizeConstantList<Indexes...>) {
		if(argCount == argumentCount && argumentCount == 0) {
			return 1000;
		}
		std::array<int, argCount> rankList {
			rankArgumentMatching<ArgumentTypeList, Indexes>(arguments, argumentCount)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)argumentCount;
		(void)rankList;
		return std::accumulate(std::begin(rankList), std::end(rankList), 0);
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
		using Sequence = typename MakeSizeSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * /*instance*/, const Args & arguments, SizeConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant(func(getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...));
	}
};

template <typename ArgList>
struct MetaCallableInvoker <void, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT, typename Args>
	static Variant invoke(FT && func, void * instance, const Args & arguments, const size_t /*argumentCount*/) {
		using Sequence = typename MakeSizeSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * /*instance*/, const Args & arguments, SizeConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		func(getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...);
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
		using Sequence = typename MakeSizeSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * instance, const Args & arguments, SizeConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant((static_cast<Class *>(instance)->*func)(getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...));
	}
};

template <typename Class, typename ArgList>
struct MetaCallableInvoker <Class, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT, typename Args>
	static Variant invoke(FT && func, void * instance, const Args & arguments, const size_t /*argumentCount*/) {
		using Sequence = typename MakeSizeSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, typename Args, size_t ...Indexes>
	static Variant doInvoke(FT func, void * instance, const Args & arguments, SizeConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		(static_cast<Class *>(instance)->*func)(getArgument<ArgumentTypeList, Indexes>(castedArguments.data())...);
		return Variant();
	}
};


} // namespace internal_

} // namespace metapp

#endif
