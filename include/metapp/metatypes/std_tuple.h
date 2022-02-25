#ifndef STD_TUPLE_H_969872685611
#define STD_TUPLE_H_969872685611

#include "metapp/metatype.h"

#include <tuple>

namespace metapp {

template <typename ...Types>
struct DeclareMetaType <std::tuple<Types...> >
	: public DeclareMetaTypeBase <std::tuple<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdTuple;

};


} // namespace metapp


#endif

