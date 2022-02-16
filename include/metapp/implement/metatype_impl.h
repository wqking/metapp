inline constexpr UnifiedType::UnifiedType()
	: name(""), typeKind(tkEmpty), metaMethod()
{
}

inline constexpr UnifiedType::UnifiedType(
		const char * name,
		const TypeKind typeKind,
		internal_::FuncMetaMethod metaMethod
	)
	: name(name), typeKind(typeKind), metaMethod(metaMethod)
{
}

inline const char * UnifiedType::getName() const
{
	return name;
}

inline TypeKind UnifiedType::getTypeKind() const
{
	return typeKind;
}

inline void UnifiedType::constructDefault(MetaTypeData & data) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::constructDefault;
	param.paramConstruct = { &data };
	metaMethod(param);
}

inline void UnifiedType::constructWith(MetaTypeData & data, const void * value) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::constructWith;
	param.paramConstructWith = { &data, value };
	metaMethod(param);
}

inline const void * UnifiedType::getAddress(const MetaTypeData & data) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::getAddress;
	param.paramGetAddress = { &data, nullptr };
	metaMethod(param);
	return param.paramGetAddress.value;
}

inline bool UnifiedType::canCast(const MetaType * toMetaType) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::canCast;
	param.paramCanCast = { toMetaType, false };
	metaMethod(param);
	return param.paramCanCast.result;
}

inline void UnifiedType::cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::cast;
	param.paramCast = { &data, toMetaType, toData };
	metaMethod(param);
}

inline void UnifiedType::streamIn(std::istream & stream, MetaTypeData & data) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::streamIn;
	param.paramStreamIn = { &stream, &data };
	metaMethod(param);
}

inline void UnifiedType::streamOut(std::ostream & stream, const MetaTypeData & data) const
{
	internal_::MetaMethodParam param;
	param.action = internal_::MetaMethodAction::streamOut;
	param.paramStreamOut = { &stream, &data };
	metaMethod(param);
}


inline constexpr MetaType::MetaType(
		const UnifiedType * unifiedType,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) :
	unifiedType(unifiedType),
	upTypeData(upTypeData),
	typeFlags(typeFlags)
{
}

inline const UnifiedType * MetaType::getUnifiedType() const
{
	return unifiedType;
}

inline const MetaType * MetaType::getUpType() const
{
	return upTypeData.upTypeList[0];
}

inline const MetaType * MetaType::getUpType(const size_t i) const
{
	return upTypeData.upTypeList[i];
}

inline size_t MetaType::getUpTypeCount() const
{
	return upTypeData.count;
}

inline const char * MetaType::getName() const
{
	return unifiedType->getName();
}

inline TypeKind MetaType::getTypeKind() const
{
	return unifiedType->getTypeKind();
}

inline bool MetaType::isConst() const
{
	return typeFlags & tfConst;
}

inline bool MetaType::isVolatile() const
{
	return typeFlags & tfVolatile;
}

inline bool MetaType::isPodStorage() const
{
	return typeFlags & tfPodStorage;
}

inline void MetaType::constructDefault(MetaTypeData & data) const
{
	unifiedType->constructDefault(data);
}

inline void MetaType::constructWith(MetaTypeData & data, const void * value) const
{
	unifiedType->constructWith(data, value);
}

inline const void * MetaType::getAddress(const MetaTypeData & data) const
{
	return unifiedType->getAddress(data);
}

inline bool MetaType::canCast(const MetaType * toMetaType) const
{
	return unifiedType->canCast(toMetaType);
}

inline void MetaType::cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const
{
	unifiedType->cast(data, toMetaType, toData);
}

inline void MetaType::streamIn(std::istream & stream, MetaTypeData & data) const
{
	unifiedType->streamIn(stream, data);
}

inline void MetaType::streamOut(std::ostream & stream, const MetaTypeData & data) const
{
	unifiedType->streamOut(stream, data);
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
