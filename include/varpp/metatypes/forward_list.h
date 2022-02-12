#ifndef FORWARD_LIST_H_969872685611
#define FORWARD_LIST_H_969872685611

#include "varpp/metatype.h"

#include <forward_list>

namespace varpp {

template <typename T, typename Alloc>
struct DeclareMetaType <std::forward_list<T, Alloc> > : public DeclareObjectMetaType<std::forward_list<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkForwardList;

};


} // namespace varpp


#endif

