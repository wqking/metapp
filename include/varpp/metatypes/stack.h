#ifndef STACK_H_969872685611
#define STACK_H_969872685611

#include "varpp/metatype.h"

#include <stack>

namespace varpp {

template <typename T, typename Container>
struct DeclareMetaType <std::stack<T, Container> > : public DeclareObjectMetaType<std::stack<T, Container> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkStack;

};


} // namespace varpp


#endif

