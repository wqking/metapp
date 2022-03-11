#ifndef TEST_H
#define TEST_H

#include "metapp/metatype.h"
#include "metapp/typekind.h"
#include "metapp/utils/typelist.h"

#include "../catch.hpp"

#include <vector>
#include <unordered_set>
#include <tuple>
#include <initializer_list>

const metapp::MetaType * getUpTypeAt(const metapp::MetaType * metaType, size_t index);
std::vector<metapp::TypeKind> getUpTypeTypeKinds(const metapp::MetaType * metaType);

template <typename Iterator>
inline bool matchUpTypeKinds(const metapp::MetaType * metaType, Iterator begin, Iterator end)
{
	while(begin != end) {
		if(metaType == nullptr) {
			return false;
		}
		if(metaType->getTypeKind() != *begin) {
			return false;
		}
		metaType = metaType->getUpType();
		++begin;
	}
	return true;
}

template <typename T>
inline bool matchUpTypeKinds(const metapp::MetaType * metaType, const std::initializer_list<T> & typeKindList)
{
	return matchUpTypeKinds(metaType, std::begin(typeKindList), std::end(typeKindList));
}

template <typename T, typename U>
inline bool matchUpTypeKinds(const metapp::MetaType * metaType, const U & typeKindList)
{
	return matchUpTypeKinds(metaType, std::begin(typeKindList), std::end(typeKindList));
}


#endif
