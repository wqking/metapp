#ifndef TUPLE_H_969872685611
#define TUPLE_H_969872685611

#include "metapp/metatype.h"

#include <tuple>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::tuple<Types...> >
	: public DeclareObjectMetaType<std::tuple<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkTuple;

};


} // namespace metapp


#endif

