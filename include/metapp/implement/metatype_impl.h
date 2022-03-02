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

inline const MetaArray * UnifiedType::getMetaArray() const
{
	return static_cast<const MetaArray *>(doGetMetaInterface(internal_::mikMetaArray));
}

inline const MetaEnum * UnifiedType::getMetaEnum() const
{
	return static_cast<const MetaEnum *>(doGetMetaInterface(internal_::mikMetaEnum));
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

inline bool UnifiedType::canCast(const Variant & value, const MetaType * toMetaType) const
{
	return metaMethodTable.canCast(value, toMetaType);
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

inline constexpr bool MetaType::isClassMember() const noexcept
{
	return typeFlags & tfClassMember;
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

inline const MetaArray * MetaType::getMetaArray() const
{
	return unifiedType->getMetaArray();
}

inline const MetaEnum * MetaType::getMetaEnum() const
{
	return unifiedType->getMetaEnum();
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

inline bool MetaType::canCast(const Variant & value, const MetaType * toMetaType) const
{
	return unifiedType->canCast(value, toMetaType);
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


namespace internal_ {

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

inline bool areMetaTypesMatched(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode)
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

template <typename T>
inline const MetaType * getMetaType()
{
	return internal_::doGetMetaType<T>();
}


} // namespace metapp


#endif
