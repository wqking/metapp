#ifndef METATYPE_I_H_969872685611
#define METATYPE_I_H_969872685611

#include "metapp/typekind.h"
#include "metapp/typelist.h"
#include "metapp/metatypedata.h"
#include "metapp/internal/typeutil_i.h"

#include <stdexcept>

namespace metapp {

class MetaType;
bool probablySame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode = false);
template <typename T>
const MetaType * getMetaType();

namespace internal_ {

inline void errorNoStreamIn()
{
	throw std::runtime_error("No << input streaming operator.");
}

inline void errorNoStreamOut()
{
	throw std::runtime_error("No >> output streaming operator.");
}

template <typename T, typename U>
void podCast(const MetaTypeData & data, void * toData)
{
	*(U *)toData = (U)(data.podAs<T>());
}

template <typename T>
auto podStreamIn(std::istream & stream, MetaTypeData & data)
	-> typename std::enable_if<HasInputStreamOperator<T>::value, void>::type
{
	stream >> data.podAs<T>();
}

template <typename T>
auto podStreamIn(std::istream & /*stream*/, MetaTypeData & /*data*/)
	-> typename std::enable_if<! HasInputStreamOperator<T>::value, void>::type
{
	errorNoStreamIn();
}

template <typename T>
auto podStreamOut(std::ostream & stream, const MetaTypeData & data)
	-> typename std::enable_if<HasOutputStreamOperator<T>::value, void>::type
{
	stream << data.podAs<T>();
}

template <typename T>
auto podStreamOut(std::ostream & /*stream*/, MetaTypeData & /*data*/)
	-> typename std::enable_if<! HasOutputStreamOperator<T>::value, void>::type
{
	errorNoStreamOut();
}


} // namespace internal_

} // namespace metapp

#endif
