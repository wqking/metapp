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
#include "metapp/utilities/span.h"

namespace metapp {

class Variant;

using ArgumentSpan = metapp::span<const Variant>;

class MetaCallable
{
public:
	class ParameterCountInfo
	{
	public:
		ParameterCountInfo()
			:
				resultCount(0),
				minParameterCount(0),
				maxParameterCount(0)
		{
		}

		ParameterCountInfo(const int resultCount, const int parameterCount)
			:
				resultCount(resultCount),
				minParameterCount(parameterCount),
				maxParameterCount(parameterCount)
		{
		}

		ParameterCountInfo(const int resultCount, const int minParameterCount, const int maxParameterCount)
			:
				resultCount(resultCount),
				minParameterCount(minParameterCount),
				maxParameterCount(maxParameterCount)
		{
		}

		int getResultCount() const {
			return resultCount;
		}

		int getMinParameterCount() const {
			return minParameterCount;
		}

		int getMaxParameterCount() const {
			return maxParameterCount;
		}
	private:
		int resultCount;
		int minParameterCount;
		int maxParameterCount;
	};

	template <typename T>
	static constexpr int detectResultCount()
	{
		return std::is_void<T>::value ? 0 : 1;
	}

public:
	MetaCallable(
		const MetaType * (*getClassType)(const Variant & callable),
		ParameterCountInfo (*getParameterCountInfo)(const Variant & callable),
		const MetaType * (*getReturnType)(const Variant & callable),
		const MetaType * (*getParameterType)(const Variant & callable, const int index),
		int (*rankInvoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments),
		bool (*canInvoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments),
		Variant (*invoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments)
	)
		:
			getClassType(getClassType),
			getParameterCountInfo(getParameterCountInfo),
			getReturnType(getReturnType),
			getParameterType(getParameterType),
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{
	}

	const MetaType * (*getClassType)(const Variant & callable);
	ParameterCountInfo (*getParameterCountInfo)(const Variant & callable);
	const MetaType * (*getReturnType)(const Variant & callable);
	const MetaType * (*getParameterType)(const Variant & callable, const int index);

	int (*rankInvoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments);
	bool (*canInvoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments);
	Variant (*invoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments);

	bool isStatic(const Variant & callable) const {
		return getClassType(callable)->isVoid();
	}
};

template <typename Iterator>
Iterator findCallable(
	Iterator first,
	Iterator last,
	const Variant & instance, 
	const ArgumentSpan & arguments,
	int * resultMaxRank = nullptr
)
{
	Iterator result = last;

	int maxRank = 0;
	for(; first != last; ++first) {
		const Variant & callable = (const Variant &)*first;
		const int rank = callable.getMetaType()->getMetaCallable()->rankInvoke(callable, instance, arguments);
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
	static Variant invoke(const Variant & callable, const Variant & instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[sizeof...(Args) + 1] = {
			arg0,
			args...
		};
		return callable.getMetaType()->getMetaCallable()->invoke(callable, instance, arguments);
	}

	static int rankInvoke(const Variant & callable, const Variant & instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[sizeof...(Args) + 1] = {
			arg0,
			args...
		};
		return callable.getMetaType()->getMetaCallable()->rankInvoke(callable, instance, arguments);
	}

	static bool canInvoke(const Variant & callable, const Variant & instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[sizeof...(Args) + 1] = {
			arg0,
			args...
		};
		return callable.getMetaType()->getMetaCallable()->canInvoke(callable, instance, arguments);
	}

};

template <>
struct CallableInvoker <>
{
	static Variant invoke(const Variant & callable, const Variant & instance)
	{
		return callable.getMetaType()->getMetaCallable()->invoke(callable, instance, {});
	}

	static int rankInvoke(const Variant & callable, const Variant & instance)
	{
		return callable.getMetaType()->getMetaCallable()->rankInvoke(callable, instance, {});
	}

	static bool canInvoke(const Variant & callable, const Variant & instance)
	{
		return callable.getMetaType()->getMetaCallable()->canInvoke(callable, instance, {});
	}

};

inline const MetaType * callableGetClassType(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getClassType(callable);
}

inline MetaCallable::ParameterCountInfo callableGetParameterCountInfo(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getParameterCountInfo(callable);
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
inline int callableRankInvoke(const Variant & callable, const Variant & instance, Args ...args)
{
	return CallableInvoker<Args...>::rankInvoke(callable, instance, args...);
}

template <typename ...Args>
inline bool callableCanInvoke(const Variant & callable, const Variant & instance, Args ...args)
{
	return CallableInvoker<Args...>::canInvoke(callable, instance, args...);
}

template <typename ...Args>
inline Variant callableInvoke(const Variant & callable, const Variant & instance, Args ...args)
{
	return CallableInvoker<Args...>::invoke(callable, instance, args...);
}

inline bool callableIsStatic(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->isStatic(callable);
}


} // namespace metapp

#endif
