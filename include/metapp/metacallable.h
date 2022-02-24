#ifndef METACALLABLE_H_969872685611
#define METACALLABLE_H_969872685611

namespace metapp {

class Variant;

class MetaCallable
{
public:
	MetaCallable(
		size_t (*getParameterCount)(),
		int (*rankInvoke)(const Variant * arguments, const size_t argumentCount),
		bool (*canInvoke)(const Variant * arguments, const size_t argumentCount),
		Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	)
		:
			getParameterCount(getParameterCount),
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{}

	size_t (*getParameterCount)();
	int (*rankInvoke)(const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount);
private:
};


} // namespace metapp

#endif
