#ifndef MEMBER_H_969872685611
#define MEMBER_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...)>
	: public BaseDeclareMetaType <RT (C::*)(Args...)>
{
	using UpType = TypeList <C, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;
};

template <typename C, typename T>
struct DeclareMetaType <T C::*>
	: public BaseDeclareMetaType <T C::*>
{
	using UpType = TypeList <C, T>;
	static constexpr TypeKind typeKind = tkMemberPointer;
};

} // namespace metapp


#endif

