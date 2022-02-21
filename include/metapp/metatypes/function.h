#ifndef FUNCTION_H_969872685611
#define FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/utility.h"

namespace metapp {

class Function
{
public:
	Function() : func() {
	}

	template <typename FT>
	explicit Function(FT func) : func(func) {
	}

	const Variant & getFunc() const {
		return func;
	}

private:
	Variant func;
};

#if 0
template <typename RT, typename ...Args>
struct DeclareMetaType <RT (*)(Args...)>
	: public DeclareMetaTypeBase<RT (*)(Args...)>
{
private:
	using FunctionType = RT (*)(Args...);

public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkFunction;

	static size_t getParameterCount(const Variant & /*func*/)
	{
		return sizeof...(Args);
	}

	static int rankInvoke(const Variant & /*func*/, const Variant * arguments, const size_t argumentCount)
	{
		return MetaFunctionInvokeChecker<Args...>::rankInvoke(arguments, argumentCount);
	}

	static bool canInvoke(const Variant & /*func*/, const Variant * arguments, const size_t argumentCount)
	{
		return MetaFunctionInvokeChecker<Args...>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		FunctionType f = func.get<FunctionType>();
		return MetaFunctionInvoker<void, RT, Args...>::invoke(instance, f, arguments, argumentCount);
	}

};

#endif


} // namespace metapp


#endif

