#ifndef DEQUE_H_969872685611
#define DEQUE_H_969872685611

#include "metapp/metatype.h"

#include <deque>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::deque<T, Allocator> >
	: public DeclareObjectMetaType<std::deque<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkDeque;

};


} // namespace metapp


#endif

