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
	DefaultArgsFunction() : func(), defaultArgs() {
	}

	DefaultArgsFunction(const Variant & func, std::vector<Variant> defaultArgs)
		: func(func), defaultArgs(std::move(defaultArgs)) {
	}

	const Variant & getFunc() const {
		return func;
	}

	const Variant * getDefaultArgs() const {
		return defaultArgs.data();
	}

	int getDefaultArgsCount() const {
		return static_cast<int>(defaultArgs.size());
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

template <typename FT>
struct DeclareMetaTypeBase <DefaultArgsFunction<FT> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = DefaultArgsFunction<FT>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr int argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	using UpType = typename Underlying::UpType;
	static constexpr TypeKind typeKind = tkDefaultArgsFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetClassType,
			&metaCallableGetParameterCountInfo,
			&metaCallableGetReturnType,
			&metaCallableGetParameterType,
			&metaCallableRankInvoke,
			&metaCallableCanInvoke,
			&metaCallableInvoke
		);
		return &metaCallable;
	}

	static bool isValidArgumentCount(const Variant & func, const std::size_t argumentCount)
	{
		return ((int)argumentCount >= argsCount - func.get<FunctionType &>().getDefaultArgsCount())
			&& ((int)argumentCount <= argsCount)
		;
	}

	static const MetaType * metaCallableGetClassType(const Variant & func)
	{
		const FunctionType & defaultArgsFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = defaultArgsFunc.getFunc();
		return underlyingFunc.getMetaType()->getMetaCallable()->getClassType(underlyingFunc);
	}

	static MetaCallable::ParameterCountInfo metaCallableGetParameterCountInfo(const Variant & func)
	{
		return MetaCallable::ParameterCountInfo {
			MetaCallable::detectResultCount<ReturnType>(),
			argsCount - func.get<FunctionType &>().getDefaultArgsCount(),
			argsCount
		};
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*func*/)
	{
		return getMetaType<ReturnType>();
	}

	static const MetaType * metaCallableGetParameterType(const Variant & /*func*/, const int index)
	{
		return internal_::getMetaTypeAt(index, ArgumentTypeList());
	}

	static int metaCallableRankInvoke(const Variant & func, const Variant & /*instance*/, const ArgumentSpan & arguments)
	{
		if(! isValidArgumentCount(func, arguments.size())) {
			return invokeRankNone;
		}

		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments);
	}

	static bool metaCallableCanInvoke(const Variant & func, const Variant & /*instance*/, const ArgumentSpan & arguments)
	{
		if(! isValidArgumentCount(func, arguments.size())) {
			return false;
		}

		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments);
	}

	static Variant metaCallableInvoke(const Variant & func, const Variant & instance, const ArgumentSpan & arguments)
	{
		if(! isValidArgumentCount(func, arguments.size())) {
			raiseException<IllegalArgumentException>();
			return Variant();
		}

		const FunctionType & defaultArgsFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = defaultArgsFunc.getFunc();
		if(arguments.size() == argsCount) {
			return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, arguments);
		}
		else {
			std::array<Variant, argsCount> newArguments;
			int i = 0;
			for(; i < (int)arguments.size(); ++i) {
				newArguments[i] = arguments[i];
			}
			while(i < argsCount) {
				newArguments[i] = defaultArgsFunc.getDefaultArgs()[argsCount - i - 1];
				++i;
			}
			return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, newArguments);
		}
	}

};


} // namespace metapp


#endif

