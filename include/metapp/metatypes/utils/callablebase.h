#ifndef CALLABLEBASE_FUNCTION_H_969872685611
#define CALLABLEBASE_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/utility.h"

namespace metapp {

template <typename FT, typename C, typename RT, typename ...Args>
struct CallableBase
	: public DeclareMetaTypeBase <FT>
{
private:
	using FunctionType = FT;

public:
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
		FunctionType f = func.get<FunctionType &>();
		return MetaFunctionInvoker<C, RT, Args...>::invoke(instance, f, arguments, argumentCount);
	}

};


} // namespace metapp


#endif

