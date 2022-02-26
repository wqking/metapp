#ifndef STD_DEQUE_H_969872685611
#define STD_DEQUE_H_969872685611

#include "metapp/metatype.h"

#include <deque>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaType <std::deque<T, Allocator> >
	: public DeclareMetaTypeBase <std::deque<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdDeque;

};


} // namespace metapp


#endif
