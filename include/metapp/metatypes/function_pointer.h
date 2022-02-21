#ifndef FUNCTION_POINTER_H_969872685611
#define FUNCTION_POINTER_H_969872685611

#include "metapp/metatypes/utils/callablebase.h"

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (*)(Args...)>
	: public CallableBase<RT (*)(Args...), void, RT, Args...>
{
public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkFunction;

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

