#ifndef FORWARD_LIST_H_969872685611
#define FORWARD_LIST_H_969872685611

#include "metapp/metatype.h"

#include <forward_list>

namespace metapp {

template <typename T, typename Allocator>
struct BaseDeclareMetaType <std::forward_list<T, Allocator> >
	: public DeclareObjectMetaType<std::forward_list<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkForwardList;

};


} // namespace metapp


#endif

