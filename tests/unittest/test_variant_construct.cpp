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

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <cstring>
#include <iostream>

namespace {

TEST_CASE("Variant, construct, Variant()")
{
	metapp::Variant v;
	REQUIRE(metapp::getTypeKind(v) == metapp::tkVoid);
	REQUIRE(v.canCast<void>());
	REQUIRE(! v.canCast<int>());
	REQUIRE(! v.canCast<void *>());
}

TEST_CASE("Variant, construct, Variant(const MetaType * metaType, nullptr)")
{
	SECTION("int") {
		metapp::Variant v(metapp::getMetaType<int>(), nullptr);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 0);
	}

	SECTION("MyClass") {
		struct MyClass
		{
			int value = 5;
			std::string text = "Hello";
		};
		metapp::Variant v(metapp::getMetaType<MyClass>(), nullptr);
		REQUIRE(v.getMetaType() == metapp::getMetaType<MyClass>());
		REQUIRE(v.get<const MyClass &>().value == 5);
		REQUIRE(v.get<const MyClass &>().text == "Hello");
	}
}

TEST_CASE("Variant, construct, Variant(T && value)")
{
	SECTION("int") {
		metapp::Variant v(5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(! v.getMetaType()->isConst());
	}

	SECTION("const int") {
		const int n = 5;
		metapp::Variant v(n);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(v.getMetaType()->isConst());
	}

	SECTION("const int &") {
		int n = 5;
		const int & rn = n;
		metapp::Variant v(rn);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(v.getMetaType()->isConst());
	}

	SECTION("vector, copy") {
		std::vector<int> dataList { 1, 2, 3 };
		metapp::Variant v(dataList);
		REQUIRE(dataList.size() == 3);
		REQUIRE(dataList[0] == 1);
		REQUIRE(v.get<const std::vector<int> &>().size() == 3);
		REQUIRE(v.get<const std::vector<int> &>()[0] == 1);
	}

	SECTION("vector, move") {
		std::vector<int> dataList { 1, 2, 3 };
		metapp::Variant v(std::move(dataList));
		REQUIRE(dataList.empty());
		REQUIRE(v.get<const std::vector<int> &>().size() == 3);
		REQUIRE(v.get<const std::vector<int> &>()[0] == 1);
	}

}

TEST_CASE("Variant, construct, operator = ")
{
	SECTION("int") {
		metapp::Variant v;
		v = 5;
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(! v.getMetaType()->isConst());
	}

	SECTION("const int") {
		const int n = 5;
		metapp::Variant v;
		v = n;
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(v.getMetaType()->isConst());
	}

	SECTION("const int &") {
		int n = 5;
		const int & rn = n;
		metapp::Variant v;
		v = rn;
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(v.getMetaType()->isConst());
	}

	SECTION("vector, copy") {
		std::vector<int> dataList { 1, 2, 3 };
		metapp::Variant v;
		v = dataList;
		REQUIRE(dataList.size() == 3);
		REQUIRE(dataList[0] == 1);
		REQUIRE(v.get<const std::vector<int> &>().size() == 3);
		REQUIRE(v.get<const std::vector<int> &>()[0] == 1);
	}

	SECTION("vector, move") {
		std::vector<int> dataList { 1, 2, 3 };
		metapp::Variant v;
		v = std::move(dataList);
		REQUIRE(dataList.empty());
		REQUIRE(v.get<const std::vector<int> &>().size() == 3);
		REQUIRE(v.get<const std::vector<int> &>()[0] == 1);
	}

}

TEST_CASE("Variant, construct, create()")
{
	SECTION("int") {
		metapp::Variant v = metapp::Variant::create<int>(5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(! v.getMetaType()->isConst());
	}

	SECTION("const int") {
		metapp::Variant v = metapp::Variant::create<const int>(5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 5);
		REQUIRE(v.getMetaType()->isConst());
	}

	SECTION("vector, copy") {
		std::vector<int> dataList { 1, 2, 3 };
		metapp::Variant v = metapp::Variant::create<std::vector<int> >(dataList);
		REQUIRE(dataList.size() == 3);
		REQUIRE(dataList[0] == 1);
		REQUIRE(v.get<const std::vector<int> &>().size() == 3);
		REQUIRE(v.get<const std::vector<int> &>()[0] == 1);
	}

	SECTION("vector, move") {
		std::vector<int> dataList { 1, 2, 3 };
		metapp::Variant v = metapp::Variant::create<std::vector<int> >(std::move(dataList));
		REQUIRE(dataList.empty());
		REQUIRE(v.get<const std::vector<int> &>().size() == 3);
		REQUIRE(v.get<const std::vector<int> &>()[0] == 1);
	}

}


} // namespace
