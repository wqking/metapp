#ifndef METAPP_ENUM_H_969872685611
#define METAPP_ENUM_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaenum.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T, typename std::enable_if<std::is_enum<T>::value>::type>
	: public DeclareMetaTypeBase <typename std::underlying_type<T>::type>
{
	using UpType = typename std::underlying_type<T>::type;
	static constexpr TypeKind typeKind = tkEnum;
};


} // namespace metapp


#endif

