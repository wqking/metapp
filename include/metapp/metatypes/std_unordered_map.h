#ifndef METAPP_STD_UNORDERED_MAP_H_969872685611
#define METAPP_STD_UNORDERED_MAP_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"
#include "metapp/interfaces/bases/metamapbase.h"

#include <unordered_map>

namespace metapp {

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct DeclareMetaTypeBase <std::unordered_map<Key, T, Hash, KeyEqual, Allocator> >
	: public MetaIterableBase <std::unordered_map<Key, T, Hash, KeyEqual, Allocator> >,
		public MetaMapBase <std::unordered_map<Key, T, Hash, KeyEqual, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdUnorderedMap;

};

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct DeclareMetaTypeBase <std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> >
	: public MetaIterableBase <std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> >,
		public MetaMapBase <std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdUnorderedMultimap;

};


} // namespace metapp


#endif

