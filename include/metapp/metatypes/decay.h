#ifndef DECAY_H_969872685611
#define DECAY_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T, typename std::enable_if<std::is_array<T>::value>::type>
	: public DeclareMetaTypeBase<typename std::decay<T>::type>
{
};


} // namespace metapp


#endif

