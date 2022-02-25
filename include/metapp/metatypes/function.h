#ifndef FUNCTION_H_969872685611
#define FUNCTION_H_969872685611

#include "metapp/metatypes/utils/callablebase.h"
#include "metapp/compiler.h"

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
struct DeclareMetaType <RT (Args...)>
	: public DeclareMetaType <RT (*)(Args...)>
{
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (*)(Args...) noexcept>
	: public DeclareMetaType <RT (*)(Args...)>
{
};

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (Args...) noexcept>
	: public DeclareMetaType <RT (Args...)>
{
};

#endif


} // namespace metapp


#endif

