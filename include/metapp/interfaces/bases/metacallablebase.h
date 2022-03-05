#ifndef METAPP_METACALLABLEBASE_H_969872685611
#define METAPP_METACALLABLEBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/metatypes/utils/declareutil.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename FT, typename Class, typename RT, typename ...Args>
struct MetaCallableBase
{
public:
	using FunctionType = FT;
	using ClassType = Class;
	using ReturnType = RT;
	using ArgumentTypeList = TypeList<Args...>;
	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetParamCount,
			&metaCallableGetReturnType,
			&metaCallableGetParamType,
			&metaCallableRankInvoke,
			&metaCallableCanInvoke,
			&metaCallableInvoke
		);
		return &metaCallable;
	}

	static size_t metaCallableGetParamCount()
	{
		return argsCount;
	}

	static const MetaType * metaCallableGetReturnType()
	{
		return getMetaType<RT>();
	}

	static const MetaType * metaCallableGetParamType(const size_t index)
	{
		return getMetaTypeAt<Args...>(index);
	}

	static int metaCallableRankInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return 0;
		}
		return MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool metaCallableCanInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return false;
		}
		return MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant metaCallableInvoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			errorIllegalArgument();
			return Variant();
		}

		FunctionType f = func.get<FunctionType &>();
		return MetaCallableInvoker<Class, RT, ArgumentTypeList>::invoke(f, instance, arguments, argumentCount);
	}

};


} // namespace metapp


#endif

