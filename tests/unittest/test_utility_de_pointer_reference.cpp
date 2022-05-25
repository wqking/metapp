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

TEST_CASE("utility, depointer, int *")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(! v.canGet<int>());
	REQUIRE(v.canGet<int *>());
	REQUIRE(*v.get<int *>() == 5);

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkReference);
	REQUIRE(nonPointer.canGet<int>());
	REQUIRE(nonPointer.get<int>() == 5);

	n = 38;
	REQUIRE(nonPointer.get<int>() == 38);
	REQUIRE(*v.get<int *>() == 38);
}

TEST_CASE("utility, depointer, int &")
{
	int n = 5;
	metapp::Variant v(metapp::Variant::create<int &>(n));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkReference);
	REQUIRE(nonPointer.canGet<int>());
	REQUIRE(nonPointer.get<int>() == 5);

	n = 38;
	REQUIRE(nonPointer.get<int>() == 38);
	REQUIRE(v.get<int>() == 38);
}

TEST_CASE("utility, depointer, int")
{
	metapp::Variant v(5);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkInt);
	REQUIRE(nonPointer.canGet<int>());
	REQUIRE(nonPointer.get<int>() == 5);

	v.get<int &>() = 38;
	REQUIRE(nonPointer.get<int>() == 5);
	REQUIRE(v.get<int>() == 38);
}

TEST_CASE("utility, depointer, std::string")
{
	std::string s = "hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);
	REQUIRE(v.canGet<std::string>());
	REQUIRE(v.get<std::string &>() == "hello");

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkStdString);
	REQUIRE(nonPointer.canGet<std::string>());
	REQUIRE(nonPointer.get<std::string>() == "hello");
	REQUIRE(nonPointer.cast<std::string>().get<std::string>() == "hello");

	v.get<std::string &>() = "world";
	// Even though both v and nonPointer are values, they refer to the same copy of std::string
	REQUIRE(nonPointer.get<std::string>() == "world");
	REQUIRE(v.get<std::string>() == "world");
}

TEST_CASE("utility, depointer, const std::string, constness")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<const std::string>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);
	REQUIRE(v.canGet<std::string>());
	REQUIRE(v.get<std::string &>() == "hello");
	REQUIRE(v.getMetaType()->isConst());

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkStdString);
	REQUIRE(nonPointer.canGet<std::string>());
	REQUIRE(nonPointer.get<std::string>() == "hello");
	REQUIRE(nonPointer.cast<std::string>().get<std::string>() == "hello");
	REQUIRE(nonPointer.getMetaType()->isConst());
}

TEST_CASE("utility, depointer, const std::string &, constness")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<const std::string &>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<std::string>());
	REQUIRE(v.get<std::string &>() == "hello");
	REQUIRE(! v.getMetaType()->isConst());
	REQUIRE(v.getMetaType()->getUpType()->isConst());

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkReference);
	REQUIRE(nonPointer.canGet<std::string>());
	REQUIRE(nonPointer.get<std::string>() == "hello");
	REQUIRE(nonPointer.cast<std::string>().get<std::string>() == "hello");
	REQUIRE(! nonPointer.getMetaType()->isConst());
	REQUIRE(nonPointer.getMetaType()->getUpType()->isConst());
}

TEST_CASE("utility, depointer, const std::string *, constness")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<const std::string *>(&s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.canGet<std::string *>());
	REQUIRE(*v.get<std::string *>() == "hello");
	REQUIRE(! v.getMetaType()->isConst());
	REQUIRE(v.getMetaType()->getUpType()->isConst());

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkReference);
	REQUIRE(nonPointer.canGet<std::string>());
	REQUIRE(nonPointer.get<std::string>() == "hello");
	REQUIRE(nonPointer.cast<std::string>().get<std::string>() == "hello");
	REQUIRE(! nonPointer.getMetaType()->isConst());
	REQUIRE(nonPointer.getMetaType()->getUpType()->isConst());
}

struct MyClass
{
	std::string text;
	int data[100]; // be sure it can't be stored in Variant internal buffer
};

TEST_CASE("utility, depointer, MyClass *")
{
	MyClass n { "hello", {0} };
	metapp::Variant v(&n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(! v.canGet<MyClass>());
	REQUIRE(v.canGet<MyClass *>());
	REQUIRE(v.get<MyClass *>()->text == "hello");

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkReference);
	REQUIRE(nonPointer.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
	REQUIRE(nonPointer.canGet<MyClass>());
	REQUIRE(nonPointer.get<MyClass &>().text == "hello");

	n.text = "world";
	REQUIRE(nonPointer.get<MyClass &>().text == "world");
	REQUIRE(v.get<MyClass *>()->text == "world");
}

TEST_CASE("utility, depointer, MyClass &")
{
	MyClass n { "hello", {0} };
	metapp::Variant v(metapp::Variant::create<MyClass &>(n));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<MyClass>());
	REQUIRE(v.get<MyClass &>().text == "hello");

	metapp::Variant nonPointer(depointer(v));
	REQUIRE(metapp::getTypeKind(nonPointer) == metapp::tkReference);
	REQUIRE(nonPointer.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
	REQUIRE(nonPointer.canGet<MyClass>());
	REQUIRE(nonPointer.get<MyClass &>().text == "hello");

	n.text = "world";
	REQUIRE(nonPointer.get<MyClass &>().text == "world");
	REQUIRE(v.get<MyClass &>().text == "world");
}

TEST_CASE("utility, dereference, int *")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(! v.canGet<int>());
	REQUIRE(v.canGet<int *>());
	REQUIRE(*v.get<int *>() == 5);

	metapp::Variant deref(dereference(v));
	REQUIRE(metapp::getTypeKind(deref) == metapp::tkInt);
	REQUIRE(deref.canGet<int>());
	REQUIRE(deref.get<int>() == 5);

	n = 38;
	REQUIRE(deref.get<int>() == 5);
	REQUIRE(*v.get<int *>() == 38);
}

TEST_CASE("utility, dereference, int &")
{
	int n = 5;
	metapp::Variant v(metapp::Variant::create<int &>(n));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);

	metapp::Variant deref(dereference(v));
	REQUIRE(metapp::getTypeKind(deref) == metapp::tkInt);
	REQUIRE(deref.canGet<int>());
	REQUIRE(deref.get<int>() == 5);

	n = 38;
	REQUIRE(deref.get<int>() == 5);
	REQUIRE(v.get<int>() == 38);
}

TEST_CASE("utility, dereference, MyClass *")
{
	MyClass n { "hello", {0} };
	metapp::Variant v(&n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(! v.canGet<MyClass>());
	REQUIRE(v.canGet<MyClass *>());
	REQUIRE(v.get<MyClass *>()->text == "hello");

	metapp::Variant deref(dereference(v));
	REQUIRE(metapp::getTypeKind(deref) == metapp::tkObject);
	REQUIRE(deref.getMetaType() == metapp::getMetaType<MyClass>());
	REQUIRE(deref.canGet<MyClass>());
	REQUIRE(deref.get<MyClass &>().text == "hello");

	n.text = "world";
	REQUIRE(deref.get<MyClass &>().text == "hello");
	REQUIRE(v.get<MyClass *>()->text == "world");
}

TEST_CASE("utility, dereference, MyClass &")
{
	MyClass n { "hello", {0} };
	metapp::Variant v(metapp::Variant::create<MyClass &>(n));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<MyClass>());
	REQUIRE(v.get<MyClass &>().text == "hello");

	metapp::Variant deref(dereference(v));
	REQUIRE(metapp::getTypeKind(deref) == metapp::tkObject);
	REQUIRE(deref.getMetaType() == metapp::getMetaType<MyClass>());
	REQUIRE(deref.canGet<MyClass>());
	REQUIRE(deref.get<MyClass &>().text == "hello");

	n.text = "world";
	REQUIRE(deref.get<MyClass &>().text == "hello");
	REQUIRE(v.get<MyClass &>().text == "world");
}


} // namespace
