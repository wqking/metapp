#ifndef METATYPE_IMPL_H_969872685611
#define METATYPE_IMPL_H_969872685611

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

inline constexpr bool UnifiedType::isCallable() const noexcept
{
	return metaMethodTable.invokeMethodTable != nullptr;
}

inline constexpr bool UnifiedType::isAccessible() const noexcept
{
	return metaMethodTable.accessibleMethodTable != nullptr;
}

inline void * UnifiedType::construct(MetaTypeData * data, const void * copyFrom) const
{
	return metaMethodTable.construct(data, copyFrom);
}

inline void UnifiedType::destroy(void * instance) const
{
	metaMethodTable.destroy(instance);
}

inline const MetaClass * UnifiedType::getMetaClass() const
{
	return metaMethodTable.getMetaClass();
}

inline void * UnifiedType::getAddress(const MetaTypeData & data) const
{
	return metaMethodTable.getAddress(data);
}

inline bool UnifiedType::canCast(const MetaType * toMetaType) const
{
	return metaMethodTable.canCast(toMetaType);
}

inline Variant UnifiedType::cast(const Variant & value, const MetaType * toMetaType) const
{
	return metaMethodTable.cast(value, toMetaType);
}

inline void UnifiedType::streamIn(std::istream & stream, Variant & value) const
{
	metaMethodTable.streamIn(stream, value);
}

inline void UnifiedType::streamOut(std::ostream & stream, const Variant & value) const
{
	metaMethodTable.streamOut(stream, value);
}

inline size_t UnifiedType::getParameterCount() const
{
	if(metaMethodTable.invokeMethodTable == nullptr) {
		throw NotSupportedException("Invoke is not supported.");
	}
	return metaMethodTable.invokeMethodTable->getParameterCount();
}

inline int UnifiedType::rankInvoke(const Variant * arguments, const size_t argumentCount) const
{
	if(metaMethodTable.invokeMethodTable == nullptr) {
		throw NotSupportedException("Invoke is not supported.");
	}
	return metaMethodTable.invokeMethodTable->rankInvoke(arguments, argumentCount);
}

inline bool UnifiedType::canInvoke(const Variant * arguments, const size_t argumentCount) const
{
	if(metaMethodTable.invokeMethodTable == nullptr) {
		throw NotSupportedException("Invoke is not supported.");
	}
	return metaMethodTable.invokeMethodTable->canInvoke(arguments, argumentCount);
}

inline Variant UnifiedType::invoke(
		void * instance,
		const Variant & func,
		const Variant * arguments,
		const size_t argumentCount
	) const
{
	if(metaMethodTable.invokeMethodTable == nullptr) {
		throw NotSupportedException("Invoke is not supported.");
	}
	return metaMethodTable.invokeMethodTable->invoke(instance, func, arguments, argumentCount);
}


inline Variant UnifiedType::accessibleGet(const Variant & accessible, const void * instance) const
{
	if(metaMethodTable.accessibleMethodTable == nullptr) {
		throw NotSupportedException("Accessible is not supported.");
	}

	return metaMethodTable.accessibleMethodTable->accessibleGet(accessible, instance);
}

inline void UnifiedType::accessibleSet(const Variant & accessible, void * instance, const Variant & value) const
{
	if(metaMethodTable.accessibleMethodTable == nullptr) {
		throw NotSupportedException("Accessible is not supported.");
	}

	metaMethodTable.accessibleMethodTable->accessibleSet(accessible, instance, value);
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

inline constexpr bool MetaType::isCallable() const noexcept
{
	return unifiedType->isCallable();
}

inline constexpr bool MetaType::isAccessible() const noexcept
{
	return unifiedType->isAccessible();
}

inline constexpr bool MetaType::isPodStorage() const noexcept
{
	return typeFlags & tfPodStorage;
}

inline constexpr bool MetaType::isObjectStorage() const noexcept
{
	return ! isPodStorage();
}

inline void * MetaType::construct(MetaTypeData * data, const void * copyFrom) const
{
	return unifiedType->construct(data, copyFrom);
}

inline void MetaType::destroy(void * instance) const
{
	unifiedType->destroy(instance);
}

inline const MetaClass * MetaType::getMetaClass() const
{
	return unifiedType->getMetaClass();
}

inline void * MetaType::getAddress(const MetaTypeData & data) const
{
	return unifiedType->getAddress(data);
}

inline bool MetaType::canCast(const MetaType * toMetaType) const
{
	return unifiedType->canCast(toMetaType);
}

inline Variant MetaType::cast(const Variant & value, const MetaType * toMetaType) const
{
	return unifiedType->cast(value, toMetaType);
}

inline void MetaType::streamIn(std::istream & stream, Variant & value) const
{
	unifiedType->streamIn(stream, value);
}

inline void MetaType::streamOut(std::ostream & stream, const Variant & value) const
{
	unifiedType->streamOut(stream, value);
}

inline size_t MetaType::getParameterCount() const
{
	return unifiedType->getParameterCount();
}

inline int MetaType::rankInvoke(const Variant * arguments, const size_t argumentCount) const
{
	return unifiedType->rankInvoke(arguments, argumentCount);
}

inline bool MetaType::canInvoke(const Variant * arguments, const size_t argumentCount) const
{
	return unifiedType->canInvoke(arguments, argumentCount);
}

inline Variant MetaType::invoke(
		void * instance,
		const Variant & func,
		const Variant * arguments,
		const size_t argumentCount
	) const
{
	return unifiedType->invoke(instance, func, arguments, argumentCount);
}

inline Variant MetaType::accessibleGet(const Variant & accessible, const void * instance) const
{
	return unifiedType->accessibleGet(accessible, instance);
}

inline void MetaType::accessibleSet(const Variant & accessible, void * instance, const Variant & value) const
{
	unifiedType->accessibleSet(accessible, instance, value);
}


namespace internal_ {

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<std::is_same<T, NoneUpType>::value, const MetaType *>::type;
template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<! std::is_same<T, NoneUpType>::value, const MetaType *>::type;

template <typename T>
struct UpTypeGetter;

template <typename Arg0, typename ...Args>
struct UpTypeGetter <TypeList<Arg0, Args...> >
{
	static const MetaType ** makeUpTypeList()
	{
		static std::array<const MetaType *, sizeof...(Args) + 1> upTypeList {
			getMetaType<Arg0>(),
			getMetaType<Args>()...,
		};
		return upTypeList.data();
	}

	static UpTypeData getUpType() {
		return {
			makeUpTypeList(),
			(uint16_t)(sizeof...(Args) + 1)
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
		UpTypeGetter<typename M::UpType>::getUpType(),
		M::typeFlags
	);
	return &metaType;
}

} // namespace internal_

template <typename T>
inline const MetaType * getMetaType()
{
	return internal_::doGetMetaType<T>();
}

template <typename Iterator>
inline bool matchUpTypeKinds(const MetaType * metaType, Iterator begin, Iterator end)
{
	while(begin != end) {
		if(metaType == nullptr) {
			return false;
		}
		if(metaType->getTypeKind() != *begin) {
			return false;
		}
		metaType = metaType->getUpType();
		++begin;
	}
	return true;
}

template <typename T>
inline bool matchUpTypeKinds(const MetaType * metaType, const std::initializer_list<T> & typeKindList)
{
	return matchUpTypeKinds(metaType, std::begin(typeKindList), std::end(typeKindList));
}

template <typename T, typename U>
inline bool matchUpTypeKinds(const MetaType * metaType, const U & typeKindList)
{
	return matchUpTypeKinds(metaType, std::begin(typeKindList), std::end(typeKindList));
}

inline bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode)
{
	if(toMetaType->getTypeKind() == tkReference && fromMetaType->getTypeKind() != tkReference) {
		toMetaType = toMetaType->getUpType();
	}
	else if(toMetaType->getTypeKind() != tkReference && fromMetaType->getTypeKind() == tkReference) {
		fromMetaType = fromMetaType->getUpType();
	}
	if(strictMode) {
		if(toMetaType == fromMetaType) {
			return true;
		}
		if(toMetaType == nullptr || fromMetaType == nullptr) {
			return false;
		}
		return toMetaType->getUnifiedType() == fromMetaType->getUnifiedType();
	}
	else {
		if(toMetaType->getTypeKind() == tkReference && fromMetaType->getTypeKind() == tkReference) {
			return true;
		}
		if(toMetaType->getTypeKind() == tkPointer && fromMetaType->getTypeKind() == tkPointer) {
			return true;
		}
		return toMetaType->getUnifiedType() == fromMetaType->getUnifiedType();
	}
}


} // namespace metapp


#endif
