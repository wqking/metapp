#ifndef METAPP_STD_MAP_H_969872685611
#define METAPP_STD_MAP_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"
#include "metapp/interfaces/bases/metamapbase.h"

#include <map>

namespace metapp {

template <typename Key, typename T, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::map<Key, T, Compare, Allocator> >
	: public MetaIterableBase<std::map<Key, T, Compare, Allocator> >,
		public MetaMapBase<std::map<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdMap;

};

template <typename Key, typename T, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multimap<Key, T, Compare, Allocator> >
	: public MetaIterableBase<std::multimap<Key, T, Compare, Allocator> >,
		public MetaMapBase<std::multimap<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdMultimap;

};


} // namespace metapp


#endif

