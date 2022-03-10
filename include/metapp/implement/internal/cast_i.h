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
			typename std::enable_if<CanCastSafely<F, ToType>::value>::type * = nullptr) {
			// If ToType is a class which is constructible from F, but the argument in its constructor
			// is narrow than F, warning C4244 will be issued on MSVC.
			// std::unordered_map is such an example, because it can be construct from size_type,
			// and cause warning when F is long double.
			// Current to avoid the warning, we don't allow narrowing casting to class, see CanCastSafely
			// But that can't suppress warning when ToType is container with element of metapp::Variant,
			// such as std::vector<metapp::Variant>
			return static_cast<ToType>(*(F *)(value.getAddress()));
		}

		template <typename F>
		static Variant doCastFrom(const Variant & /*value*/,
			typename std::enable_if<! CanCastSafely<F, ToType>::value>::type * = nullptr) {
			return Variant();
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
			BoolConstantList<CanCastSafely<Types, Decayed>::value...>
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
			typename std::enable_if<CanStaticCast<FromType, T>::value>::type * = nullptr) {
			return static_cast<T>(*(FromType *)(value.getAddress()));
		}

		template <typename T>
		static Variant doCastTo(const Variant & /*value*/,
			typename std::enable_if<! CanStaticCast<FromType, T>::value>::type * = nullptr) {
			return Variant();
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
			BoolConstantList<CanStaticCast<Decayed, Types>::value...>
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
