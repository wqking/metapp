// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef METAPP_STD_MAP_H_969872685611
#define METAPP_STD_MAP_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"
#include "metapp/interfaces/bases/metamappablebase.h"

#include <map>

namespace metapp {

template <typename Key, typename T, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::map<Key, T, Compare, Allocator> >
	: MetaIterableBase<std::map<Key, T, Compare, Allocator> >,
		MetaMappableBase<std::map<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdMap;

};

template <typename Key, typename T, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multimap<Key, T, Compare, Allocator> >
	: MetaIterableBase<std::multimap<Key, T, Compare, Allocator> >,
		MetaMappableBase<std::multimap<Key, T, Compare, Allocator> >
{
	using UpType = TypeList<Key, T>;
	static constexpr TypeKind typeKind = tkStdMultimap;

};


} // namespace metapp


#endif

