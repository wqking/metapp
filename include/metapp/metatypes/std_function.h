#ifndef STD_FUNCTION_H_969872685611
#define STD_FUNCTION_H_969872685611

#include "metapp/metatype.h"

#include <functional>

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaType <std::function<RT (Args...)> >
	: public DeclareMetaTypeBase <std::function<RT (Args...)> >
{
private:
	using FunctionType = std::function<RT (Args...)>;

public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkStdFunction;
	static constexpr TypeFlags typeFlags = tfCallable | DeclareMetaTypeBase<FunctionType>::typeFlags;

	static bool canInvoke(const Variant * arguments)
	{
		return MetaFunctionInvokeChecker<Args...>::canInvoke(arguments);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments)
	{
		auto & f = func.get<FunctionType &>();
		return MetaFunctionInvoker<void, RT, Args...>::invoke(instance, f, arguments);
	}

};


} // namespace metapp


#endif

