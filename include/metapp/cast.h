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

#include "metapp/variant.h"

namespace metapp {

struct CastFromItem
{
	const MetaType * fromMetaType;
	Variant (*castFrom)(const Variant & value);
};

template <typename MyType, typename FromTypes>
struct CastFromChecker
{
public:
	static bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType)
	{
		auto castFromItem = findCastFromItem(fromMetaType);
		if(castFromItem.fromMetaType != nullptr) {
			if(result != nullptr) {
				*result = castFromItem.castFrom(value);
			}
			return true;
		}
		return false;
	}

private:
	template <typename FromType>
	struct HelperCastFrom
	{
		static CastFromItem getCastFromItem() {
			return CastFromItem {
				getMetaType<FromType>(),
				&castFrom
			};
		}

	private:
		using ToType = typename std::remove_cv<typename std::remove_reference<MyType>::type>::type;

		static Variant castFrom(const Variant & value) {
			// If ToType is a class which is constructible from F, but the argument in its constructor
			// is narrower than F, warning C4244 will be issued on MSVC.
			// std::unordered_map is such an example, because it can be constructed from size_type,
			// and cause warning when F is long double.
			// Current to avoid the warning, we may choose not to allow narrowing casting to class,
			// But that can't suppress warning when ToType is container with element of metapp::Variant,
			// such as std::vector<metapp::Variant>
			return static_cast<ToType>(*(FromType *)(value.getAddress()));
		}
	};

	template <typename ...Types>
	static CastFromItem doFindCastFromItemHelper(const MetaType * fromMetaType, TypeList<Types...>)
	{
		const CastFromItem itemList[] = {
			HelperCastFrom<Types>::getCastFromItem()...,
			CastFromItem {}
		};
		for(std::size_t i = 0; i < sizeof(itemList) / sizeof(itemList[0]) - 1; ++i) {
			if(itemList[i].fromMetaType->equal(fromMetaType)) {
				return itemList[i];
			}
		}
		return CastFromItem {};
	}

	template <typename ...Types>
	static CastFromItem doFindCastFromItem(const MetaType * fromMetaType, TypeList<Types...>)
	{
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			internal_::BoolConstantList<internal_::CanStaticCast<Types, MyType>::value...>
		>::Type;
		return doFindCastFromItemHelper(fromMetaType, TL());
	}

	static CastFromItem findCastFromItem(const MetaType * fromMetaType)
	{
		return doFindCastFromItem(fromMetaType, FromTypes());
	}

};

struct CastToItem
{
	const MetaType * toMetaType;
	Variant (*castTo)(const Variant & value);
};

template <typename MyType, typename ToTypes>
struct CastToChecker
{
public:
	static bool castTo(Variant * result, const Variant & value, const MetaType * toMetaType)
	{
		auto castToItem = findCastToItem(toMetaType);
		if(castToItem.toMetaType != nullptr) {
			if(result != nullptr) {
				*result = castToItem.castTo(value);
			}
			return true;
		}
		return false;
	}

private:
	template <typename ToType>
	struct HelperCastTo
	{
		static CastToItem getCastToItem() {
			return CastToItem {
				getMetaType<ToType>(),
				&castTo
			};
		}

	private:
		using FromType = typename std::remove_cv<typename std::remove_reference<MyType>::type>::type;

		static Variant castTo(const Variant & value) {
			return static_cast<ToType>(*(FromType *)(value.getAddress()));
		}
	};

	template <typename ...Types>
	static CastToItem doFindCastToItemHelper(const MetaType * toMetaType, TypeList<Types...>)
	{
		const CastToItem itemList[] = {
			HelperCastTo<Types>::getCastToItem()...,
			CastToItem {}
		};
		for(std::size_t i = 0; i < sizeof(itemList) / sizeof(itemList[0]) - 1; ++i) {
			if(itemList[i].toMetaType->equal(toMetaType)) {
				return itemList[i];
			}
		}
		return CastToItem {};
	}

	template <typename ...Types>
	static CastToItem doFindCastToItem(const MetaType * toMetaType, TypeList<Types...>)
	{
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			internal_::BoolConstantList<internal_::CanStaticCast<MyType, Types>::value...>
		>::Type;
		return doFindCastToItemHelper(toMetaType, TL());
	}

	static CastToItem findCastToItem(const MetaType * toMetaType)
	{
		return doFindCastToItem(toMetaType, ToTypes());
	}

};

template <typename T, typename ToTypes>
struct CastToTypes
{
	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType)
	{
		return CastToChecker<T, ToTypes>::castTo(result, value, toMetaType)
			|| CommonDeclareMetaType<T>::cast(result, value, toMetaType)
			;
	}
};

template <typename T, typename FromTypes>
struct CastFromTypes
{
	static bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType)
	{
		return CastFromChecker<T, FromTypes>::castFrom(result, value, fromMetaType)
			|| (CommonDeclareMetaType<T>::castFrom != nullptr && CommonDeclareMetaType<T>::castFrom(result, value, fromMetaType))
		;
	}
};

template <typename T, typename FromToTypes>
struct CastFromToTypes : CastToTypes<T, FromToTypes>, CastFromTypes<T, FromToTypes>
{
};


} // namespace metapp

#endif
