#ifndef METAPP_STD_SET_H_969872685611
#define METAPP_STD_SET_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/iterablebase.h"

#include <set>

namespace metapp {

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::set<Key, Compare, Allocator> >
	: public DeclareMetaTypeObject <std::set<Key, Compare, Allocator> >,
		public IterableBase<std::set<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdSet;

};

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multiset<Key, Compare, Allocator> >
	: public DeclareMetaTypeObject <std::multiset<Key, Compare, Allocator> >,
		public IterableBase<std::multiset<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdMultiset;

};


} // namespace metapp


#endif

