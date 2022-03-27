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

#ifndef METAPP_CAST_I_H_969872685611
#define METAPP_CAST_I_H_969872685611

namespace metapp {

namespace internal_ {

template <typename From, typename To>
struct CanCastSafely
{
	static constexpr bool value =
		CanStaticCast<From, To>::value
		&& (
			! (
				std::is_class<typename std::remove_reference<To>::type>::value
				)
			|| ! IsNarrowingCast<From, To>::value
			)
		;
};

template <typename MyType>
struct CastFrom
{
private:
	struct CastFromItem
	{
		const void * fromUnifiedType;
		Variant (*castFrom)(const Variant & value);
	};

public:
	static bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType)
	{
		auto castFromItem = findCastFromItem(fromMetaType);
		if(castFromItem.fromUnifiedType != nullptr) {
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
				getMetaType<FromType>()->getUnifiedType(),
				&castFrom
			};
		}

	private:
		using ToType = typename std::remove_reference<MyType>::type;

		static Variant castFrom(const Variant & value) {
			// If ToType is a class which is constructible from F, but the argument in its constructor
			// is narrower than F, warning C4244 will be issued on MSVC.
			// std::unordered_map is such an example, because it can be constructed from size_type,
			// and cause warning when F is long double.
			// Current to avoid the warning, we don't allow narrowing casting to class, see CanCastSafely
			// But that can't suppress warning when ToType is container with element of metapp::Variant,
			// such as std::vector<metapp::Variant>
			return static_cast<ToType>(*(FromType *)(value.getAddress()));
		}
	};

	template <typename ...Types>
	static CastFromItem doFindCastFromItemHelper(const MetaType * fromMetaType, TypeList<Types...>)
	{
		const void * fromUnifiedType = fromMetaType->getUnifiedType();
		const CastFromItem itemList[] = {
			HelperCastFrom<Types>::getCastFromItem()...,
			CastFromItem {}
		};
		for(size_t i = 0; i < sizeof(itemList) / sizeof(itemList[0]); ++i) {
			if(itemList[i].fromUnifiedType == fromUnifiedType) {
				return itemList[i];
			}
		}
		return CastFromItem {};
	}

	template <typename ...Types>
	static CastFromItem doFindCastFromItem(const MetaType * fromMetaType, TypeList<Types...>)
	{
		using TL = typename FilterTypes<
			TypeList<Types...>,
			BoolConstantList<CanCastSafely<Types, MyType>::value...>
		>::Type;
		return doFindCastFromItemHelper(fromMetaType, TL());
	}

	static CastFromItem findCastFromItem(const MetaType * fromMetaType)
	{
		return doFindCastFromItem(fromMetaType, AllKnownTypeList());
	}

};

template <typename MyType>
struct CastTo
{
private:
	struct CastToItem
	{
		const void * toUnifiedType;
		Variant (*castTo)(const Variant & value);
	};

public:
	static bool castTo(Variant * result, const Variant & value, const MetaType * toMetaType)
	{
		auto castToItem = findCastToItem(toMetaType);
		if(castToItem.toUnifiedType != nullptr) {
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
				getMetaType<ToType>()->getUnifiedType(),
				&castTo
			};
		}

	private:
		using FromType = typename std::remove_reference<MyType>::type;

		static Variant castTo(const Variant & value) {
			return static_cast<ToType>(*(FromType *)(value.getAddress()));
		}
	};

	template <typename ...Types>
	static CastToItem doFindCastToItemHelper(const MetaType * toMetaType, TypeList<Types...>)
	{
		const void * toUnifiedType = toMetaType->getUnifiedType();
		const CastToItem itemList[] = {
			HelperCastTo<Types>::getCastToItem()...,
			CastToItem {}
		};
		for(size_t i = 0; i < sizeof(itemList) / sizeof(itemList[0]); ++i) {
			if(itemList[i].toUnifiedType == toUnifiedType) {
				return itemList[i];
			}
		}
		return CastToItem {};
	}

	template <typename ...Types>
	static CastToItem doFindCastToItem(const MetaType * toMetaType, TypeList<Types...>)
	{
		using TL = typename FilterTypes<
			TypeList<Types...>,
			BoolConstantList<CanStaticCast<MyType, Types>::value...>
		>::Type;
		return doFindCastToItemHelper(toMetaType, TL());
	}

	static CastToItem findCastToItem(const MetaType * toMetaType)
	{
		return doFindCastToItem(toMetaType, AllKnownTypeList());
	}

};


} // namespace internal_

} // namespace metapp

#endif
