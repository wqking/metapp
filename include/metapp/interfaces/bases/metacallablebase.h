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

#ifndef METAPP_METACALLABLEBASE_H_969872685611
#define METAPP_METACALLABLEBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/implement/internal/declareutil_i.h"
#include "metapp/utilities/utility.h"

namespace metapp {

template <typename FT, typename Class, typename RT, typename ...Args>
struct MetaCallableBase
{
public:
	using FunctionType = FT;
	using ClassType = Class;
	using ReturnType = RT;
	using ArgumentTypeList = TypeList<Args...>;
	static constexpr std::size_t argsCount = TypeListCount<ArgumentTypeList>::value;

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

	static const MetaType * metaCallableGetClassType(const Variant & /*callable*/)
	{
		return getMetaType<ClassType>();
	}

	static MetaCallable::ParameterCountInfo metaCallableGetParameterCountInfo(const Variant & /*callable*/)
	{
		return MetaCallable::ParameterCountInfo { MetaCallable::detectResultCount<RT>(), argsCount };
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*callable*/)
	{
		return getMetaType<RT>();
	}

	static const MetaType * metaCallableGetParameterType(const Variant & /*callable*/, const int index)
	{
		return internal_::getMetaTypeAt<Args...>(index);
	}

	static int metaCallableRankInvoke(const Variant & /*callable*/, const Variant & /*instance*/, const ArgumentSpan & arguments)
	{
		if(arguments.size() != argsCount) {
			return 0;
		}
		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments);
	}

	static bool metaCallableCanInvoke(const Variant & /*callable*/, const Variant & /*instance*/, const ArgumentSpan & arguments)
	{
		if(arguments.size() != argsCount) {
			return false;
		}
		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments);
	}

	static Variant metaCallableInvoke(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments)
	{
		if(arguments.size() != argsCount) {
			raiseException<IllegalArgumentException>();
			return Variant();
		}

		FunctionType & f = callable.get<FunctionType &>();
		return internal_::MetaCallableInvoker<Class, RT, ArgumentTypeList>::invoke(f, getPointer(instance), arguments);
	}

};


} // namespace metapp


#endif

