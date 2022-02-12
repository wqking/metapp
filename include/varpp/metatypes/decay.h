#ifndef DECAY_H_969872685611
#define DECAY_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <typename T>
struct DeclareMetaType <T,
	typename std::enable_if<std::is_array<T>::value>::type> : public DeclareMetaType<typename std::decay<T>::type>
{
};

template <typename T>
struct DeclareMetaType <T,
	typename std::enable_if<std::is_function<T>::value>::type> : public DeclareMetaType<typename std::decay<T>::type>
{
};


} // namespace varpp


#endif

