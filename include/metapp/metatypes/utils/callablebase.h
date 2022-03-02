#ifndef METAPP_CALLABLEBASE_FUNCTION_H_969872685611
#define METAPP_CALLABLEBASE_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename FT, typename Class, typename RT, typename ...Args>
struct CallableBase
	: public DeclareMetaTypeObject<FT>
{
public:
	using FunctionType = FT;
	using ClassType = Class;
	using ReturnType = RT;
	using ArgumentTypeList = TypeList<Args...>;
	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

	static constexpr TypeFlags typeFlags = DeclareMetaTypeObject<FT>::typeFlags
		| (std::is_void<Class>::value ? 0 : tfClassMember)
	;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&rankInvoke,
			&canInvoke,
			&invoke
		);
		return &metaCallable;
	}

	static int rankInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return 0;
		}
		return MetaCallableInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool canInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return false;
		}
		return MetaCallableInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			errorIllegalArgument();
		}

		FunctionType f = func.get<FunctionType &>();
		return MetaCallableInvoker<Class, RT, ArgumentTypeList>::invoke(f, instance, arguments, argumentCount);
	}

};


} // namespace metapp


#endif

