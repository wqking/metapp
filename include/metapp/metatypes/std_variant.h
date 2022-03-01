#ifndef STD_VARIANT_H_969872685611
#define STD_VARIANT_H_969872685611

#include "metapp/metatype.h"

#include <variant>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::variant<Types...> >
	: public DeclareMetaTypeRoot <std::variant<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdVariant;

};


} // namespace metapp


#endif

