#ifndef METAPP_FUNCTION_H_969872685611
#define METAPP_FUNCTION_H_969872685611

#include "metapp/metatypes/utils/callablebase.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (*)(Args...)>
	: public CallableBase<RT (*)(Args...), void, RT, Args...>
{
public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkFunction;

};

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Args...)>
	: public DeclareMetaTypeBase <RT (*)(Args...)>
{
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (*)(Args...) noexcept>
	: public DeclareMetaTypeBase <RT (*)(Args...)>
{
};

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Args...) noexcept>
	: public DeclareMetaTypeBase <RT (Args...)>
{
};

#endif


} // namespace metapp


#endif

