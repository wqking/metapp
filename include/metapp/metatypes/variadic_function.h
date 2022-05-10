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

	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	static constexpr TypeKind typeKind = tkVariadicFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetClassType,
			&metaCallableGetParameterCount,
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

	static size_t metaCallableGetParameterCount(const Variant & /*func*/)
	{
		return 0;
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*func*/)
	{
		return getMetaType<ReturnType>();
	}

	static const MetaType * metaCallableGetParameterType(const Variant & /*func*/, const size_t /*index*/)
	{
		return voidMetaType;
	}

	static unsigned int metaCallableRankInvoke(const Variant & /*func*/, const Variant * /*arguments*/, const size_t /*argumentCount*/)
	{
		return invokeRankCast;
	}

	static bool metaCallableCanInvoke(const Variant & /*func*/, const Variant * /*arguments*/, const size_t /*argumentCount*/)
	{
		return true;
	}

	static Variant metaCallableInvoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		Variant newArguments[2] = { arguments, argumentCount };

		const FunctionType & variadicFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = variadicFunc.getFunc();
		return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, newArguments, 2);
	}

};


} // namespace metapp


#endif

