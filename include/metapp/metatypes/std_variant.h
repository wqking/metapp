#ifndef METAPP_STD_VARIANT_H_969872685611
#define METAPP_STD_VARIANT_H_969872685611

#include "metapp/metatype.h"

#include <variant>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::variant<Types...> >
	: public DeclareMetaTypeObject <std::variant<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdVariant;

};


} // namespace metapp


#endif

