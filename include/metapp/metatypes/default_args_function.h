#ifndef DEFAULT_ARGS_FUNCTION_H_969872685611
#define DEFAULT_ARGS_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/utility.h"

namespace metapp {

template <typename FT, size_t defaultArgsCount>
class DefaultArgsFunction
{
public:
	DefaultArgsFunction() : func() {
	}

	template <typename FT>
	explicit DefaultArgsFunction(FT func) : func(func) {
	}

	const Variant & getFunc() const {
		return func;
	}

private:
	Variant func;
};

template <typename FT, size_t defaultArgsCount>
struct DeclareMetaType <DefaultArgsFunction<FT, defaultArgsCount> >
	: public DeclareMetaTypeBase<DefaultArgsFunction<FT, defaultArgsCount> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = typename Underlying::FunctionType;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

public:
	using UpType = FT;
	static constexpr TypeKind typeKind = tkDefaultArgsFunction;

#if 0
	static size_t getParameterCount()
	{
		return TypeListCount<ArgumentTypeList>::value;
	}

	static int rankInvoke(const Variant * arguments, const size_t argumentCount)
	{
		return MetaFunctionInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool canInvoke(const Variant * arguments, const size_t argumentCount)
	{
		return MetaFunctionInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		FunctionType f = func.get<FunctionType &>();
		return MetaFunctionInvoker<C, RT, ArgumentTypeList>::invoke(instance, f, arguments, argumentCount);
	}
#endif

};


} // namespace metapp


#endif

