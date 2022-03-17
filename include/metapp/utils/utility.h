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

#ifndef METAPP_UTILITY_H_969872685611
#define METAPP_UTILITY_H_969872685611

#include "metapp/variant.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/metatype.h"
#include "metapp/utils/typelist.h"
#include "metapp/exception.h"

#include <type_traits>

namespace metapp {

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
};

template <>
struct CallableInvoker <>
{
	static Variant invoke(const Variant & func, void * instance)
	{
		return func.getMetaType()->getMetaCallable()->invoke(func, instance, nullptr, 0);
	}
};

template <typename ...Args>
inline Variant invokeCallable(const Variant & func, void * instance, Args ...args)
{
	return CallableInvoker<Args...>::invoke(func, instance, args...);
}

inline const MetaType * getReferredMetaType(const MetaType * metaType)
{
	if(metaType->isPointer() || metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

inline const MetaType * getReferredMetaType(const Variant & value)
{
	return getReferredMetaType(value.getMetaType());
}

template <typename ...Types>
inline const MetaType * getMetaTypeAt(const size_t index)
{
	const MetaType * metaTypeList[] = {
		getMetaType<Types>()...,
		nullptr
	};
	return metaTypeList[index];
}

template <typename ...Types>
inline const MetaType * getMetaTypeAt(const size_t index, const TypeList<Types...> &)
{
	const MetaType * metaTypeList[] = {
		getMetaType<Types>()...,
		nullptr
	};
	return metaTypeList[index];
}

template <typename ToType, typename FromType>
inline void assignValue(ToType & to, const FromType & from,
	typename std::enable_if<std::is_assignable<ToType &, FromType>::value>::type * = nullptr)
{
	using U = typename std::remove_cv<ToType>::type;
	to = (U)from;
}

template <typename ToType, typename FromType>
inline void assignValue(ToType & /*to*/, const FromType & /*from*/,
	typename std::enable_if<! std::is_assignable<ToType &, FromType>::value>::type * = nullptr)
{
	errorUnwritable();
}

inline void verifyVariantWritable(const Variant & var)
{
	auto metaType = getReferredMetaType(var);
	if(metaType->isConst()) {
		errorUnwritable();
	}
}

} // namespace metapp

#endif
