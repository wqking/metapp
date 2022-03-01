#ifndef MEMBER_DATA_H_969872685611
#define MEMBER_DATA_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metaaccessible.h"

namespace metapp {

template <typename Class, typename T>
struct DeclareMetaTypeBase <T Class::*, typename std::enable_if<! std::is_function<T>::value>::type>
	: public DeclareMetaTypeRoot<T Class::*>
{
	using UpType = TypeList <Class, T>;
	static constexpr TypeKind typeKind = tkMemberPointer;
	static constexpr TypeFlags typeFlags = DeclareMetaTypeRoot<T Class::*>::typeFlags
		| tfClassMember
	;

};


} // namespace metapp


#endif

