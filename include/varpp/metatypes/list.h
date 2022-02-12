#ifndef LIST_H_969872685611
#define LIST_H_969872685611

#include "varpp/metatype.h"

#include <list>

namespace varpp {

template <typename T, typename Alloc>
struct DeclareMetaType <std::list<T, Alloc> > : public DeclareObjectMetaType<std::list<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkList;

};


} // namespace varpp


#endif

