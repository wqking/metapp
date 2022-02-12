#ifndef STRING_H_969872685611
#define STRING_H_969872685611

#include "varpp/metatype.h"

#include <string>

namespace varpp {

template <>
struct DeclareMetaType <std::string> : public DeclareObjectMetaType<std::string>
{
	static constexpr TypeKind typeKind = tkString;
};

template <>
struct DeclareMetaType <std::wstring> : public DeclareObjectMetaType<std::wstring>
{
	static constexpr TypeKind typeKind = tkWideString;
};


} // namespace varpp


#endif

