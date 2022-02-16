#ifndef ANY_H_969872685611
#define ANY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypeutil.h"

#include <any>

namespace metapp {

template <>
struct DeclareMetaType <std::any> : public DeclareMetaTypeBase <std::any>
{
	static constexpr TypeKind typeKind = tkAny;
};



} // namespace metapp


#endif

