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

#ifndef METAPP_CAST_H_969872685611
#define METAPP_CAST_H_969872685611

#include "metapp/implement/variant_intf.h"

#include <array>

namespace metapp {

class MetaType;

template <typename T>
constexpr const MetaType * getMetaType();

bool commonCast(
	Variant * result,
	const Variant * fromVar,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
);

template <typename MyType, typename FromTypes>
struct CastFromChecker
{
public:
	static bool castFrom(Variant * result, const Variant * fromVar, const MetaType * fromMetaType)
	{
		auto castFromFunc = findCastFromFunc(fromMetaType);
		if(castFromFunc != nullptr) {
			if(result != nullptr) {
				*result = castFromFunc(fromVar);
			}
			return true;
		}
		return false;
	}

private:
	template <typename FromType>
	struct HelperCastFrom
	{
		using ToType = typename std::remove_cv<typename std::remove_reference<MyType>::type>::type;

		static Variant castFrom(const Variant * fromVar) {
			return static_cast<ToType>(*(FromType *)(fromVar->getAddress()));
		}
	};

	using CastFromFunc = Variant (*)(const Variant * fromVar);

	template <typename ...Types>
	static CastFromFunc doFindCastFromFuncHelper(const MetaType * fromMetaType, TypeList<Types...>)
	{
		constexpr std::size_t typeCount = sizeof...(Types);
		std::array<const MetaType *, typeCount> fromMetaTypeList {
			getMetaType<Types>()...
		};
		std::array<CastFromFunc, typeCount> castFromFuncList {
			&HelperCastFrom<Types>::castFrom...,
		};
		for(std::size_t i = 0; i < typeCount; ++i) {
			if(fromMetaTypeList[i]->equal(fromMetaType)) {
				return castFromFuncList[i];
			}
		}
		return nullptr;
	}

	template <typename ...Types>
	static CastFromFunc doFindCastFromFunc(const MetaType * fromMetaType, TypeList<Types...>)
	{
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			internal_::BoolConstantList<internal_::CanStaticCast<Types, MyType>::value...>
		>::Type;
		return doFindCastFromFuncHelper(fromMetaType, TL());
	}

	static CastFromFunc findCastFromFunc(const MetaType * fromMetaType)
	{
		return doFindCastFromFunc(fromMetaType, FromTypes());
	}

};

template <typename MyType, typename ToTypes>
struct CastToChecker
{
public:
	static bool castTo(Variant * result, const Variant * fromVar, const MetaType * toMetaType)
	{
		auto castToFunc = findCastToFunc(toMetaType);
		if(castToFunc != nullptr) {
			if(result != nullptr) {
				*result = castToFunc(fromVar);
			}
			return true;
		}
		return false;
	}

private:
	template <typename ToType>
	struct HelperCastTo
	{
		using FromType = typename std::remove_cv<typename std::remove_reference<MyType>::type>::type;

		static Variant castTo(const Variant * fromVar) {
			return static_cast<ToType>(*(FromType *)(fromVar->getAddress()));
		}
	};

	using CastToFunc = Variant (*)(const Variant * fromVar);

	template <typename ...Types>
	static CastToFunc doFindCastToFuncHelper(const MetaType * toMetaType, TypeList<Types...>)
	{
		constexpr std::size_t typeCount = sizeof...(Types);
		const std::array<const MetaType *, typeCount> toMetaTypeList {
			getMetaType<Types>()...
		};
		const std::array<CastToFunc, typeCount> castToFuncList {
			&HelperCastTo<Types>::castTo...,
		};

		for(std::size_t i = 0; i < typeCount; ++i) {
			if(toMetaTypeList[i]->equal(toMetaType)) {
				return castToFuncList[i];
			}
		}
		return nullptr;
	}

	template <typename ...Types>
	static CastToFunc doFindCastToFunc(const MetaType * toMetaType, TypeList<Types...>)
	{
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			internal_::BoolConstantList<internal_::CanStaticCast<MyType, Types>::value...>
		>::Type;
		return doFindCastToFuncHelper(toMetaType, TL());
	}

	static CastToFunc findCastToFunc(const MetaType * toMetaType)
	{
		return doFindCastToFunc(toMetaType, ToTypes());
	}

};

template <typename T, typename ToTypes>
struct CastToTypes
{
	static bool cast(Variant * result, const Variant * fromVar, const MetaType * toMetaType)
	{
		return CastToChecker<T, ToTypes>::castTo(result, fromVar, toMetaType)
			|| commonCast(result, fromVar, getMetaType<T>(), toMetaType)
		;
	}
};

template <typename T, typename FromTypes>
struct CastFromTypes
{
	static bool castFrom(Variant * result, const Variant * fromVar, const MetaType * fromMetaType)
	{
		return CastFromChecker<T, FromTypes>::castFrom(result, fromVar, fromMetaType);
	}
};

template <typename T, typename FromToTypes>
struct CastFromToTypes : CastToTypes<T, FromToTypes>, CastFromTypes<T, FromToTypes>
{
};


} // namespace metapp

#include "metapp/metatype.h"

#include "metapp/implement/variant_impl.h"

#endif
