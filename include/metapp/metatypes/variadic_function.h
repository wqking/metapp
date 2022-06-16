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

#ifndef METAPP_VARIADIC_FUNCTION_H_969872685611
#define METAPP_VARIADIC_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/utilities/utility.h"
#include "metapp/implement/internal/declareutil_i.h"

#include <array>
#include <limits>

namespace metapp {

template <typename FT>
class VariadicFunction
{
public:
	VariadicFunction() : func() {
	}

	explicit VariadicFunction(const Variant & func)
		: func(func) {
	}

	const Variant & getFunc() const {
		return func;
	}

private:
	Variant func;
};

template <typename FT>
auto createVariadicFunction(FT && func) -> VariadicFunction<FT>
{
	return VariadicFunction<FT>(std::forward<FT>(func));
}

template <typename FT>
struct DeclareMetaType <VariadicFunction<FT> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = VariadicFunction<FT>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr int argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	static constexpr TypeKind typeKind = tkVariadicFunction;

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

	static const MetaType * metaCallableGetClassType(const Variant & func)
	{
		const FunctionType & variadicFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = variadicFunc.getFunc();
		return underlyingFunc.getMetaType()->getMetaCallable()->getClassType(underlyingFunc);
	}

	static MetaCallable::ParameterCountInfo metaCallableGetParameterCountInfo(const Variant & /*func*/)
	{
		return MetaCallable::ParameterCountInfo {
			MetaCallable::detectResultCount<ReturnType>(),
			0,
			std::numeric_limits<int>::max()
		};
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*func*/)
	{
		return getMetaType<ReturnType>();
	}

	static const MetaType * metaCallableGetParameterType(const Variant & /*func*/, const int /*index*/)
	{
		return voidMetaType;
	}

	static int metaCallableRankInvoke(const Variant & /*func*/, const Variant & /*instance*/, const ArgumentSpan & /*arguments*/)
	{
		return invokeRankCast;
	}

	static bool metaCallableCanInvoke(const Variant & /*func*/, const Variant & /*instance*/, const ArgumentSpan & /*arguments*/)
	{
		return true;
	}

	static Variant metaCallableInvoke(const Variant & func, const Variant & instance, const ArgumentSpan & arguments)
	{
		std::array<Variant, 1> newArguments { Variant::reference(arguments) };
		const FunctionType & variadicFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = variadicFunc.getFunc();
		return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, newArguments);
	}

};


} // namespace metapp


#endif

