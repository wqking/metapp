#ifndef NULLPTR_H_969872685611
#define NULLPTR_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <>
struct DeclareMetaType <std::nullptr_t> : public DeclareMetaType<void *>
{
};


} // namespace varpp


#endif

