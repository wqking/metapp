#ifndef METAPP_CAST_I_H_969872685611
#define METAPP_CAST_I_H_969872685611

namespace metapp {

namespace internal_ {

template <typename MyType>
struct CastFrom
{
private:
	struct CastFromItem
	{
		const UnifiedType * fromUnifiedType;
		Variant (*castFrom)(const Variant & value);
	};

public:
	static bool canCastFrom(const Variant & /*value*/, const MetaType * fromMetaType)
	{
		auto castFromItem = findCastFromItem(fromMetaType);
		return castFromItem.fromUnifiedType != nullptr;
	}

	static Variant castFrom(const Variant & value, const MetaType * fromMetaType)
	{
		auto castFromItem = findCastFromItem(fromMetaType);
		if(castFromItem.fromUnifiedType != nullptr) {
			return castFromItem.castFrom(value);
		}
		return Variant();
	}

private:
	using Decayed = MyType;

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
		using ToType = Decayed;

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
	static CastFromItem doFindCastFromItemHelper(const MetaType * fromMetaType, TypeList<Types...>)
	{
		const UnifiedType * fromUnifiedType = fromMetaType->getUnifiedType();
		static const CastFromItem itemList[] = {
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
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			std::is_convertible<Types, Decayed>::value...
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
		const UnifiedType * toUnifiedType;
		Variant (*castTo)(const Variant & value);
	};

public:
	static bool canCastTo(const Variant & /*value*/, const MetaType * toMetaType)
	{
		auto castToItem = findCastToItem(toMetaType);
		return castToItem.toUnifiedType != nullptr;
	}

	static Variant castTo(const Variant & value, const MetaType * toMetaType)
	{
		auto castToItem = findCastToItem(toMetaType);
		if(castToItem.toUnifiedType != nullptr) {
			return castToItem.castTo(value);
		}
		return Variant();
	}

private:
	using Decayed = MyType;

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
		using FromType = Decayed;

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
	static CastToItem doFindCastToItemHelper(const MetaType * toMetaType, TypeList<Types...>)
	{
		const UnifiedType * toUnifiedType = toMetaType->getUnifiedType();
		static const CastToItem itemList[] = {
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
		using TL = typename internal_::FilterTypes<
			TypeList<Types...>,
			std::is_convertible<Decayed, Types>::value...
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
