#ifndef METAPP_VARIADIC_FUNCTION_H_969872685611
#define METAPP_VARIADIC_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/metacallable.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename FT>
class VariadicFunction
{
public:
	VariadicFunction() : func() {
	}

	explicit VariadicFunction(FT func)
		: func(func) {
	}

	const Variant & getFunc() const {
		return func;
	}

private:
	Variant func;
};

template <typename FT>
auto createVariadicFunction(FT && func) -> VariadicFunction<FT>
{
	return VariadicFunction<FT>(std::forward<FT>(func));
}

namespace internal_ {

} // namespace internal_

template <typename FT>
struct DeclareMetaType <VariadicFunction<FT> >
	: public DeclareMetaTypeBase<VariadicFunction<FT> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = VariadicFunction<FT>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	using UpType = FT;
	static constexpr TypeKind typeKind = tkVariadicFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&rankInvoke,
			&canInvoke,
			&invoke
		);
		return &metaCallable;
	}

	static int rankInvoke(const Variant * /*arguments*/, const size_t /*argumentCount*/)
	{
		return 1;
	}

	static bool canInvoke(const Variant * /*arguments*/, const size_t /*argumentCount*/)
	{
		return true;
	}

	static Variant invoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		Variant newArguments[2] = { arguments, argumentCount };

		const FunctionType & variadicFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = variadicFunc.getFunc();
		return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, newArguments, 2);
	}

};


} // namespace metapp


#endif

