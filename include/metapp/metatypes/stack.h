#ifndef STACK_H_969872685611
#define STACK_H_969872685611

#include "metapp/metatype.h"

#include <stack>

namespace metapp {

template <typename T, typename Container>
struct BaseDeclareMetaType <std::stack<T, Container> > : public DeclareObjectMetaType<std::stack<T, Container> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkStack;

};


} // namespace metapp


#endif

