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
#include "metapp/implement/internal/declareutil_i.h"
#include "metapp/utilities/utility.h"
// requires these headers to get UpType
#include "metapp/metatypes/function.h"
#include "metapp/metatypes/member_function.h"
#include "metapp/metatypes/constructor.h"
#include "metapp/metatypes/std_function.h"

#include <vector>
#include <initializer_list>

namespace metapp {

template <typename FT>
class DefaultArgsFunction
{
public:
	DefaultArgsFunction() : func() {
	}

	DefaultArgsFunction(FT func, std::vector<Variant> defaultArgs)
		: func(func), defaultArgs(std::move(defaultArgs)) {
	}

	const Variant & getFunc() const {
		return func;
	}

	const Variant * getDefaultArgs() const {
		return defaultArgs.data();
	}

	size_t getDefaultArgsCount() const {
		return defaultArgs.size();
	}

private:
	Variant func;
	std::vector<Variant> defaultArgs;
};

template <typename FT>
auto createDefaultArgsFunction(FT && func, std::vector<Variant> defaultArgs)
	-> DefaultArgsFunction<FT>
{
	return DefaultArgsFunction<FT>(std::forward<FT>(func), std::move(defaultArgs));
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

template <typename FT>
struct DeclareMetaTypeBase <DefaultArgsFunction<FT> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = DefaultArgsFunction<FT>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	using UpType = typename Underlying::UpType;
	static constexpr TypeKind typeKind = tkDefaultArgsFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetParameterCount,
			&metaCallableGetReturnType,
			&metaCallableGetParameterType,
			&metaCallableRankInvoke,
			&metaCallableCanInvoke,
			&metaCallableInvoke
		);
		return &metaCallable;
	}

	static bool isValidArgumentCount(const Variant & func, const size_t argumentCount)
	{
		return (argumentCount >= argsCount - func.get<FunctionType &>().getDefaultArgsCount())
			&& (argumentCount <= argsCount)
		;
	}

	static size_t metaCallableGetParameterCount(const Variant & /*func*/)
	{
		return argsCount;
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*func*/)
	{
		return getMetaType<ReturnType>();
	}

	static const MetaType * metaCallableGetParameterType(const Variant & /*func*/, const size_t index)
	{
		return internal_::getMetaTypeAt(index, ArgumentTypeList());
	}

	static unsigned int metaCallableRankInvoke(const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(func, argumentCount)) {
			return invokeRankNone;
		}

		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool metaCallableCanInvoke(const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(func, argumentCount)) {
			return false;
		}

		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant metaCallableInvoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(func, argumentCount)) {
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

