#ifndef METAPP_MEMBER_DATA_H_969872685611
#define METAPP_MEMBER_DATA_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metaaccessible.h"

namespace metapp {

template <typename Class, typename T>
struct DeclareMetaTypeBase <T Class::*, typename std::enable_if<! std::is_function<T>::value>::type>
	: public DeclareMetaTypeObject<T Class::*>
{
	using UpType = TypeList <Class, T>;
	static constexpr TypeKind typeKind = tkMemberPointer;
	static constexpr TypeFlags typeFlags = DeclareMetaTypeObject<T Class::*>::typeFlags
		| tfClassMember
	;

};


} // namespace metapp


#endif

