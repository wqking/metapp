#ifndef STD_FUNCTION_H_969872685611
#define STD_FUNCTION_H_969872685611

#include "metapp/metatypes/utils/callablebase.h"

#include <functional>

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <std::function<RT (Args...)> >
	: public CallableBase <std::function<RT (Args...)>, void, RT, Args...>
{
public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkStdFunction;

};


} // namespace metapp


#endif

