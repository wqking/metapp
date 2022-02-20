#ifndef FUNCTION_H_969872685611
#define FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/utility.h"

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (*)(Args...)>
	: public DeclareMetaTypeBase<RT (*)(Args...)>
{
private:
	using FunctionType = RT (*)(Args...);

public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkFunction;

	static bool canInvoke(const Variant * arguments, const size_t argumentCount)
	{
		return MetaFunctionInvokeChecker<Args...>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		FunctionType f = func.get<FunctionType>();
		return MetaFunctionInvoker<void, RT, Args...>::invoke(instance, f, arguments, argumentCount);
	}

};

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (*)(Args...) noexcept>
	: public DeclareMetaType <RT (*)(Args...)>
{
};

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (Args...)>
	: public DeclareMetaType <RT (*)(Args...)>
{
};

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (Args...) noexcept>
	: public DeclareMetaType <RT (Args...)>
{
};


} // namespace metapp


#endif

