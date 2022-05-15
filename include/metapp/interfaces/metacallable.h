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

#ifndef METAPP_METACALLABLE_H_969872685611
#define METAPP_METACALLABLE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

class Variant;

class MetaCallable
{
public:
	MetaCallable(
		const MetaType * (*getClassType)(const Variant & callable),
		int (*getParameterCount)(const Variant & callable),
		const MetaType * (*getReturnType)(const Variant & callable),
		const MetaType * (*getParameterType)(const Variant & callable, const int index),
		int (*rankInvoke)(const Variant & callable, const Variant * arguments, const int argumentCount),
		bool (*canInvoke)(const Variant & callable, const Variant * arguments, const int argumentCount),
		Variant (*invoke)(const Variant & callable, void * instance, const Variant * arguments, const int argumentCount)
	)
		:
			getClassType(getClassType),
			getParameterCount(getParameterCount),
			getReturnType(getReturnType),
			getParameterType(getParameterType),
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{
	}

	const MetaType * (*getClassType)(const Variant & callable);
	int (*getParameterCount)(const Variant & callable);
	const MetaType * (*getReturnType)(const Variant & callable);
	const MetaType * (*getParameterType)(const Variant & callable, const int index);

	int (*rankInvoke)(const Variant & callable, const Variant * arguments, const int argumentCount);
	bool (*canInvoke)(const Variant & callable, const Variant * arguments, const int argumentCount);
	Variant (*invoke)(const Variant & callable, void * instance, const Variant * arguments, const int argumentCount);

	bool isStatic(const Variant & callable) const {
		return getClassType(callable)->isVoid();
	}
};

template <typename Iterator>
Iterator findCallable(
	Iterator first,
	Iterator last,
	const Variant * arguments,
	const int argumentCount,
	int * resultMaxRank = nullptr
)
{
	Iterator result = last;

	int maxRank = 0;
	for(; first != last; ++first) {
		const Variant & callable = (const Variant &)*first;
		const int rank = callable.getMetaType()->getMetaCallable()->rankInvoke(callable, arguments, argumentCount);
		if(rank > maxRank) {
			maxRank = rank;
			result = first;
		}
	}
	
	if(resultMaxRank != nullptr) {
		*resultMaxRank = maxRank;
	}

	return result;
}

template <typename ...Args>
struct CallableInvoker;

template <typename Arg0, typename ...Args>
struct CallableInvoker <Arg0, Args...>
{
	static Variant invoke(const Variant & callable, void * instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return callable.getMetaType()->getMetaCallable()->invoke(callable, instance, arguments, sizeof...(Args) + 1);
	}

	template <typename Iterator>
	static Variant invokeCallableList(Iterator first, Iterator last, void * instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		auto it = findCallable(first, last, arguments, sizeof...(Args) + 1);
		if(it != last) {
			const Variant & callable = (const Variant &)*it;
			return callable.getMetaType()->getMetaCallable()->invoke(callable, instance, arguments, sizeof...(Args) + 1);
		}
		else {
			errorIllegalArgument();
			return Variant();
		}
	}

	static int rankInvoke(const Variant & callable, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return callable.getMetaType()->getMetaCallable()->rankInvoke(callable, arguments, sizeof...(Args) + 1);
	}

	static bool canInvoke(const Variant & callable, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return callable.getMetaType()->getMetaCallable()->canInvoke(callable, arguments, sizeof...(Args) + 1);
	}

};

template <>
struct CallableInvoker <>
{
	static Variant invoke(const Variant & callable, void * instance)
	{
		return callable.getMetaType()->getMetaCallable()->invoke(callable, instance, nullptr, 0);
	}

	template <typename Iterator>
	static Variant invokeCallableList(Iterator first, Iterator last, void * instance)
	{
		auto it = findCallable(first, last, nullptr, 0);
		if(it != last) {
			const Variant & callable = (const Variant &)*it;
			return callable.getMetaType()->getMetaCallable()->invoke(callable, instance, nullptr, 0);
		}
		else {
			errorIllegalArgument();
			return Variant();
		}
	}

	static int rankInvoke(const Variant & callable)
	{
		return callable.getMetaType()->getMetaCallable()->rankInvoke(callable, nullptr, 0);
	}

	static bool canInvoke(const Variant & callable)
	{
		return callable.getMetaType()->getMetaCallable()->canInvoke(callable, nullptr, 0);
	}

};

inline const MetaType * callableGetClassType(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getClassType(callable);
}

inline int callableGetParameterCount(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getParameterCount(callable);
}

inline const MetaType * callableGetReturnType(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getReturnType(callable);
}

inline const MetaType * callableGetParameterType(const Variant & callable, const int index)
{
	return callable.getMetaType()->getMetaCallable()->getParameterType(callable, index);
}

template <typename ...Args>
inline int callableRankInvoke(const Variant & callable, Args ...args)
{
	return CallableInvoker<Args...>::rankInvoke(callable, args...);
}

template <typename ...Args>
inline bool callableCanInvoke(const Variant & callable, Args ...args)
{
	return CallableInvoker<Args...>::canInvoke(callable, args...);
}

template <typename ...Args>
inline Variant callableInvoke(const Variant & callable, void * instance, Args ...args)
{
	return CallableInvoker<Args...>::invoke(callable, instance, args...);
}

template <template <typename, typename> class Container, typename T, typename Allocator, typename ...Args>
inline Variant callableInvoke(const Container<T, Allocator> & callableList, void * instance, Args ...args)
{
	return CallableInvoker<Args...>::invokeCallableList(callableList.begin(), callableList.end(), instance, args...);
}

inline bool callableIsStatic(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->isStatic(callable);
}


} // namespace metapp

#endif
