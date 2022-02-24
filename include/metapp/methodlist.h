#ifndef METHODLIST_H_969872685611
#define METHODLIST_H_969872685611

#include "metapp/variant.h"
#include "metapp/metacallable.h"

#include <vector>

namespace metapp {

class MethodList
{
public:
	void addMethod(const Variant & method) {
		methodList.push_back(method);
	}

	size_t getCount() const {
		return methodList.size();
	}

	const Variant & get(const size_t index) const {
		return methodList[index];
	}

	const Variant * findCallable(const Variant * arguments, const size_t argumentCount) const {
		int maxRank = 0;
		const Variant * callable = nullptr;
		for(const Variant & method : methodList) {
			const int rank = method.getMetaType()->getMetaCallable()->rankInvoke(arguments, argumentCount);
			if(rank > maxRank) {
				maxRank = rank;
				callable = &method;
			}
		}
		return callable;
	}

private:
	std::vector<Variant> methodList;
};


} // namespace metapp

#endif
