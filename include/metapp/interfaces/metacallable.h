#ifndef METAPP_METACALLABLE_H_969872685611
#define METAPP_METACALLABLE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

class Variant;

class MetaCallable
{
public:
	MetaCallable(
		size_t (*getParamCount)(),
		const MetaType * (*getReturnType)(),
		const MetaType * (*getParamType)(const size_t index),
		int (*rankInvoke)(const Variant * arguments, const size_t argumentCount),
		bool (*canInvoke)(const Variant * arguments, const size_t argumentCount),
		Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	)
		:
			getParamCount(getParamCount),
			getReturnType(getReturnType),
			getParamType(getParamType),
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{
	}

	size_t (*getParamCount)();
	const MetaType * (*getReturnType)();
	const MetaType * (*getParamType)(const size_t index);

	int (*rankInvoke)(const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount);
};


} // namespace metapp

#endif
