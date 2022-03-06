#ifndef METAPP_MEMBER_FUNCTION_H_969872685611
#define METAPP_MEMBER_FUNCTION_H_969872685611

#include "metapp/interfaces/bases/metacallablebase.h"
#include "metapp/interfaces/bases/metamemberbase.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename FullType, typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeMemberFunctionBase
	: public DeclareMetaTypeObject<FullType>,
		public MetaCallableBase<FullType, Class, RT, Args...>,
		public MetaMemberBase<Class>
{
public:
	using UpType = TypeList <Class, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;

};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...)>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...), Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...), Class, RT, Args...>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const, Class, RT, Args...>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfVolatile
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile, Class, RT, Args...>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile, Class, RT, Args...>::typeFlags;
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) noexcept, Class, RT, Args...>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const noexcept, Class, RT, Args...>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfVolatile
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile noexcept, Class, RT, Args...>::typeFlags;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile
		| DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile noexcept, Class, RT, Args...>::typeFlags;
};

#endif


} // namespace metapp


#endif

