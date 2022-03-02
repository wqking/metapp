#ifndef METAPP_STD_TUPLE_H_969872685611
#define METAPP_STD_TUPLE_H_969872685611

#include "metapp/metatype.h"

#include <tuple>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::tuple<Types...> >
	: public DeclareMetaTypeObject <std::tuple<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdTuple;

};


} // namespace metapp


#endif

