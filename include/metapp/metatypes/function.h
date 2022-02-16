#ifndef FUNCTION_H_969872685611
#define FUNCTION_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (*)(Args...)>
	: public DeclareMetaTypeBase<RT (*)(Args...)>
{
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkFunction;

};

template <typename RT, typename ...Args>
struct DeclareMetaType <RT (Args...)>
	: public DeclareMetaType <RT (*)(Args...)>
{
};

} // namespace metapp


#endif

