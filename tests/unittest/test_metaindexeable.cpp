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
#include "metapp/interfaces/metaindexable.h"
#include "metapp/allmetatypes.h"

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
	metapp::Variant v;
	SECTION("value") {
		v = container;
	}
	SECTION("pointer") {
		v = &container;
	}
	SECTION("reference") {
		v = metapp::Variant::reference(container);
	}
	auto metaIndexable = metapp::getPointedType(v)->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);

	auto nonPointer = metapp::depointer(v);
	REQUIRE(metaIndexable->getSizeInfo(nonPointer).getSize() == container.size());
	for(std::size_t i = 0; i < container.size(); ++i) {
		const auto item = metaIndexable->get(nonPointer, i);
		REQUIRE(item.getMetaType()->isReference());
		REQUIRE(item.template get<ValueType>() == container.at(i));
	}
}

TEMPLATE_LIST_TEST_CASE("MetaIndexable set", "", TestTypes_Indexables)
{
	using Container = TestType;
	using ValueType = typename Container::value_type;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container container = dataProvider.getContainer();

	metapp::Variant v;
	SECTION("value") {
		v = container;
	}
	SECTION("pointer") {
		v = &container;
	}
	SECTION("reference") {
		v = metapp::Variant::reference(container);
	}

	REQUIRE(metapp::getPointedType(v)->getTypeKind() == dataProvider.getTypeKind());
	REQUIRE(metapp::getPointedType(v)->getUpType()->getTypeKind() == dataProvider.getValueTypeKind());
	
	auto metaIndexable = metapp::getPointedType(v)->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);

	auto nonPointer = metapp::depointer(v);
	REQUIRE(metaIndexable->getSizeInfo(nonPointer).getSize() == container.size());

	REQUIRE(metaIndexable->get(nonPointer, 0).template get<ValueType>() != container.at(1));
	metaIndexable->set(nonPointer, 0, container.at(1));
	REQUIRE(metaIndexable->get(nonPointer, 0).template get<ValueType>() == container.at(1));

	REQUIRE(metaIndexable->get(nonPointer, 2).template get<ValueType>() != container.at(1));
	metaIndexable->set(nonPointer, 2, container.at(1));
	REQUIRE(metaIndexable->get(nonPointer, 2).template get<ValueType>() == container.at(1));
}

TEMPLATE_LIST_TEST_CASE("MetaIndexable set T &", "", TestTypes_Indexables)
{
	using Container = TestType;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container container = dataProvider.getContainer();
	metapp::Variant v(metapp::Variant::reference(container));
	REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkReference);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == dataProvider.getTypeKind());
	REQUIRE(v.getMetaType()->getUpType()->getUpType()->getTypeKind() == dataProvider.getValueTypeKind());

	auto metaIndexable = v.getMetaType()->getUpType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSizeInfo(v).getSize() == container.size());

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
