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
	static constexpr TypeFlags typeFlags = tfCallable | DeclareMetaTypeBase<RT (*)(Args...)>::typeFlags;

	static bool canInvoke(const Variant * arguments)
	{
		return MetaFunctionInvokeChecker<Args...>::canInvoke(arguments);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments)
	{
		FunctionType f = func.get<FunctionType>();
		return MetaFunctionInvoker<void, RT, Args...>::invoke(instance, f, arguments);
	}

};

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (Args...)>
	: public DeclareMetaType <RT (*)(Args...)>
{
};

} // namespace metapp


#endif

