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
	return metaType->cast(data, toMetaType);
}

inline const MetaType * Variant::getMetaType() const noexcept
{
	return metaType;
}

inline std::istream & operator >> (std::istream & stream, Variant & v)
{
	v.metaType->streamIn(stream, v.data);
	return stream;
}

inline std::ostream & operator << (std::ostream & stream, const Variant & v)
{
	v.metaType->streamOut(stream, v.data);
	return stream;
}

inline TypeKind getTypeKind(const Variant & v)
{
	return v.getMetaType()->getTypeKind();
}


} // namespace metapp


#endif
