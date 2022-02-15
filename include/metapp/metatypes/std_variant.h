#ifndef TUPLE_H_969872685611
#define TUPLE_H_969872685611

#include "metapp/metatype.h"

#include <variant>

namespace metapp {

template <typename ...Types>
struct BaseDeclareMetaType <std::variant<Types...> >
	: public DeclareObjectMetaType<std::variant<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdVariant;

};


} // namespace metapp


#endif

