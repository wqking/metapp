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

#include "test.h"
#include "include/containerdataprovider.h"

#include "metapp/variant.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEMPLATE_LIST_TEST_CASE("MetaIndexable get", "", TestTypes_Indexables)
{
	using Container = TestType;
	using ValueType = typename Container::value_type;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container container = dataProvider.getContainer();
	metapp::Variant v(container);
	auto metaIndexable = v.getMetaType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == container.size());
	for(size_t i = 0; i < container.size(); ++i) {
		REQUIRE(metaIndexable->get(v, i).template get<ValueType>() == container.at(i));
	}
}

TEMPLATE_LIST_TEST_CASE("MetaIndexable set", "", TestTypes_Indexables)
{
	using Container = TestType;
	using ValueType = typename Container::value_type;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container container = dataProvider.getContainer();
	metapp::Variant v(container);
	REQUIRE(v.getMetaType()->getTypeKind() == dataProvider.getTypeKind());
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == dataProvider.getValueTypeKind());
	
	auto metaIndexable = v.getMetaType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == container.size());

	REQUIRE(metaIndexable->get(v, 0).template get<ValueType>() != container.at(1));
	metaIndexable->set(v, 0, container.at(1));
	REQUIRE(metaIndexable->get(v, 0).template get<ValueType>() == container.at(1));

	REQUIRE(metaIndexable->get(v, 2).template get<ValueType>() != container.at(1));
	metaIndexable->set(v, 2, container.at(1));
	REQUIRE(metaIndexable->get(v, 2).template get<ValueType>() == container.at(1));
}

TEMPLATE_LIST_TEST_CASE("MetaIndexable set T &", "", TestTypes_Indexables)
{
	using Container = TestType;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container container = dataProvider.getContainer();
	metapp::Variant v(metapp::Variant::create<Container &>(container));
	REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkReference);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == dataProvider.getTypeKind());
	REQUIRE(v.getMetaType()->getUpType()->getUpType()->getTypeKind() == dataProvider.getValueTypeKind());

	auto metaIndexable = v.getMetaType()->getUpType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == container.size());

	auto value0 = container.at(0);
	auto value1 = container.at(1);
	auto value2 = container.at(2);

	REQUIRE(container.at(0) == value0);
	REQUIRE(container.at(0) != value1);
	metaIndexable->set(v, 0, value1);
	REQUIRE(container.at(0) == value1);

	REQUIRE(container.at(2) == value2);
	REQUIRE(container.at(2) != value1);
	metaIndexable->set(v, 2, value1);
	REQUIRE(container.at(2) == value1);
}


} // namespace
