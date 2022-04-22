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
	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

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

	static size_t metaCallableGetParamCount(const Variant & /*var*/)
	{
		return argsCount;
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*var*/)
	{
		return getMetaType<RT>();
	}

	static const MetaType * metaCallableGetParamType(const Variant & /*var*/, const size_t index)
	{
		return internal_::getMetaTypeAt<Args...>(index);
	}

	static int metaCallableRankInvoke(const Variant & /*var*/, const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return 0;
		}
		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool metaCallableCanInvoke(const Variant & /*var*/, const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return false;
		}
		return internal_::MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant metaCallableInvoke(const Variant & var, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			errorIllegalArgument();
			return Variant();
		}

		FunctionType f = var.get<FunctionType &>();
		return internal_::MetaCallableInvoker<Class, RT, ArgumentTypeList>::invoke(f, instance, arguments, argumentCount);
	}

};


} // namespace metapp


#endif

