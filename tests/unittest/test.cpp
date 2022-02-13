#include "test.h"

const metapp::MetaType * getUpTypeAt(const metapp::MetaType * metaType, size_t index)
{
	while(metaType != nullptr && index > 0) {
		metaType = metaType->getUpType();
		--index;
	}
	return metaType;
}

std::vector<metapp::TypeKind> getUpTypeTypeKinds(const metapp::MetaType * metaType)
{
	std::vector<metapp::TypeKind> result;
	result.reserve(8);
	while(metaType != nullptr) {
		result.push_back(metaType->getTypeKind());
		metaType = metaType->getUpType();
	}
	return result;
}

