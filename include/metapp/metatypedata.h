#ifndef METATYPEDATA_H_969872685611
#define METATYPEDATA_H_969872685611

#include "metapp/exception.h"
#include "metapp/internal/typeutil_i.h"

#include <memory>
#include <array>

namespace metapp {

constexpr size_t podSize = internal_::MaxOfInt<
		sizeof(long long),
		sizeof(long double),
		sizeof(void *)
	>::value;

struct MetaTypeData
{
	std::array<uint8_t, podSize> pod;
	std::shared_ptr<void> object;

	template <typename T>
	T & podAs() const {
		return *(T *)(pod.data());
	}
};

template <typename T>
auto podStreamIn(std::istream & stream, MetaTypeData & data)
-> typename std::enable_if<internal_::HasInputStreamOperator<T>::value, void>::type
{
	stream >> data.podAs<T>();
}

template <typename T>
auto podStreamIn(std::istream & /*stream*/, MetaTypeData & /*data*/)
-> typename std::enable_if<! internal_::HasInputStreamOperator<T>::value, void>::type
{
	errorNoStreamIn();
}

template <typename T>
auto podStreamOut(std::ostream & stream, const MetaTypeData & data)
-> typename std::enable_if<internal_::HasOutputStreamOperator<T>::value, void>::type
{
	stream << data.podAs<T>();
}

template <typename T>
auto podStreamOut(std::ostream & /*stream*/, const MetaTypeData & /*data*/)
-> typename std::enable_if<! internal_::HasOutputStreamOperator<T>::value, void>::type
{
	errorNoStreamOut();
}

template <typename T>
auto objectStreamIn(std::istream & stream, MetaTypeData & data)
-> typename std::enable_if<internal_::HasInputStreamOperator<T>::value, void>::type
{
	stream >> *static_cast<T *>(data.object.get());
}

template <typename T>
auto objectStreamIn(std::istream & /*stream*/, MetaTypeData & /*data*/)
-> typename std::enable_if<! internal_::HasInputStreamOperator<T>::value, void>::type
{
	errorNoStreamIn();
}

template <typename T>
auto objectStreamOut(std::ostream & stream, const MetaTypeData & data)
-> typename std::enable_if<internal_::HasOutputStreamOperator<T>::value, void>::type
{
	stream << *static_cast<T *>(data.object.get());
}

template <typename T>
auto objectStreamOut(std::ostream & /*stream*/, const MetaTypeData & /*data*/)
-> typename std::enable_if<! internal_::HasOutputStreamOperator<T>::value, void>::type
{
	errorNoStreamOut();
}


} // namespace metapp

#endif
