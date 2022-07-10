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

#include <string>
#include <array>

namespace metapp {

namespace internal_ {

template <typename ...Types>
inline const MetaType * getMetaTypeAt(const int index)
{
	std::array<const MetaType *, sizeof...(Types)> metaTypeList {
		getMetaType<Types>()...,
	};
	return metaTypeList[index];
}

template <typename ...Types>
inline const MetaType * getMetaTypeAt(const int index, const TypeList<Types...> &)
{
	std::array<const MetaType *, sizeof...(Types)> metaTypeList {
		getMetaType<Types>()...,
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
	raiseException<UnwritableException>();
}

// Used by MetaAccessible::getClassType that returns void meta type
const MetaType * voidMetaTypeFromVariant(const Variant & var);


template <typename T>
struct SameMetaTypeChecker
{
	static constexpr bool isSame(const MetaType * metaType) {
		return metaType->equal(getMetaType<T>());
	}
};

template <typename A, typename ...Ts>
struct SameMetaTypeChecker <TypeList<A, Ts...> >
{
	static constexpr bool isSame(const MetaType * metaType) {
		return SameMetaTypeChecker<A>::isSame(metaType)
			|| SameMetaTypeChecker<TypeList<Ts...> >::isSame(metaType)
			;
	}
};

template <>
struct SameMetaTypeChecker <TypeList<> >
{
	static constexpr bool isSame(const MetaType * /*metaType*/) {
		return false;
	}
};


} // namespace internal_

} // namespace metapp

#endif
