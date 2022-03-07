#ifndef METAPP_STD_SET_H_969872685611
#define METAPP_STD_SET_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"

#include <set>

namespace metapp {

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::set<Key, Compare, Allocator> >
	: public MetaIterableBase<std::set<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdSet;

};

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multiset<Key, Compare, Allocator> >
	: public MetaIterableBase<std::multiset<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdMultiset;

};


} // namespace metapp


#endif

