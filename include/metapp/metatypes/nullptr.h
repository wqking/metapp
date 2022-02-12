#ifndef NULLPTR_H_969872685611
#define NULLPTR_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct DeclareMetaType <std::nullptr_t> : public DeclareMetaType<void *>
{
};


} // namespace metapp


#endif

