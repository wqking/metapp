#ifndef METATYPEDATAUTIL_H_969872685611
#define METATYPEDATAUTIL_H_969872685611

#include "metapp/typekind.h"
#include "metapp/typelist.h"
#include "metapp/metatypedata.h"
#include "metapp/exception.h"
#include "metapp/internal/typeutil_i.h"

namespace metapp {

template <typename T, typename U>
void podCast(const MetaTypeData & data, void * toData)
{
	*(U *)toData = (U)(data.podAs<T>());
}

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
