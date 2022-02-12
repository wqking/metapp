#ifndef QUEUE_H_969872685611
#define QUEUE_H_969872685611

#include "metapp/metatype.h"

#include <queue>

namespace metapp {

template <typename T, typename Container>
struct DeclareMetaType <std::queue<T, Container> > : public DeclareObjectMetaType<std::queue<T, Container> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkQueue;

};

template <typename T, typename Container>
struct DeclareMetaType <std::priority_queue<T, Container> > : public DeclareObjectMetaType<std::priority_queue<T, Container> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkPriorityQueue;

};


} // namespace metapp


#endif

