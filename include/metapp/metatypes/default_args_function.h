#ifndef DEFAULT_ARGS_FUNCTION_H_969872685611
#define DEFAULT_ARGS_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/metacallable.h"
#include "metapp/utility.h"

#include <vector>
#include <initializer_list>

namespace metapp {

template <typename FT, size_t defaultArgsCount>
class DefaultArgsFunction
{
public:
	DefaultArgsFunction() : func() {
	}

	DefaultArgsFunction(FT func, const std::initializer_list<Variant> & defaultArgs_)
		: func(func), defaultArgs(defaultArgs_.begin(), defaultArgs_.end()) {
	}

	const Variant & getFunc() const {
		return func;
	}

	const Variant * getDefaultArgs() const {
		return defaultArgs.data();
	}

private:
	Variant func;
	// If we use std::array here, initial it from the std::initializer_list
	// will cause the first Variant wrongly captures the std::initializer_list
	// as tkObject
	std::vector<Variant> defaultArgs;
};

namespace internal_ {

struct DefaultArgsGetter
{
	size_t argumentCount;
	const Variant * passedArguments;
	size_t passedArgumentCount;
	const Variant * defaultArguments;

	const Variant & operator[] (const size_t index) const {
		if(index < passedArgumentCount) {
			return passedArguments[index];
		}
		else {
			return defaultArguments[(argumentCount - index) - 1];
		}
	}
};

} // namespace internal_

template <typename FT, size_t defaultArgsCount>
struct DeclareMetaType <DefaultArgsFunction<FT, defaultArgsCount> >
	: public DeclareMetaTypeBase<DefaultArgsFunction<FT, defaultArgsCount> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = DefaultArgsFunction<FT, defaultArgsCount>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	using UpType = FT;
	static constexpr TypeKind typeKind = tkDefaultArgsFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&getParameterCount,
			&rankInvoke,
			&canInvoke,
			&invoke
		);
		return &metaCallable;
	}

	static bool isValidArgumentCount(const size_t argumentCount)
	{
		return argumentCount >= argsCount - defaultArgsCount && argumentCount <= argsCount;
	}

	static size_t getParameterCount()
	{
		return argsCount;
	}

	static int rankInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(argumentCount)) {
			return 0;
		}

		return MetaFunctionInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool canInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(argumentCount)) {
			return false;
		}

		return MetaFunctionInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		if(! isValidArgumentCount(argumentCount)) {
			errorIllegalArgument();
		}

		const FunctionType & defaultArgsFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = defaultArgsFunc.getFunc();
		if(argumentCount == argsCount) {
			return underlyingFunc.getMetaType()->getMetaCallable()->invoke(instance, underlyingFunc, arguments, argumentCount);
		}
		else {
			std::array<Variant, argsCount> newArguments;
			size_t i = 0;
			for(; i < argumentCount; ++i) {
				newArguments[i] = arguments[i];
			}
			while(i < argsCount) {
				newArguments[i] = defaultArgsFunc.getDefaultArgs()[argsCount - i - 1];
				++i;
			}
			return underlyingFunc.getMetaType()->getMetaCallable()->invoke(instance, underlyingFunc, newArguments.data(), argsCount);
		}
	}

};


} // namespace metapp


#endif

