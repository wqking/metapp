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
		size_t (*getParamCount)(const Variant & func),
		const MetaType * (*getReturnType)(const Variant & func),
		const MetaType * (*getParamType)(const Variant & func, const size_t index),
		int (*rankInvoke)(const Variant & func, const Variant * arguments, const size_t argumentCount),
		bool (*canInvoke)(const Variant & func, const Variant * arguments, const size_t argumentCount),
		Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	)
		:
			getParamCount(getParamCount),
			getReturnType(getReturnType),
			getParamType(getParamType),
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{
	}

	// The func parameter is not used in almost all functions except invoke, in C++ meta types.
	// But it should be useful when func contains dynamic information, such as script method, or std::variant.
	// Also adding the func parameter makes DefaultArgsFunction interface better

	size_t (*getParamCount)(const Variant & func);
	const MetaType * (*getReturnType)(const Variant & func);
	const MetaType * (*getParamType)(const Variant & func, const size_t index);

	int (*rankInvoke)(const Variant & func, const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant & func, const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount);
};

template <typename Iterator>
Iterator findCallable(
	Iterator first,
	Iterator last,
	const Variant * arguments,
	const size_t argumentCount
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
	return result;
}

template <typename ...Args>
struct CallableInvoker;

template <typename Arg0, typename ...Args>
struct CallableInvoker <Arg0, Args...>
{
	static Variant invoke(const Variant & func, void * instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return func.getMetaType()->getMetaCallable()->invoke(func, instance, arguments, sizeof...(Args) + 1);
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

	static int rankInvoke(const Variant & func, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return func.getMetaType()->getMetaCallable()->rankInvoke(func, arguments, sizeof...(Args) + 1);
	}

	static bool canInvoke(const Variant & func, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return func.getMetaType()->getMetaCallable()->canInvoke(func, arguments, sizeof...(Args) + 1);
	}

};

template <>
struct CallableInvoker <>
{
	static Variant invoke(const Variant & func, void * instance)
	{
		return func.getMetaType()->getMetaCallable()->invoke(func, instance, nullptr, 0);
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

	static int rankInvoke(const Variant & func)
	{
		return func.getMetaType()->getMetaCallable()->rankInvoke(func, nullptr, 0);
	}

	static bool canInvoke(const Variant & func)
	{
		return func.getMetaType()->getMetaCallable()->canInvoke(func, nullptr, 0);
	}

};

inline size_t callableGetParamCount(const Variant & var)
{
	return var.getMetaType()->getMetaCallable()->getParamCount(var);
}

inline const MetaType * calllableGetReturnType(const Variant & var)
{
	return var.getMetaType()->getMetaCallable()->getReturnType(var);
}

inline const MetaType * calllableGetParamType(const Variant & var, const size_t index)
{
	return var.getMetaType()->getMetaCallable()->getParamType(var, index);
}

template <typename ...Args>
inline int callableRankInvoke(const Variant & var, Args ...args)
{
	return CallableInvoker<Args...>::rankInvoke(var, args...);
}

template <typename ...Args>
inline bool callableCanInvoke(const Variant & var, Args ...args)
{
	return CallableInvoker<Args...>::canInvoke(var, args...);
}

template <typename ...Args>
inline Variant callableInvoke(const Variant & var, void * instance, Args ...args)
{
	return CallableInvoker<Args...>::invoke(var, instance, args...);
}

template <template <typename, typename> class Container, typename T, typename Allocator, typename ...Args>
inline Variant callableInvoke(const Container<T, Allocator> & callableList, void * instance, Args ...args)
{
	return CallableInvoker<Args...>::invokeCallableList(callableList.begin(), callableList.end(), instance, args...);
}


} // namespace metapp

#endif
