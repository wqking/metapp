#ifndef METAPP_VARIANT_IMPL_H_969872685611
#define METAPP_VARIANT_IMPL_H_969872685611

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
	metaType->constructData(&data, &value);
}

inline Variant::Variant(const MetaType * metaType) noexcept
	:
		metaType(metaType)
{
	metaType->constructData(&data, nullptr);
}

inline Variant::Variant(const MetaType * metaType, const void * copyFrom) noexcept
	:
		metaType(metaType)
{
	metaType->constructData(&data, copyFrom);
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

inline Variant Variant::clone() const
{
	Variant result;
	result.metaType = metaType;
	result.metaType->constructData(&result.data, metaType->getAddress(data));
	return result;
}

template <typename T>
inline Variant & Variant::set(T value)
{
	metaType = metapp::getMetaType<T>();
	data.reset();
	metaType->constructData(&data, &value);

	return *this;
}

inline Variant & Variant::makeObject(const MetaType * metaType_, void * object_)
{
	metaType = metaType_;
	data.constructObject(std::shared_ptr<void>(object_, [metaType_](void * p) {
		metaType_->destroy(p);
	}));

	return *this;
}

inline Variant & Variant::makeObject(const Variant & object_)
{
	const MetaType * mt = object_.getMetaType();
	if(mt != nullptr && mt->getTypeKind() == tkPointer) {
		mt = mt->getUpType();
	}
	return makeObject(mt, object_.get<void *>());
}

template <typename T>
inline bool Variant::canGet() const
{
	return areMetaTypesMatched(metaType, metapp::getMetaType<T>(), false);
}

template <typename T>
inline auto Variant::get() const -> typename internal_::VariantReturnType<T>::Type
{
	using U = typename internal_::VariantReturnType<T>::Type;
	return (U)(*(typename std::remove_reference<U>::type *)(metaType->getAddress(data)));
}

inline void * Variant::getAddress() const
{
	return metaType->getAddress(data);
}

inline bool Variant::canCast(const MetaType * toMetaType) const
{
	return metaType->canCast(*this, toMetaType);
}

template <typename T>
inline bool Variant::canCast() const
{
	return canCast(metapp::getMetaType<T>());
}

inline Variant Variant::cast(const MetaType * toMetaType) const
{
	assert(canCast(toMetaType));
	return metaType->cast(*this, toMetaType);
}

template <typename T>
inline Variant Variant::cast() const
{
	return cast(metapp::getMetaType<T>());
}

inline const MetaType * Variant::getMetaType() const noexcept
{
	return metaType;
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
	using U = typename std::remove_reference<T>::type;
	stream >> *static_cast<U *>(value.getAddress());
}

template <typename T>
inline auto variantStreamIn(std::istream & /*stream*/, Variant & /*value*/)
	-> typename std::enable_if<! internal_::HasInputStreamOperator<T>::value, void>::type
{
	errorNotSupported("No << input streaming operator.");
}

template <typename T>
inline auto variantStreamOut(std::ostream & stream, const Variant & value)
	-> typename std::enable_if<internal_::HasOutputStreamOperator<T>::value, void>::type
{
	using U = typename std::remove_reference<T>::type;
	stream << *static_cast<const U *>(value.getAddress());
}

template <typename T>
inline auto variantStreamOut(std::ostream & /*stream*/, const Variant & /*value*/)
	-> typename std::enable_if<! internal_::HasOutputStreamOperator<T>::value, void>::type
{
	errorNotSupported("No >> output streaming operator.");
}


} // namespace metapp


#endif
