#ifndef MEMBER_FUNCTION_H_969872685611
#define MEMBER_FUNCTION_H_969872685611

#include "metapp/metatypes/utils/callablebase.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...)>
	: public CallableBase <RT (C::*)(Args...), C, RT, Args...>
{
public:
	using UpType = TypeList <C, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;

};

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) const> : public DeclareMetaType <RT (C::*)(Args...)>
{
	static constexpr TypeFlags typeFlags = tfConst | DeclareMetaType <RT (C::*)(Args...)>::typeFlags;
};

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) volatile> : public DeclareMetaType <RT (C::*)(Args...)>
{
	static constexpr TypeFlags typeFlags = tfVolatile | DeclareMetaType <RT (C::*)(Args...)>::typeFlags;
};

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) const volatile> : public DeclareMetaType <RT (C::*)(Args...)>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile | DeclareMetaType <RT (C::*)(Args...)>::typeFlags;
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) noexcept> : public DeclareMetaType <RT (C::*)(Args...)>
{
};

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) const noexcept> : public DeclareMetaType <RT (C::*)(Args...) const>
{
};

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) volatile noexcept> : public DeclareMetaType <RT (C::*)(Args...) volatile>
{
};

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...) const volatile noexcept> : public DeclareMetaType <RT (C::*)(Args...) const volatile>
{
};

#endif


} // namespace metapp


#endif

