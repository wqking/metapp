#ifndef VARIANT_IMPL_H_969872685611
#define VARIANT_IMPL_H_969872685611

namespace metapp {

inline Variant::Variant() noexcept
	: 
		metaType(metapp::getMetaType<void>()),
		data()
{
}

template <typename T>
inline Variant::Variant(T value) noexcept
	:
		metaType(metapp::getMetaType<T>())
{
	metaType->constructWith(data, &value);
}

inline Variant::Variant(const MetaType * metaType) noexcept
	:
		metaType(metaType)
{
	metaType->constructDefault(data);
}

inline Variant::Variant(const MetaType * metaType, const MetaTypeData & data) noexcept
	:
		metaType(metaType),
		data(data)
{
}

inline Variant::Variant(const Variant & other) noexcept
	:
		metaType(other.metaType),
		data(other.data)
{
}

inline Variant::Variant(Variant && other) noexcept
	:
		metaType(std::move(other.metaType)),
		data(std::move(other.data))
{
}

template <typename T>
inline Variant & Variant::operator = (T value) noexcept
{
	return set<T>(value);
}

inline Variant & Variant::operator = (const Variant & other) noexcept
{
	metaType = other.metaType;
	data = other.data;

	return *this;
}

inline Variant & Variant::operator = (Variant && other) noexcept
{
	metaType = std::move(other.metaType);
	data = std::move(other.data);

	return *this;
}

template <typename T>
inline Variant & Variant::set(T value)
{
	metaType = metapp::getMetaType<T>();
	metaType->constructWith(data, &value);

	return *this;
}

template <typename T>
inline Variant & Variant::makeObject(const MetaType * metaType_, T * object_)
{
	assert(metaType_->isObjectStorage());

	metaType = metaType_;
	data.object.reset(object_);

	return *this;
}

inline Variant & Variant::makeObject(const MetaType * metaType_, void * object_)
{
	assert(metaType_->isObjectStorage());

	metaType = metaType_;
	data.object = std::shared_ptr<void>(object_, [metaType_](void * p) {
		metaType_->destroy(p);
	});

	return *this;
}

inline Variant & Variant::makeObject(const Variant & object_)
{
	const MetaType * mt = object_.getMetaType();
	while(mt != nullptr && mt->getTypeKind() == tkPointer) {
		mt = mt->getUpType();
	}
	return makeObject(mt, object_.get<void *>());
}

template <typename T>
inline bool Variant::canGet(const bool strictMode) const
{
	return isPossibleSame(metaType, metapp::getMetaType<T>(), strictMode);
}

template <typename T>
inline T Variant::get() const
{
	return *(typename std::remove_reference<T>::type *)(metaType->getAddress(data));
}

template <typename T>
inline auto Variant::getAddress() const -> const typename std::remove_reference<T>::type *
{
	using U = typename std::remove_reference<T>::type;
	return const_cast<const U *>(static_cast<U * >(metaType->getAddress(data)));
}

template <typename T>
inline auto Variant::getAddress() -> typename std::remove_reference<T>::type *
{
	using U = typename std::remove_reference<T>::type;
	return static_cast<U * >(metaType->getAddress(data));
}

template <typename T>
inline bool Variant::canCast() const
{
	const MetaType * toMetaType = metapp::getMetaType<T>();
	return metaType->canCast(toMetaType);
}

template <typename T>
inline Variant Variant::cast() const
{
	const MetaType * toMetaType = metapp::getMetaType<T>();
	assert(metaType->canCast(toMetaType));
	return metaType->cast(*this, toMetaType);
}

inline const MetaType * Variant::getMetaType() const noexcept
{
	return metaType;
}

inline const MetaTypeData & Variant::getMetaTypeData() const noexcept
{
	return data;
}

inline std::istream & operator >> (std::istream & stream, Variant & value)
{
	value.metaType->streamIn(stream, value);
	return stream;
}

inline std::ostream & operator << (std::ostream & stream, const Variant & value)
{
	value.metaType->streamOut(stream, value);
	return stream;
}

inline TypeKind getTypeKind(const Variant & v)
{
	return v.getMetaType()->getTypeKind();
}

template <typename T, typename U>
inline Variant variantCast(const Variant & value)
{
	return (U)(value.get<T>());
}

template <typename T>
inline auto variantStreamIn(std::istream & stream, Variant & value)
	-> typename std::enable_if<internal_::HasInputStreamOperator<T>::value, void>::type
{
	stream >> *value.getAddress<T>();
}

template <typename T>
inline auto variantStreamIn(std::istream & /*stream*/, Variant & /*value*/)
	-> typename std::enable_if<! internal_::HasInputStreamOperator<T>::value, void>::type
{
	errorNoStreamIn();
}

template <typename T>
inline auto variantStreamOut(std::ostream & stream, const Variant & value)
	-> typename std::enable_if<internal_::HasOutputStreamOperator<T>::value, void>::type
{
	stream << *value.getAddress<T>();
}

template <typename T>
inline auto variantStreamOut(std::ostream & /*stream*/, const Variant & /*value*/)
	-> typename std::enable_if<! internal_::HasOutputStreamOperator<T>::value, void>::type
{
	errorNoStreamOut();
}


} // namespace metapp


#endif
