#ifndef STRING_H_969872685611
#define STRING_H_969872685611

#include "metapp/metatype.h"

#include <string>

namespace metapp {

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


} // namespace metapp


#endif

