#ifndef METAPP_STD_MAP_H_969872685611
#define METAPP_STD_MAP_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/iterablebase.h"

#include <map>

namespace metapp {

template <typename Key, typename T, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::map<Key, T, Compare, Allocator> >
	: public DeclareMetaTypeObject <std::map<Key, T, Compare, Allocator> >,
		public IterableBase<std::map<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdMap;

};

template <typename Key, typename T, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multimap<Key, T, Compare, Allocator> >
	: public DeclareMetaTypeObject <std::multimap<Key, T, Compare, Allocator> >,
		public IterableBase<std::multimap<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdMultimap;

};


} // namespace metapp


#endif

