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

#ifndef METAPP_STD_SET_H_969872685611
#define METAPP_STD_SET_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"

#include <set>

namespace metapp {

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::set<Key, Compare, Allocator> >
	: MetaIterableBase<std::set<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdSet;

};

template <typename Key, typename Compare, typename Allocator>
struct DeclareMetaTypeBase <std::multiset<Key, Compare, Allocator> >
	: MetaIterableBase<std::multiset<Key, Compare, Allocator> >
{
	using UpType = Key;
	static constexpr TypeKind typeKind = tkStdMultiset;

};


} // namespace metapp


#endif

