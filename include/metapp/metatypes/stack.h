#ifndef STACK_H_969872685611
#define STACK_H_969872685611

#include "metapp/metatype.h"

#include <stack>

namespace metapp {

template <typename T, typename Container>
struct DeclareMetaType <std::stack<T, Container> >
	: public DeclareMetaTypeBase <std::stack<T, Container> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStack;

};


} // namespace metapp


#endif

