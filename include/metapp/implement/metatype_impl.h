#ifndef METAPP_METATYPE_IMPL_H_969872685611
#define METAPP_METATYPE_IMPL_H_969872685611

namespace metapp {

inline constexpr UnifiedType::UnifiedType(
		const TypeKind typeKind,
		const internal_::MetaMethodTable & metaMethodTable
	) noexcept
	: typeKind(typeKind), metaMethodTable(metaMethodTable)
{
}

inline TypeKind UnifiedType::getTypeKind() const noexcept
{
	return typeKind;
}

inline const MetaClass * UnifiedType::getMetaClass() const
{
	return static_cast<const MetaClass *>(doGetMetaInterface(internal_::mikMetaClass));
}

inline const MetaCallable * UnifiedType::getMetaCallable() const
{
	return static_cast<const MetaCallable *>(doGetMetaInterface(internal_::mikMetaCallable));
}

inline const MetaAccessible * UnifiedType::getMetaAccessible() const
{
	return static_cast<const MetaAccessible *>(doGetMetaInterface(internal_::mikMetaAccessible));
}

inline const MetaEnum * UnifiedType::getMetaEnum() const
{
	return static_cast<const MetaEnum *>(doGetMetaInterface(internal_::mikMetaEnum));
}

inline const MetaIndexable * UnifiedType::getMetaIndexable() const
{
	return static_cast<const MetaIndexable *>(doGetMetaInterface(internal_::mikMetaIndexable));
}

inline const MetaIterable * UnifiedType::getMetaIterable() const
{
	return static_cast<const MetaIterable *>(doGetMetaInterface(internal_::mikMetaIterable));
}

inline const MetaStreaming * UnifiedType::getMetaStreaming() const
{
	return static_cast<const MetaStreaming *>(doGetMetaInterface(internal_::mikMetaStreaming));
}

inline const MetaMap * UnifiedType::getMetaMap() const
{
	return static_cast<const MetaMap *>(doGetMetaInterface(internal_::mikMetaMap));
}

inline const MetaMember * UnifiedType::getMetaMember() const
{
	return static_cast<const MetaMember *>(doGetMetaInterface(internal_::mikMetaMember));
}

inline const void * UnifiedType::getMetaUser() const
{
	return static_cast<const void *>(doGetMetaInterface(internal_::mikMetaUser));
}

inline const void * UnifiedType::doGetMetaInterface(const internal_::MetaInterfaceKind kind) const
{
	if((kind & metaMethodTable.metaInterfaceData.kinds) != 0) {
		if(metaMethodTable.metaInterfaceData.items[0].kind == kind) {
			return metaMethodTable.metaInterfaceData.items[0].getter();
		}
		if(metaMethodTable.metaInterfaceData.count > 1) {
			for(uint16_t i = 1; i < metaMethodTable.metaInterfaceData.count; ++i) {
				if(metaMethodTable.metaInterfaceData.items[i].kind == kind) {
					return metaMethodTable.metaInterfaceData.items[i].getter();
				}
			}
		}
	}
	return nullptr;
}

inline void * UnifiedType::constructData(MetaTypeData * data, const void * copyFrom) const
{
	return metaMethodTable.constructData(data, copyFrom);
}

inline void UnifiedType::destroy(void * instance) const
{
	metaMethodTable.destroy(instance);
}

inline void * UnifiedType::getAddress(const MetaTypeData & data) const
{
	return metaMethodTable.getAddress(data);
}

inline Variant UnifiedType::toReference(const Variant & value) const
{
	return metaMethodTable.toReference(value);
}

inline bool UnifiedType::canCast(const Variant & value, const MetaType * toMetaType) const
{
	return metaMethodTable.canCast(value, toMetaType);
}

inline Variant UnifiedType::cast(const Variant & value, const MetaType * toMetaType) const
{
	return metaMethodTable.cast(value, toMetaType);
}

inline bool UnifiedType::canCastFrom(const Variant & value, const MetaType * fromMetaType) const
{
	return metaMethodTable.canCastFrom(value, fromMetaType);
}

inline Variant UnifiedType::castFrom(const Variant & value, const MetaType * fromMetaType) const
{
	return metaMethodTable.castFrom(value, fromMetaType);
}

inline constexpr MetaType::MetaType(
		const UnifiedType * unifiedType,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept :
	unifiedType(unifiedType),
	upTypeData(upTypeData),
	typeFlags(typeFlags)
{
}

inline const UnifiedType * MetaType::getUnifiedType() const noexcept
{
	return unifiedType;
}

inline const MetaType * MetaType::getUpType() const noexcept
{
	return upTypeData.upTypeList[0];
}

inline const MetaType * MetaType::getUpType(const size_t i) const
{
	return upTypeData.upTypeList[i];
}

inline size_t MetaType::getUpTypeCount() const noexcept
{
	return upTypeData.count;
}

inline TypeKind MetaType::getTypeKind() const noexcept
{
	return unifiedType->getTypeKind();
}

inline constexpr bool MetaType::isConst() const noexcept
{
	return typeFlags & tfConst;
}

inline constexpr bool MetaType::isVolatile() const noexcept
{
	return typeFlags & tfVolatile;
}

inline constexpr bool MetaType::isPointer() const noexcept
{
	return typeFlags & tfPointer;
}

inline constexpr bool MetaType::isReference() const noexcept
{
	return typeFlags & tfReference;
}

inline const MetaClass * MetaType::getMetaClass() const
{
	return unifiedType->getMetaClass();
}

inline const MetaCallable * MetaType::getMetaCallable() const
{
	return unifiedType->getMetaCallable();
}

inline const MetaAccessible * MetaType::getMetaAccessible() const
{
	return unifiedType->getMetaAccessible();
}

inline const MetaEnum * MetaType::getMetaEnum() const
{
	return unifiedType->getMetaEnum();
}

inline const MetaIndexable * MetaType::getMetaIndexable() const
{
	return unifiedType->getMetaIndexable();
}

inline const MetaIterable * MetaType::getMetaIterable() const
{
	return unifiedType->getMetaIterable();
}

inline const MetaStreaming * MetaType::getMetaStreaming() const
{
	return unifiedType->getMetaStreaming();
}

inline const MetaMap * MetaType::getMetaMap() const
{
	return unifiedType->getMetaMap();
}

inline const MetaMember * MetaType::getMetaMember() const
{
	return unifiedType->getMetaMember();
}

inline const void * MetaType::getMetaUser() const
{
	return unifiedType->getMetaUser();
}

inline void * MetaType::construct() const
{
	return constructData(nullptr, nullptr);
}

inline void * MetaType::copyConstruct(const void * copyFrom) const
{
	return constructData(nullptr, copyFrom);
}

inline void * MetaType::constructData(MetaTypeData * data, const void * copyFrom) const
{
	return unifiedType->constructData(data, copyFrom);
}

inline void MetaType::destroy(void * instance) const
{
	unifiedType->destroy(instance);
}

inline void * MetaType::getAddress(const MetaTypeData & data) const
{
	return unifiedType->getAddress(data);
}

inline Variant MetaType::toReference(const Variant & value) const
{
	return unifiedType->toReference(value);
}

inline bool MetaType::canCast(const Variant & value, const MetaType * toMetaType) const
{
	return unifiedType->canCast(value, toMetaType);
}

inline Variant MetaType::cast(const Variant & value, const MetaType * toMetaType) const
{
	return unifiedType->cast(value, toMetaType);
}

inline bool MetaType::canCastFrom(const Variant & value, const MetaType * fromMetaType) const
{
	return unifiedType->canCastFrom(value, fromMetaType);
}

inline Variant MetaType::castFrom(const Variant & value, const MetaType * fromMetaType) const
{
	return unifiedType->castFrom(value, fromMetaType);
}

namespace internal_ {

template <typename T>
struct UpTypeGetter;

template <typename Type0, typename ...Types>
struct UpTypeGetter <TypeList<Type0, Types...> >
{
	static const MetaType ** makeUpTypeList()
	{
		static std::array<const MetaType *, sizeof...(Types) + 1> upTypeList {
			getMetaType<Type0>(),
			getMetaType<Types>()...,
		};
		return upTypeList.data();
	}

	static UpTypeData getUpType() {
		return {
			makeUpTypeList(),
			(uint16_t)(sizeof...(Types) + 1)
		};
	}
};

template <>
struct UpTypeGetter <TypeList<> >
{
	static UpTypeData getUpType() {
		return {
			nullptr,
			(uint16_t)0
		};
	}
};

template <typename T>
struct UpTypeGetter
{
	static UpTypeData getUpType() {
		return UpTypeGetter<TypeList<T> >::getUpType();
	}
};

template <typename T, bool has>
struct SelectFallback;

template <typename T>
struct SelectFallback <T, true>
{
	using Type = typename DeclareMetaType<T>::Fallback;
};

template <typename T>
struct SelectFallback <T, false>
{
	using Type = CommonDeclareMetaType<T>;
};

template <typename T, bool has>
using SelectDeclareClass = typename std::conditional<
	has,
	DeclareMetaType<T>,
	typename SelectFallback<T, HasMember_Fallback<DeclareMetaType<T> >::value>::Type
>::type;

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type
{
	return nullptr;
}

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<! std::is_same<T, internal_::NoneUpType>::value, const MetaType *>::type
{
	using M = DeclareMetaType<T>;

	static const MetaType metaType (
		getUnifiedType<typename std::remove_cv<T>::type>(),
		UpTypeGetter<
			typename internal_::SelectDeclareClass<T, internal_::HasMember_UpType<M>::value>::UpType
		>::getUpType(),
		internal_::SelectDeclareClass<T, internal_::HasMember_typeFlags<M>::value>::typeFlags
	);
	return &metaType;
}

inline bool areMetaTypesMatched(const MetaType * fromMetaType, const MetaType * toMetaType)
{
	if(toMetaType->getTypeKind() == tkReference && fromMetaType->getTypeKind() != tkReference) {
		toMetaType = toMetaType->getUpType();
	}
	else if(toMetaType->getTypeKind() != tkReference && fromMetaType->getTypeKind() == tkReference) {
		fromMetaType = fromMetaType->getUpType();
	}

	if(toMetaType->isReference() && fromMetaType->isReference()) {
		return true;
	}
	if((toMetaType->isPointer()) && (fromMetaType->isPointer())) {
		return true;
	}

	return toMetaType->getUnifiedType() == fromMetaType->getUnifiedType();
}


} // namespace internal_

template <typename T>
const UnifiedType * getUnifiedType()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType (
		internal_::SelectDeclareClass<T, internal_::HasMember_typeKind<M>::value>::typeKind,
		internal_::MetaMethodTable {
			&internal_::SelectDeclareClass<T, internal_::HasMember_constructData<M>::value>::constructData,
			&internal_::SelectDeclareClass<T, internal_::HasMember_destroy<M>::value>::destroy,
			&internal_::SelectDeclareClass<T, internal_::HasMember_getAddress<M>::value>::getAddress,
			&internal_::SelectDeclareClass<T, internal_::HasMember_toReference<M>::value>::toReference,
			&internal_::SelectDeclareClass<T, internal_::HasMember_canCast<M>::value>::canCast,
			&internal_::SelectDeclareClass<T, internal_::HasMember_cast<M>::value>::cast,
			&internal_::SelectDeclareClass<T, internal_::HasMember_canCastFrom<M>::value>::canCastFrom,
			&internal_::SelectDeclareClass<T, internal_::HasMember_castFrom<M>::value>::castFrom,
			
			internal_::MakeMetaInterfaceData<T>::getMetaInterfaceData(),
		}
	);
	return &unifiedType;
}

template <typename T>
inline const MetaType * getMetaType()
{
	return internal_::doGetMetaType<T>();
}


} // namespace metapp


#endif
