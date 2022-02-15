#ifndef MAP_H_969872685611
#define MAP_H_969872685611

#include "metapp/metatype.h"

#include <map>

namespace metapp {

template <typename Key, typename T, typename Compare, typename Allocator>
struct BaseDeclareMetaType <std::map<Key, T, Compare, Allocator> >
	: public DeclareObjectMetaType<std::map<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkMap;

};

template <typename Key, typename T, typename Compare, typename Allocator>
struct BaseDeclareMetaType <std::multimap<Key, T, Compare, Allocator> >
	: public DeclareObjectMetaType<std::multimap<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkMultimap;

};


} // namespace metapp


#endif

