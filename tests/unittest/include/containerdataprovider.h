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

#ifndef CONTAINERDATAPROVIDER_H
#define CONTAINERDATAPROVIDER_H

#include "metapp/compiler.h"
#include "metapp/metatype.h"
#include "metapp/typekind.h"
#include "metapp/utilities/typelist.h"

#include <initializer_list>
#include <array>
#include <deque>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#ifdef METAPP_COMPILER_VC
#pragma warning(push)
#pragma warning(disable: 4310) // cast truncates constant value
#endif

template <typename Container>
class TestContainerDataProviderBase
{
private:
	using ValueType = typename Container::value_type;

public:
	TestContainerDataProviderBase(
			const metapp::TypeKind typeKind,
			const metapp::TypeKind valueTypeKind,
			const Container & container
		)
		: typeKind(typeKind), valueTypeKind(valueTypeKind), container(container)
	{
	}

	metapp::TypeKind getTypeKind() const {
		return typeKind;
	}

	metapp::TypeKind getValueTypeKind() const {
		return valueTypeKind;
	}

	const Container & getContainer() const {
		return container;
	}

	Container & getContainer() {
		return container;
	}

private:
	metapp::TypeKind typeKind;
	metapp::TypeKind valueTypeKind;
	Container container;
};

template <typename T>
class TestContainerDataProvider;

#define _EXPAND(...) __VA_ARGS__
#define EXPAND(a) _EXPAND a

#define MAKE_CONTAINER_DATA_PROVIDER(type, typeKind, valueTypeKind, values) \
	template <>	class TestContainerDataProvider<EXPAND(type) > \
		: public TestContainerDataProviderBase<EXPAND(type) > \
	{ \
	private: \
		using super = TestContainerDataProviderBase<EXPAND(type) >; \
	public: \
		TestContainerDataProvider() \
			: super( typeKind, valueTypeKind, EXPAND(values) )\
		{} \
	};

MAKE_CONTAINER_DATA_PROVIDER((std::array<int, 5>), metapp::tkStdArray, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::array<std::string, 5>), metapp::tkStdArray, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::vector<int>), metapp::tkStdVector, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::vector<std::string>), metapp::tkStdVector, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::deque<int>), metapp::tkStdDeque, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::deque<std::string>), metapp::tkStdDeque, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::list<int>), metapp::tkStdDeque, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::list<std::string>), metapp::tkStdDeque, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::forward_list<int>), metapp::tkStdDeque, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::forward_list<std::string>), metapp::tkStdDeque, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::set<int>), metapp::tkStdSet, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::set<std::string>), metapp::tkStdSet, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::unordered_set<int>), metapp::tkStdUnorderedSet, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::unordered_set<std::string>), metapp::tkStdUnorderedSet, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::multiset<int>), metapp::tkStdMultiset, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::multiset<std::string>), metapp::tkStdMultiset, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::unordered_multiset<int>), metapp::tkStdUnorderedMultiset, metapp::tkInt,
	({ -5, 0, 123456789, 38, -98765 }))
MAKE_CONTAINER_DATA_PROVIDER((std::unordered_multiset<std::string>), metapp::tkStdUnorderedMultiset, metapp::tkStdString,
	({ "hello", "", "world", "This is great!", "Very good" }))

MAKE_CONTAINER_DATA_PROVIDER((std::map<std::string, int>), metapp::tkStdMap, metapp::tkStdPair,
	({ { "hello", -5 }, { "", 0 }, { "world", 123456789 }, { "This is great!", 38 }, { "Very good", -98765 } }))

MAKE_CONTAINER_DATA_PROVIDER((std::unordered_map<std::string, int>), metapp::tkStdUnorderedMap, metapp::tkStdPair,
	({ { "hello", -5 }, { "", 0 }, { "world", 123456789 }, { "This is great!", 38 }, { "Very good", -98765 } }))

MAKE_CONTAINER_DATA_PROVIDER((std::multimap<std::string, int>), metapp::tkStdMultimap, metapp::tkStdPair,
	({ { "hello", -5 }, { "", 0 }, { "world", 123456789 }, { "This is great!", 38 }, { "Very good", -98765 } }))

MAKE_CONTAINER_DATA_PROVIDER((std::unordered_multimap<std::string, int>), metapp::tkStdUnorderedMultimap, metapp::tkStdPair,
	({ { "hello", -5 }, { "", 0 }, { "world", 123456789 }, { "This is great!", 38 }, { "Very good", -98765 } }))

using TestTypes_Indexables = metapp::TypeList<
	std::array<int, 5>,
	std::array<std::string, 5>,
	std::vector<int>,
	std::vector<std::string>,
	std::deque<int>,
	std::deque<std::string>
>;

using TestTypes_Iterables = metapp::TypeList<
	std::array<int, 5>,
	std::array<std::string, 5>,
	std::vector<int>,
	std::vector<std::string>,
	std::deque<int>,
	std::deque<std::string>,
	std::list<int>,
	std::list<std::string>,
	std::forward_list<int>,
	std::forward_list<std::string>,
	
	std::set<int>,
	std::set<std::string>,
	std::unordered_set<int>,
	std::unordered_set<std::string>,
	std::multiset<int>,
	std::multiset<std::string>,
	std::unordered_multiset<int>,
	std::unordered_multiset<std::string>,

	std::map<std::string, int>,
	std::unordered_map<std::string, int>,
	std::multimap<std::string, int>,
	std::unordered_multimap<std::string, int>
>;


#ifdef METAPP_COMPILER_VC
#pragma warning(pop)
#endif


#endif
