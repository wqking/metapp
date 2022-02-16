#ifndef SET_H_969872685611
#define SET_H_969872685611

#include "metapp/metatype.h"

#include <set>

namespace metapp {

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::set<Key, Compare, Allocator> >
	: public DeclareObjectMetaType<std::set<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkSet;

};

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multiset<Key, Compare, Allocator> >
	: public DeclareObjectMetaType<std::multiset<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkMultiset;

};


} // namespace metapp


#endif

