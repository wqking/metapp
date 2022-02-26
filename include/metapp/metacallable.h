#ifndef METACALLABLE_H_969872685611
#define METACALLABLE_H_969872685611

namespace metapp {

class Variant;

class MetaCallable
{
public:
	MetaCallable(
		int (*rankInvoke)(const Variant * arguments, const size_t argumentCount),
		bool (*canInvoke)(const Variant * arguments, const size_t argumentCount),
		Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	)
		:
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{}

	int (*rankInvoke)(const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount);
};


} // namespace metapp

#endif
