#ifndef METAPP_CAST_I_H_969872685611
#define METAPP_CAST_I_H_969872685611

namespace metapp {

namespace internal_ {

template <typename MyType>
struct CastFrom
{
	static bool canCastFrom(const Variant & /*value*/, const MetaType * fromMetaType)
	{
		auto castFromItem = findCastFromItem(fromMetaType);
		if(castFromItem != nullptr) {
			return castFromItem->canCastFrom();
		}
		return false;
	}

	static Variant castFrom(const Variant & value, const MetaType * fromMetaType)
	{
		auto castFromItem = findCastFromItem(fromMetaType);
		if(castFromItem != nullptr) {
			return castFromItem->castFrom(value);
		}
		return Variant();
	}

private:
	using Decayed = MyType;

	struct CastFromItem
	{
		const UnifiedType * fromUnifiedType;
		bool (*canCastFrom)();
		Variant (*castFrom)(const Variant & value);
	};

	template <typename FromType>
	struct HelperCastFrom
	{
		static const CastFromItem * getCastFromItem() {
			static CastFromItem item {
				getMetaType<FromType>()->getUnifiedType(),
				&canCastFrom,
				&castFrom
			};
			return &item;
		}

	private:
		using ToType = Decayed;
		static constexpr bool canCastFrom() {
			return std::is_convertible<FromType, ToType>::value;
		}

		static Variant castFrom(const Variant & value) {
			return doCastFrom<FromType>(value);
		}

		template <typename F>
		static Variant doCastFrom(const Variant & value,
			typename std::enable_if<std::is_convertible<F, ToType>::value>::type * = 0) {
			return static_cast<ToType>(value.get<F>());
		}

		template <typename F>
		static Variant doCastFrom(const Variant & /*value*/,
			typename std::enable_if<! std::is_convertible<F, ToType>::value>::type * = 0) {
			return Variant();
		}
	};

	template <typename ...Types>
	static const CastFromItem ** getCastFromItemList(TypeList<Types...>)
	{
		static const CastFromItem * itemList[] = {
			HelperCastFrom<Types>::getCastFromItem()...,
			nullptr
		};
		return itemList;
	}

	template <typename ...Types>
	static const CastFromItem * doFindCastFromItem(const MetaType * fromMetaType, TypeList<Types...>)
	{
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			std::is_convertible<Types, Decayed>::value...
		>::Type;
		const UnifiedType * fromUnifiedType = fromMetaType->getUnifiedType();
		auto itemList = getCastFromItemList(TL());
		while(*itemList != nullptr) {
			if((*itemList)->fromUnifiedType == fromUnifiedType) {
				return *itemList;
			}
			++itemList;
		}
		return nullptr;
	}
	static const CastFromItem * findCastFromItem(const MetaType * fromMetaType)
	{
		return doFindCastFromItem(fromMetaType, AllKnownTypeList());
	}

};

template <typename MyType>
struct CastTo
{
	static bool canCastTo(const Variant & /*value*/, const MetaType * toMetaType)
	{
		auto castToItem = findCastToItem(toMetaType);
		if(castToItem != nullptr) {
			return castToItem->canCastTo();
		}
		return false;
	}

	static Variant castTo(const Variant & value, const MetaType * toMetaType)
	{
		auto castToItem = findCastToItem(toMetaType);
		if(castToItem != nullptr) {
			return castToItem->castTo(value);
		}
		return Variant();
	}

private:
	using Decayed = MyType;

	struct CastToItem
	{
		const UnifiedType * toUnifiedType;
		bool (*canCastTo)();
		Variant (*castTo)(const Variant & value);
	};

	template <typename ToType>
	struct HelperCastTo
	{
		static const CastToItem * getCastToItem() {
			static CastToItem item {
				getMetaType<ToType>()->getUnifiedType(),
				&canCastTo,
				&castTo
			};
			return &item;
		}

	private:
		using FromType = Decayed;
		static constexpr bool canCastTo() {
			return std::is_convertible<FromType, ToType>::value;
		}

		static Variant castTo(const Variant & value) {
			return doCastTo<ToType>(value);
		}

		template <typename T>
		static Variant doCastTo(const Variant & value,
			typename std::enable_if<std::is_convertible<FromType, T>::value>::type * = 0) {
			return static_cast<T>(value.get<FromType>());
		}

		template <typename T>
		static Variant doCastTo(const Variant & /*value*/,
			typename std::enable_if<! std::is_convertible<FromType, T>::value>::type * = 0) {
			return Variant();
		}
	};

	template <typename ...Types>
	static const CastToItem ** getCastToItemList(TypeList<Types...>)
	{
		static const CastToItem * itemList[] = {
			HelperCastTo<Types>::getCastToItem()...,
			nullptr
		};
		return itemList;
	}

	template <typename ...Types>
	static const CastToItem * doFindCastToItem(const MetaType * toMetaType, TypeList<Types...>)
	{
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			std::is_convertible<Decayed, Types>::value...
		>::Type;
		const UnifiedType * toUnifiedType = toMetaType->getUnifiedType();
		auto itemList = getCastToItemList(TL());
		while(*itemList != nullptr) {
			if((*itemList)->toUnifiedType == toUnifiedType) {
				return *itemList;
			}
			++itemList;
		}
		return nullptr;
	}
	static const CastToItem * findCastToItem(const MetaType * toMetaType)
	{
		return doFindCastToItem(toMetaType, AllKnownTypeList());
	}

};


} // namespace internal_

} // namespace metapp

#endif
