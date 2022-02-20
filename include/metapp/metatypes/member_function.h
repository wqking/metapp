#ifndef MEMBER_FUNCTION_H_969872685611
#define MEMBER_FUNCTION_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename C, typename RT, typename ...Args>
struct DeclareMetaType <RT (C::*)(Args...)>
	: public DeclareMetaTypeBase <RT (C::*)(Args...)>
{
private:
	using FunctionType = RT (C::*)(Args...);

public:
	using UpType = TypeList <C, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;

	static bool canInvoke(const Variant * arguments, const size_t argumentCount)
	{
		return MetaFunctionInvokeChecker<Args...>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		FunctionType f = func.get<FunctionType>();
		return MetaFunctionInvoker<C, RT, Args...>::invoke(instance, f, arguments, argumentCount);
	}

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


} // namespace metapp


#endif

