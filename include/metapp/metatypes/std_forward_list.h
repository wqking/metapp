#ifndef STD_FORWARD_LIST_H_969872685611
#define STD_FORWARD_LIST_H_969872685611

#include "metapp/metatype.h"

#include <forward_list>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::forward_list<T, Allocator> >
	: public DeclareMetaTypeRoot <std::forward_list<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdForwardList;

};


} // namespace metapp


#endif

