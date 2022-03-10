#ifndef METAPP_METATYPE_IMPL_H_969872685611
#define METAPP_METATYPE_IMPL_H_969872685611

namespace metapp {

inline constexpr MetaType::MetaType(
		const internal_::UnifiedType * (*doGetUnifiedType)(),
		const internal_::MetaTable & metaTable,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept :
	doGetUnifiedType(doGetUnifiedType),
	metaTable(metaTable),
	upTypeData(upTypeData),
	typeFlags(typeFlags)
{
}

inline const void * MetaType::getUnifiedType() const noexcept
{
	return doGetUnifiedType();
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
	return doGetUnifiedType()->getTypeKind();
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
	return doGetUnifiedType()->getMetaClass();
}

inline const MetaCallable * MetaType::getMetaCallable() const
{
	return doGetUnifiedType()->getMetaCallable();
}

inline const MetaAccessible * MetaType::getMetaAccessible() const
{
	return doGetUnifiedType()->getMetaAccessible();
}

inline const MetaEnum * MetaType::getMetaEnum() const
{
	return doGetUnifiedType()->getMetaEnum();
}

inline const MetaIndexable * MetaType::getMetaIndexable() const
{
	return doGetUnifiedType()->getMetaIndexable();
}

inline const MetaIterable * MetaType::getMetaIterable() const
{
	return doGetUnifiedType()->getMetaIterable();
}

inline const MetaStreaming * MetaType::getMetaStreaming() const
{
	return doGetUnifiedType()->getMetaStreaming();
}

inline const MetaMap * MetaType::getMetaMap() const
{
	return doGetUnifiedType()->getMetaMap();
}

inline const MetaMember * MetaType::getMetaMember() const
{
	return doGetUnifiedType()->getMetaMember();
}

inline const void * MetaType::getMetaUser() const
{
	return doGetUnifiedType()->getMetaUser();
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
	return doGetUnifiedType()->constructData(data, copyFrom);
}

inline void MetaType::destroy(void * instance) const
{
	doGetUnifiedType()->destroy(instance);
}

inline Variant MetaType::toReference(const Variant & value) const
{
	return metaTable.toReference(value);
}

inline bool MetaType::canCast(const Variant & value, const MetaType * toMetaType) const
{
	return doGetUnifiedType()->canCast(value, toMetaType);
}

inline Variant MetaType::cast(const Variant & value, const MetaType * toMetaType) const
{
	return doGetUnifiedType()->cast(value, toMetaType);
}

inline bool MetaType::canCastFrom(const Variant & value, const MetaType * fromMetaType) const
{
	return doGetUnifiedType()->canCastFrom(value, fromMetaType);
}

inline Variant MetaType::castFrom(const Variant & value, const MetaType * fromMetaType) const
{
	return doGetUnifiedType()->castFrom(value, fromMetaType);
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
using SelectDeclareClass = typename std::conditional<
	has,
	DeclareMetaType<T>,
	CommonDeclareMetaType<T>
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
		&unifiedTypeGetter<typename std::remove_cv<T>::type>,
		internal_::MetaTable {
			&internal_::SelectDeclareClass<T, internal_::HasMember_toReference<M>::value>::toReference,
		},
		UpTypeGetter<
			typename internal_::SelectDeclareClass<T, internal_::HasMember_UpType<M>::value>::UpType
		>::getUpType(),
		internal_::SelectDeclareClass<T, internal_::HasMember_typeFlags<M>::value>::typeFlags
	);
	return &metaType;
}

inline bool areMetaTypesMatched(const MetaType * fromMetaType, const MetaType * toMetaType)
{
	if(toMetaType->isReference() && ! fromMetaType->isReference()) {
		toMetaType = toMetaType->getUpType();
	}
	else if(! toMetaType->isReference() && fromMetaType->isReference()) {
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


template <typename U>
inline void * CommonDeclareMetaTypeBase::doConstructDefault(
	typename std::enable_if<std::is_default_constructible<U>::value>::type *
)
{
	return new U();
}

template <typename U>
inline void * CommonDeclareMetaTypeBase::doConstructDefault(
	typename std::enable_if<! (std::is_default_constructible<U>::value)>::type *
)
{
	return nullptr;
}

template <typename U>
inline void * CommonDeclareMetaTypeBase::doConstructCopy(
	const void * copyFrom,
	typename std::enable_if<std::is_copy_assignable<U>::value>::type *
)
{
	return new U(*(U *)copyFrom);
}

template <typename U>
inline void * CommonDeclareMetaTypeBase::doConstructCopy(
	const void * /*copyFrom*/,
	typename std::enable_if<! std::is_copy_assignable<U>::value>::type *
)
{
	return nullptr;
}

template <typename P>
inline Variant CommonDeclareMetaTypeBase::doToReference(const Variant & value,
	typename std::enable_if<! std::is_void<P>::value>::type *)
{
	return Variant::create<P &>(**(P **)value.getAddress());

	// Can't call value.get here, otherwise the compiler will go crazy, either dead loop or other error
	//return Variant::create<P &>(*value.get<P *>());
}

template <typename P>
inline Variant CommonDeclareMetaTypeBase::doToReference(const Variant & value,
	typename std::enable_if<std::is_void<P>::value>::type *)
{
	return value;
}

inline bool CommonDeclareMetaTypeBase::doCanCast(
	const MetaType * fromMetaType, const Variant & value, const MetaType * toMetaType)
{
	if((fromMetaType->isReference() == toMetaType->isReference())
		&& internal_::areMetaTypesMatched(fromMetaType, toMetaType)) {
		return true;
	}
	if(fromMetaType->getTypeKind() != tkReference
		&& toMetaType->getTypeKind() == tkReference
		&& fromMetaType->canCast(value, toMetaType->getUpType())
		) {
		return true;
	}
	return false;
}

inline bool CommonDeclareMetaTypeBase::doCast(
	Variant & result, const MetaType * fromMetaType, const Variant & value, const MetaType * toMetaType)
{
	if((fromMetaType->isReference() == toMetaType->isReference())
		&& internal_::areMetaTypesMatched(fromMetaType, toMetaType)) {
		result = Variant::retype(toMetaType, value);
		return true;
	}
	if(! fromMetaType->isReference()
		&& toMetaType->isReference()
		&& fromMetaType->canCast(value, toMetaType->getUpType())
		) {
		result = fromMetaType->cast(value, toMetaType->getUpType());
		return true;
	}
	return false;
}

template <typename T>
const UnifiedType * unifiedTypeGetter()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType (
		SelectDeclareClass<T, HasMember_typeKind<M>::value>::typeKind,
		UnifiedMetaTable {
			&SelectDeclareClass<T, HasMember_constructData<M>::value>::constructData,
			&SelectDeclareClass<T, HasMember_destroy<M>::value>::destroy,
			&SelectDeclareClass<T, HasMember_canCast<M>::value>::canCast,
			&SelectDeclareClass<T, HasMember_cast<M>::value>::cast,
			&SelectDeclareClass<T, HasMember_canCastFrom<M>::value>::canCastFrom,
			&SelectDeclareClass<T, HasMember_castFrom<M>::value>::castFrom,

			MakeMetaInterfaceData<T>::getMetaInterfaceData(),
		}
	);
	return &unifiedType;
}

} // namespace internal_

template <typename T>
inline void * CommonDeclareMetaType<T>::constructData(MetaTypeData * data, const void * copyFrom)
{
	if(data != nullptr) {
		data->construct<Underlying>(copyFrom);
		return nullptr;
	}
	else {
		if(copyFrom == nullptr) {
			return doConstructDefault<Underlying>();
		}
		else {
			return doConstructCopy<Underlying>(copyFrom);
		}
	}
}

template <typename T>
inline void CommonDeclareMetaType<T>::destroy(void * instance)
{
#if defined(METAPP_COMPILER_GCC) || defined(METAPP_COMPILER_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
	delete static_cast<Underlying *>(instance);
#if defined(METAPP_COMPILER_GCC) || defined(METAPP_COMPILER_CLANG)
#pragma GCC diagnostic pop
#endif
}

template <typename T>
inline Variant CommonDeclareMetaType<T>::toReference(const Variant & value)
{
	const MetaType * fromMetaType = value.getMetaType();

	assert(fromMetaType == getMetaType<T>());

	if(fromMetaType->isReference()) {
		return value;
	}
	if(fromMetaType->isPointer()) {
		using P = typename std::remove_pointer<typename std::remove_reference<T>::type>::type;
		return doToReference<P>(value);
	}
	using U = typename std::remove_reference<T>::type;
	return Variant::create<U &>(value.get<U &>());
}

template <typename T>
inline bool CommonDeclareMetaType<T>::canCast(const Variant & value, const MetaType * toMetaType)
{
	const MetaType * fromMetaType = getMetaType<T>();
	if(doCanCast(fromMetaType, value, toMetaType)) {
		return true;
	}
	if(internal_::CastTo<Underlying>::canCastTo(value, toMetaType)) {
		return true;
	}
	if(toMetaType->canCastFrom(value, fromMetaType)) {
		return true;
	}
	return false;
}

template <typename T>
inline Variant CommonDeclareMetaType<T>::cast(const Variant & value, const MetaType * toMetaType)
{
	const MetaType * fromMetaType = getMetaType<T>();
	Variant result;
	if(doCast(result, fromMetaType, value, toMetaType)) {
		return result;
	}
	if(internal_::CastTo<Underlying>::canCastTo(value, toMetaType)) {
		return internal_::CastTo<Underlying>::castTo(value, toMetaType);
	}
	if(toMetaType->canCastFrom(value, fromMetaType)) {
		return toMetaType->castFrom(value, fromMetaType);
	}
	errorBadCast();
	return Variant();
}

template <typename T>
inline bool CommonDeclareMetaType<T>::canCastFrom(const Variant & value, const MetaType * fromMetaType)
{
	return internal_::CastFrom<Underlying>::canCastFrom(value, fromMetaType);
}

template <typename T>
inline Variant CommonDeclareMetaType<T>::castFrom(const Variant & value, const MetaType * fromMetaType)
{
	return internal_::CastFrom<Underlying>::castFrom(value, fromMetaType);
}


inline void * DeclareMetaTypeVoidBase::constructData(MetaTypeData * /*data*/, const void * /*value*/)
{
	return nullptr;
}

inline void DeclareMetaTypeVoidBase::destroy(void * /*instance*/)
{
}

inline Variant DeclareMetaTypeVoidBase::toReference(const Variant & value)
{
	return value;
}

inline bool DeclareMetaTypeVoidBase::canCast(const Variant & /*value*/, const MetaType * /*toMetaType*/)
{
	return false;
}

inline Variant DeclareMetaTypeVoidBase::cast(const Variant & /*value*/, const MetaType * /*toMetaType*/)
{
	errorBadCast();
	return Variant();
}

inline bool DeclareMetaTypeVoidBase::canCastFrom(const Variant & /*value*/, const MetaType * /*fromMetaType*/)
{
	return false;
}

inline Variant DeclareMetaTypeVoidBase::castFrom(const Variant & /*value*/, const MetaType * /*fromMetaType*/)
{
	errorBadCast();
	return Variant();
}

template <typename T>
inline const MetaType * getMetaType()
{
	return internal_::doGetMetaType<T>();
}


} // namespace metapp


#endif
