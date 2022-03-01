#ifndef STD_UNORDERED_SET_H_969872685611
#define STD_UNORDERED_SET_H_969872685611

#include "metapp/metatype.h"

#include <unordered_set>

namespace metapp {

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct DeclareMetaTypeBase <std::unordered_set<Key, Hash, KeyEqual, Allocator> >
	: public DeclareMetaTypeObject <std::unordered_set<Key, Hash, KeyEqual, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdUnorderedSet;

};

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct DeclareMetaTypeBase <std::unordered_multiset<Key, Hash, KeyEqual, Allocator> >
	: public DeclareMetaTypeObject <std::unordered_multiset<Key, Hash, KeyEqual, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdUnorderedMultiset;

};


} // namespace metapp


#endif

