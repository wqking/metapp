#ifndef STD_QUEUE_H_969872685611
#define STD_QUEUE_H_969872685611

#include "metapp/metatype.h"

#include <queue>

namespace metapp {

template <typename T, typename Container>
struct DeclareMetaType <std::queue<T, Container> >
	: public DeclareMetaTypeBase <std::queue<T, Container> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdQueue;

};

template <typename T, typename Container>
struct DeclareMetaType <std::priority_queue<T, Container> >
	: public DeclareMetaTypeBase <std::priority_queue<T, Container> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdPriorityQueue;

};


} // namespace metapp


#endif

