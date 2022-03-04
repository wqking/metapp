#ifndef METAPP_STD_FUNCTION_H_969872685611
#define METAPP_STD_FUNCTION_H_969872685611

#include "metapp/metatypes/utils/metacallablebase.h"

#include <functional>

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <std::function<RT (Args...)> >
	: public DeclareMetaTypeObject<std::function<RT (Args...)> >,
		public MetaCallableBase <std::function<RT (Args...)>, void, RT, Args...>
{
public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkStdFunction;

};


} // namespace metapp


#endif

