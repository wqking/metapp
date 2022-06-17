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

#ifndef METAPP_STD_FUNCTION_H_969872685611
#define METAPP_STD_FUNCTION_H_969872685611

#include "metapp/interfaces/bases/metacallablebase.h"

#include <functional>
#include <array>

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <std::function<RT (Args...)> >
	: MetaCallableBase <std::function<RT (Args...)>, void, RT, Args...>
{
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkStdFunction;

	static bool castFrom(Variant * result, const Variant * fromVar, const MetaType * /*fromMetaType*/)
	{
		if(fromVar == nullptr) {
			return true;
		}

		const MetaCallable * metaCallable = getNonReferenceMetaType(*fromVar)->getMetaCallable();
		if(metaCallable == nullptr) {
			return false;
		}
		const auto paramInfo = metaCallable->getParameterCountInfo(*fromVar);
		constexpr int argsCount_ = sizeof...(Args);
		if(argsCount_ < paramInfo.getMinParameterCount() || argsCount_ > paramInfo.getMaxParameterCount()) {
			return false;
		}
		
		const std::array<const MetaType *, argsCount_> argsTypeList {
			getMetaType<Args>()...,
		};
		for(int i = 0; i < argsCount_; ++i) {
			const MetaType * paramType = metaCallable->getParameterType(*fromVar, i);
			if(! paramType->isVoid() && ! argsTypeList[i]->canCast(paramType)) {
				return false;
			}
		}
		if(! getMetaType<RT>()->isVoid() && ! metaCallable->getReturnType(*fromVar)->canCast(getMetaType<RT>())) {
			return false;
		}

		if(result != nullptr) {
			*result = std::function<RT (Args...)>(
				[fromVar](Args ... args) -> RT {
					return doCastFromInvoke<RT>(*fromVar, args...);
				}
			);
		}
		return true;
	}

private:
	template <typename R, typename ...A>
	static auto doCastFromInvoke(const Variant & var, A &&... args)
		-> typename std::enable_if<std::is_void<R>::value, R>::type
	{
		callableInvoke(var, nullptr, std::forward<A>(args)...);
	}

	template <typename R, typename ...A>
	static auto doCastFromInvoke(const Variant & var, A &&... args)
		-> typename std::enable_if<! std::is_void<R>::value, R>::type
	{
		return callableInvoke(var, nullptr, std::forward<A>(args)...).template cast<RT>().template get<RT &&>();
	}

};


} // namespace metapp


#endif

