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

#ifndef METAPP_UTIL_I_H_969872685611
#define METAPP_UTIL_I_H_969872685611

#include "metapp/utilities/utility.h"

#include <string>

namespace metapp {

namespace internal_ {

void verifyVariantWritable(const Variant & var);

template <typename ...Types>
inline const MetaType * getMetaTypeAt(const int index)
{
	const MetaType * metaTypeList[] = {
		getMetaType<Types>()...,
		nullptr
	};
	return metaTypeList[index];
}

template <typename ...Types>
inline const MetaType * getMetaTypeAt(const int index, const TypeList<Types...> &)
{
	const MetaType * metaTypeList[] = {
		getMetaType<Types>()...,
		nullptr
	};
	return metaTypeList[index];
}

template <typename ToType, typename FromType>
inline void assignValue(ToType & to, const FromType & from,
	typename std::enable_if<std::is_assignable<ToType &, FromType>::value
		&& std::is_copy_assignable<FromType>::value>::type * = nullptr)
{
	using U = typename std::remove_cv<ToType>::type;
	to = (U)from;
}

template <typename ToType, typename FromType>
inline void assignValue(ToType & /*to*/, const FromType & /*from*/,
	typename std::enable_if<! (std::is_assignable<ToType &, FromType>::value
		&& std::is_copy_assignable<FromType>::value)>::type * = nullptr)
{
	errorUnwritable();
}


} // namespace internal_

} // namespace metapp

#endif
