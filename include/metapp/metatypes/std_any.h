#ifndef STD_ANY_H_969872685611
#define STD_ANY_H_969872685611

#include "metapp/metatype.h"

#include <any>

namespace metapp {

template <>
struct DeclareMetaTypeBase <std::any>
	: public DeclareMetaTypeRoot <std::any>
{
	static constexpr TypeKind typeKind = tkStdAny;
};



} // namespace metapp


#endif

