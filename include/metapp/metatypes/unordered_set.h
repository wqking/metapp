#ifndef UNORDERED_SET_H_969872685611
#define UNORDERED_SET_H_969872685611

#include "metapp/metatype.h"

#include <unordered_set>

namespace metapp {

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct BaseDeclareMetaType <std::unordered_set<Key, Hash, KeyEqual, Allocator> >
	: public DeclareObjectMetaType<std::unordered_set<Key, Hash, KeyEqual, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkUnorderedSet;

};

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct BaseDeclareMetaType <std::unordered_multiset<Key, Hash, KeyEqual, Allocator> >
	: public DeclareObjectMetaType<std::unordered_multiset<Key, Hash, KeyEqual, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkUnorderedMultiset;

};


} // namespace metapp


#endif

