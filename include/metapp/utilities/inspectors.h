#ifndef INSPECTORS_H_969872685611
#define INSPECTORS_H_969872685611

#include "metapp/metatype.h"
#include "metapp/exception.h"

namespace metapp {

class CallableInspector
{
public:
	explicit CallableInspector(const MetaType * metaType)
		: metaType(metaType), classOffset(-1)
	{
		if(metaType->getMetaCallable() == nullptr) {
			errorWrongMetaType();
		}
		if(metaType->isClassMember()) {
			classOffset = 0;
		}
	}

	const MetaType * getClassType() const {
		if(classOffset < 0) {
			return nullptr;
		}
		return metaType->getUpType(classOffset);
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

private:
	const MetaType * metaType;
	int classOffset;
};

} // namespace metapp

#endif
