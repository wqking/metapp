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

#ifndef METAPP_DEFAULT_ARGS_FUNCTION_H_969872685611
#define METAPP_DEFAULT_ARGS_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/metatypes/utils/declareutil.h"
#include "metapp/utils/utility.h"

#include <vector>
#include <initializer_list>

namespace metapp {

template <typename FT, size_t defaultArgsCount>
class DefaultArgsFunction
{
public:
	DefaultArgsFunction() : func() {
	}

	DefaultArgsFunction(FT func, const std::initializer_list<Variant> & defaultArgs_)
		: func(func), defaultArgs(defaultArgs_.begin(), defaultArgs_.end()) {
	}

	const Variant & getFunc() const {
		return func;
	}

	const Variant * getDefaultArgs() const {
		return defaultArgs.data();
	}

private:
	Variant func;
	// If we use std::array here, initial it from the std::initializer_list
	// will cause the first Variant wrongly captures the std::initializer_list
	// as tkObject
	std::vector<Variant> defaultArgs;
};

template <size_t defaultArgsCount, typename FT>
auto createDefaultArgsFunction(FT && func, const std::initializer_list<Variant> & defaultArgs_)
	-> DefaultArgsFunction<FT, defaultArgsCount>
{
	return DefaultArgsFunction<FT, defaultArgsCount>(std::forward<FT>(func), defaultArgs_);
}

namespace internal_ {

struct DefaultArgsGetter
{
	size_t argumentCount;
	const Variant * passedArguments;
	size_t passedArgumentCount;
	const Variant * defaultArguments;

	const Variant & operator[] (const size_t index) const {
		if(index < passedArgumentCount) {
			return passedArguments[index];
		}
		else {
			return defaultArguments[(argumentCount - index) - 1];
		}
	}
};

} // namespace internal_

template <typename FT, size_t defaultArgsCount>
struct DeclareMetaTypeBase <DefaultArgsFunction<FT, defaultArgsCount> >
	: public CommonDeclareMetaType<DefaultArgsFunction<FT, defaultArgsCount> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = DefaultArgsFunction<FT, defaultArgsCount>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	using UpType = FT;
	static constexpr TypeKind typeKind = tkDefaultArgsFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetParamCount,
			&metaCallableGetReturnType,
			&metaCallableGetParamType,
			&metaCallableRankInvoke,
			&metaCallableCanInvoke,
			&metaCallableInvoke
		);
		return &metaCallable;
	}

	static bool isValidArgumentCount(const size_t argumentCount)
	{
		return argumentCount >= argsCount - defaultArgsCount && argumentCount <= argsCount;
	}

	static size_t metaCallableGetParamCount()
	{
		return argsCount;
	}

	static const MetaType * metaCallableGetReturnType()
	{
		return getMetaType<ReturnType>();
	}

	static const MetaType * metaCallableGetParamType(const size_t index)
	{
		return getMetaTypeAt(index, ArgumentTypeList());
	}

	static int metaCallableRankInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(argumentCount)) {
			return 0;
		}

		return MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool metaCallableCanInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(argumentCount)) {
			return false;
		}

		return MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant metaCallableInvoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(argumentCount)) {
			errorIllegalArgument();
			return Variant();
		}

		const FunctionType & defaultArgsFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = defaultArgsFunc.getFunc();
		if(argumentCount == argsCount) {
			return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, arguments, argumentCount);
		}
		else {
			std::array<Variant, argsCount> newArguments;
			size_t i = 0;
			for(; i < argumentCount; ++i) {
				newArguments[i] = arguments[i];
			}
			while(i < argsCount) {
				newArguments[i] = defaultArgsFunc.getDefaultArgs()[argsCount - i - 1];
				++i;
			}
			return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, newArguments.data(), argsCount);
		}
	}

};


} // namespace metapp


#endif

