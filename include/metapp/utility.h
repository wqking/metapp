#ifndef UTILITY_H_969872685611
#define UTILITY_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"

#include <type_traits>

namespace metapp {

template <typename ...Args>
struct CallableInvoker;

template <typename Arg0, typename ...Args>
struct CallableInvoker <Arg0, Args...>
{
	static Variant invoke(const Variant & func, void * instance, Arg0 arg0, Args ...args)
	{
		Variant arguments[] = {
			arg0,
			args...
		};
		return func.getMetaType()->getMetaCallable()->invoke(func, instance, arguments, sizeof...(Args) + 1);
	}
};

template <>
struct CallableInvoker <>
{
	static Variant invoke(const Variant & func, void * instance)
	{
		return func.getMetaType()->getMetaCallable()->invoke(func, instance, nullptr, 0);
	}
};

template <typename ...Args>
inline Variant invokeCallable(const Variant & func, void * instance, Args ...args)
{
	return CallableInvoker<Args...>::invoke(func, instance, args...);
}

} // namespace metapp

#endif
