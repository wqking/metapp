#ifndef LIST_H_969872685611
#define LIST_H_969872685611

#include "metapp/metatype.h"

#include <list>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::list<T, Allocator> >
	: public DeclareObjectMetaType<std::list<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkList;

};


} // namespace metapp


#endif

