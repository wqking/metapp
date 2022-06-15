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

#include "metapp/utilities/utility.h"
#include "metapp/allmetatypes.h"

#include <string>

TEST_CASE("utility, getPointer, int *")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(metapp::getPointer(v) == &n);
	REQUIRE(metapp::getPointedType(v) == metapp::getMetaType<int>());
	REQUIRE(metapp::getPointerAndType(v).first == &n);
	REQUIRE(metapp::getPointerAndType(v).second == metapp::getMetaType<int>());
}

TEST_CASE("utility, getPointer, const int *")
{
	const int n = 5;
	metapp::Variant v(&n);
	REQUIRE(metapp::getPointer(v) == &n);
	REQUIRE(metapp::getPointedType(v) == metapp::getMetaType<const int>());
	REQUIRE(metapp::getPointerAndType(v).first == &n);
	REQUIRE(metapp::getPointerAndType(v).second == metapp::getMetaType<const int>());
}

TEST_CASE("utility, getPointer, std::shared_ptr<int>")
{
	std::shared_ptr<int> sp = std::make_shared<int>(5);
	metapp::Variant v(sp);
	REQUIRE(metapp::getPointer(v) == sp.get());
	REQUIRE(metapp::getPointedType(v) == metapp::getMetaType<int>());
	REQUIRE(metapp::getPointerAndType(v).first == sp.get());
	REQUIRE(metapp::getPointerAndType(v).second == metapp::getMetaType<int>());
}

TEST_CASE("utility, getPointer, std::shared_ptr<const int>")
{
	std::shared_ptr<const int> sp = std::make_shared<const int>(5);
	metapp::Variant v(sp);
	REQUIRE(metapp::getPointer(v) == sp.get());
	REQUIRE(metapp::getPointedType(v) == metapp::getMetaType<const int>());
	REQUIRE(metapp::getPointerAndType(v).first == sp.get());
	REQUIRE(metapp::getPointerAndType(v).second == metapp::getMetaType<const int>());
}

TEST_CASE("utility, getPointer, std::unique_ptr<int>")
{
	std::unique_ptr<int> up(new int(5));
	int * pn = up.get();
	metapp::Variant v(std::move(up));
	REQUIRE(metapp::getPointer(v) == pn);
	REQUIRE(metapp::getPointedType(v) == metapp::getMetaType<int>());
	REQUIRE(metapp::getPointerAndType(v).first == pn);
	REQUIRE(metapp::getPointerAndType(v).second == metapp::getMetaType<int>());
}

TEST_CASE("utility, getPointer, std::unique_ptr<const int>")
{
	std::unique_ptr<const int> up(new int(5));
	const int * pn = up.get();
	metapp::Variant v(std::move(up));
	REQUIRE(metapp::getPointer(v) == pn);
	REQUIRE(metapp::getPointedType(v) == metapp::getMetaType<const int>());
	REQUIRE(metapp::getPointerAndType(v).first == pn);
	REQUIRE(metapp::getPointerAndType(v).second == metapp::getMetaType<const int>());
}

TEST_CASE("utility, isSameMetaType")
{
	REQUIRE(metapp::isSameMetaType<int>(metapp::getMetaType<int>()));
	REQUIRE(metapp::isSameMetaType<long, const int>(metapp::getMetaType<int>()));
	REQUIRE(metapp::isSameMetaType<metapp::TypeList<long, const int> >(metapp::getMetaType<int>()));
	REQUIRE(metapp::isSameMetaType<const char *, std::string>(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::isSameMetaType<const char *, std::string>(metapp::getMetaType<volatile char *>()));

	REQUIRE(! metapp::isSameMetaType<>(metapp::getMetaType<int>()));
	REQUIRE(! metapp::isSameMetaType<long>(metapp::getMetaType<int>()));
	REQUIRE(! metapp::isSameMetaType<unsigned int, long>(metapp::getMetaType<int>()));
}

