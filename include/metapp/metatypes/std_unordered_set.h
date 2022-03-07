#ifndef METAPP_STD_UNORDERED_SET_H_969872685611
#define METAPP_STD_UNORDERED_SET_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"

#include <unordered_set>

namespace metapp {

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct DeclareMetaTypeBase <std::unordered_set<Key, Hash, KeyEqual, Allocator> >
	: public MetaIterableBase <std::unordered_set<Key, Hash, KeyEqual, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdUnorderedSet;

};

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct DeclareMetaTypeBase <std::unordered_multiset<Key, Hash, KeyEqual, Allocator> >
	: public MetaIterableBase <std::unordered_multiset<Key, Hash, KeyEqual, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdUnorderedMultiset;

};


} // namespace metapp


#endif

