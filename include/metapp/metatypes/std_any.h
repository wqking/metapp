#ifndef STD_ANY_H_969872685611
#define STD_ANY_H_969872685611

#include "metapp/metatype.h"

#include <any>

namespace metapp {

template <>
struct DeclareMetaType <std::any> : public DeclareMetaTypeBase <std::any>
{
	static constexpr TypeKind typeKind = tkStdAny;
};



} // namespace metapp


#endif

