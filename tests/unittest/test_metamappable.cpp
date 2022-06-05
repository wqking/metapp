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
#include "include/dataprovider.h"

#include "metapp/variant.h"
#include "metapp/interfaces/metamappable.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEMPLATE_LIST_TEST_CASE("MetaMappable get", "", TestTypes_Mappable)
{
	using Container = TestType;
	using ValueType = typename Container::value_type;
	using MappedType = typename Container::mapped_type;
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
	auto metaMappable = metapp::getPointedType(v)->getMetaMappable();
	REQUIRE(metaMappable != nullptr);

	auto nonPointer = metapp::depointer(v);
	REQUIRE(metaMappable->getValueType(nonPointer)->equal(metapp::getMetaType<ValueType>()));
	REQUIRE(metaMappable->getValueType(nonPointer)->getTypeKind() == metapp::tkStdPair);
	for(const auto item : container) {
		REQUIRE(metaMappable->get(nonPointer, item.first).get<const MappedType &>() == item.second);
		REQUIRE(metapp::mappableGet(nonPointer, item.first).get<const MappedType &>() == item.second);
	}
}

TEMPLATE_LIST_TEST_CASE("MetaMappable set", "", TestTypes_Mappable)
{
	using Container = TestType;
	using KeyType = typename Container::key_type;
	using ValueType = typename Container::value_type;
	using MappedType = typename Container::mapped_type;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container sample = dataProvider.getContainer();
	Container container;
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

	auto nonPointer = metapp::depointer(v);
	for(const auto item : sample) {
		REQUIRE(metapp::mappableGet(nonPointer, item.first).isEmpty());
		metapp::mappableSet(nonPointer, item.first, item.second);
		REQUIRE(metapp::mappableGet(nonPointer, item.first).get<const MappedType &>() == item.second);

		const auto newValue = TestDataProvider<MappedType>().getData(2);
		REQUIRE(metapp::mappableGet(nonPointer, item.first).get<const MappedType &>() != newValue);
		metapp::mappableSet(nonPointer, item.first, newValue);
		REQUIRE(metapp::mappableGet(nonPointer, item.first).get<const MappedType &>() == newValue);
	}
}


} // namespace
