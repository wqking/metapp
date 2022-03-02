#ifndef METAPP_STD_STACK_H_969872685611
#define METAPP_STD_STACK_H_969872685611

#include "metapp/metatype.h"

#include <stack>

namespace metapp {

template <typename T, typename Container>
struct DeclareMetaTypeBase <std::stack<T, Container> >
	: public DeclareMetaTypeObject <std::stack<T, Container> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdStack;

};


} // namespace metapp


#endif

