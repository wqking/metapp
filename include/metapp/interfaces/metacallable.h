#ifndef METAPP_METACALLABLE_H_969872685611
#define METAPP_METACALLABLE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

class Variant;

class MetaCallable
{
public:
	MetaCallable(
		const MetaType * metaType,
		int (*rankInvoke)(const Variant * arguments, const size_t argumentCount),
		bool (*canInvoke)(const Variant * arguments, const size_t argumentCount),
		Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	)
		:
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke),
			metaType(metaType),
			classOffset(metaType->isClassMember() ? 0 : -1)
	{
	}

	size_t getParamCount() const {
		return metaType->getUpTypeCount() - 1 - (classOffset + 1);
	}

	const MetaType * getReturnType() const {
		return metaType->getUpType(classOffset + 1);
	}

	const MetaType * getParamType(const size_t index) const {
		return metaType->getUpType(classOffset + 2 + index);
	}

	int (*rankInvoke)(const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount);

private:
	const MetaType * metaType;
	int classOffset;
};


} // namespace metapp

#endif
