#ifndef MEMBER_FUNCTION_H_969872685611
#define MEMBER_FUNCTION_H_969872685611

#include "metapp/metatypes/utils/callablebase.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...)>
	: public CallableBase <RT (Class::*)(Args...), Class, RT, Args...>
{
public:
	using UpType = TypeList <Class, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;

};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const> : public DeclareMetaTypeBase <RT (Class::*)(Args...)>
{
	static constexpr TypeFlags typeFlags = tfConst | DeclareMetaTypeBase <RT (Class::*)(Args...)>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile> : public DeclareMetaTypeBase <RT (Class::*)(Args...)>
{
	static constexpr TypeFlags typeFlags = tfVolatile | DeclareMetaTypeBase <RT (Class::*)(Args...)>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile> : public DeclareMetaTypeBase <RT (Class::*)(Args...)>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile | DeclareMetaTypeBase <RT (Class::*)(Args...)>::typeFlags;
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) noexcept> : public DeclareMetaTypeBase <RT (Class::*)(Args...)>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const noexcept> : public DeclareMetaTypeBase <RT (Class::*)(Args...) const>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile noexcept> : public DeclareMetaTypeBase <RT (Class::*)(Args...) volatile>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile noexcept> : public DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile>
{
};

#endif


} // namespace metapp


#endif

