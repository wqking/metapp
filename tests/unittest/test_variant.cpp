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

struct MyClass
{
	explicit MyClass(const int n = 9) : value(n) {}

	int value;
	char large[1024]; // large enough not fit in Variant internal buffer
};

TEST_CASE("Variant, copy ctor, MyClass")
{
	MyClass obj { 38 };
	
	metapp::Variant v(obj);
	metapp::Variant copied(v);
	REQUIRE(v.get<MyClass &>().value == 38);
	REQUIRE(copied.get<MyClass &>().value == 38);

	v.get<MyClass &>().value = 5;
	REQUIRE(v.get<MyClass &>().value == 5);
	REQUIRE(copied.get<MyClass &>().value == 5);
}

TEST_CASE("Variant, assignment, Variant & operator = (T value)")
{
	metapp::Variant v;
	REQUIRE(metapp::getTypeKind(v) == metapp::tkVoid);
	v = 5;
	REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
	REQUIRE(v.get<int>() == 5);
	v = std::string("abc");
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);
	REQUIRE(v.get<std::string>() == "abc");
}

TEST_CASE("Variant, clone, MyClass")
{
	MyClass obj { 38 };
	
	metapp::Variant v(obj);
	metapp::Variant cloned(v.clone());
	REQUIRE(v.get<MyClass &>().value == 38);
	REQUIRE(cloned.get<MyClass &>().value == 38);

	v.get<MyClass &>().value = 5;
	REQUIRE(v.get<MyClass &>().value == 5);
	REQUIRE(cloned.get<MyClass &>().value == 38);
}

TEST_CASE("Variant::assign, int")
{
	metapp::Variant v(5);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
	REQUIRE(v.get<int>() == 5);
	
	v.assign(6.2);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
	REQUIRE(v.get<int>() == 6);
}

TEST_CASE("Variant::assign, int &")
{
	int n = 38;
	
	metapp::Variant v(metapp::Variant::reference(n));
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<int &>()));
	REQUIRE(v.get<int>() == 38);
	REQUIRE(n == 38);
	
	v.assign(6.2);
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<int &>()));
	REQUIRE(v.get<int>() == 6);
	REQUIRE(n == 6);
}

TEST_CASE("Variant::assign, MyClass")
{
	metapp::Variant v(MyClass { 38 });
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<MyClass>()));
	REQUIRE(v.get<const MyClass &>().value == 38);

	v.assign(MyClass { 9 });
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<MyClass>()));
	REQUIRE(v.get<const MyClass &>().value == 9);
}

TEST_CASE("Variant::assign, MyClass &")
{
	MyClass obj { 5 };

	metapp::Variant v(metapp::Variant::reference(obj));
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<MyClass &>()));
	REQUIRE(v.get<const MyClass &>().value == 5);
	REQUIRE(obj.value == 5);

	v.assign(MyClass { 9876 });
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<MyClass &>()));
	REQUIRE(v.get<const MyClass &>().value == 9876);
	REQUIRE(obj.value == 9876);
}

TEST_CASE("Variant, create")
{
}

TEST_CASE("Variant, reference")
{
	SECTION("int &") {
		int n = 5;
		metapp::Variant ref = metapp::Variant::reference(n);
		REQUIRE(ref.getMetaType()->isReference());
		REQUIRE(ref.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(ref.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
		REQUIRE(! ref.getMetaType()->getUpType()->isConst());
		REQUIRE(ref.get<int>() == 5);
		n = 38;
		REQUIRE(ref.get<int>() == 38);
		ref.assign(97.3);
		REQUIRE(ref.get<int>() == 97);
		REQUIRE(n == 97);
	}

	SECTION("const int &") {
		const int n = 5;
		metapp::Variant ref = metapp::Variant::reference(n);
		REQUIRE(ref.getMetaType()->isReference());
		REQUIRE(ref.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(ref.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
		REQUIRE(ref.getMetaType()->getUpType()->isConst());
		REQUIRE(ref.get<int>() == 5);
	}

	SECTION("int &&") {
		int n = 5;
		metapp::Variant ref = metapp::Variant::reference(std::move(n));
		REQUIRE(ref.getMetaType()->isReference());
		REQUIRE(ref.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(ref.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
		REQUIRE(ref.get<int>() == 5);
		n = 38;
		REQUIRE(ref.get<int>() == 38);
	}

	SECTION("metapp::Variant &") {
		metapp::Variant v = 5;
		metapp::Variant ref = metapp::Variant::reference(v);
		REQUIRE(ref.getMetaType()->isReference());
		REQUIRE(ref.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(ref.getMetaType()->getUpType()->getTypeKind() == metapp::tkVariant);
		REQUIRE(! ref.getMetaType()->getUpType()->isConst());
		REQUIRE(ref.get<const metapp::Variant &>().get<int>() == 5);
		v = std::string("abc");
		REQUIRE(ref.get<const metapp::Variant &>().get<const std::string &>() == "abc");
		ref.assign(38);
		REQUIRE(ref.get<const metapp::Variant &>().get<int>() == 38);
	}

}

struct CtorCounter
{
	CtorCounter() = default;
	CtorCounter(int * ctorCounter) : ctorCounter(ctorCounter) {
		++*ctorCounter;
	}
	~CtorCounter() {
		--*ctorCounter;
	}

	int * ctorCounter;

};

TEST_CASE("Variant, Variant::takeFrom(const MetaType * metaType, void * object)")
{
	int counter = 0;
	void * ctorCounter = new CtorCounter(&counter);
	REQUIRE(counter == 1);

	SECTION("out of scope") {
		{
			metapp::Variant v(metapp::Variant::takeFrom(metapp::getMetaType<CtorCounter>(), ctorCounter));
			REQUIRE(counter == 1);
		}
		REQUIRE(counter == 0);
	}

	SECTION("assign") {
		metapp::Variant v(metapp::Variant::takeFrom(metapp::getMetaType<CtorCounter>(), ctorCounter));
		REQUIRE(counter == 1);
		v = 5;
		REQUIRE(counter == 0);
	}
}

TEST_CASE("Variant, Variant::takeFrom(const Variant & object)")
{
	int counter = 0;
	CtorCounter * ctorCounter = new CtorCounter(&counter);
	REQUIRE(counter == 1);

	SECTION("out of scope") {
		{
			metapp::Variant v(metapp::Variant::takeFrom(ctorCounter));
			REQUIRE(counter == 1);
		}
		REQUIRE(counter == 0);
	}

	SECTION("assign") {
		metapp::Variant v(metapp::Variant::takeFrom(ctorCounter));
		REQUIRE(counter == 1);
		v = 5;
		REQUIRE(counter == 0);
	}
}

TEST_CASE("Variant, get/canGet")
{
	SECTION("int") {
		metapp::Variant v(5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.canGet<int>());
		REQUIRE(v.canGet<int &>());
		REQUIRE(v.canGet<const int &>());
		REQUIRE(v.canGet<volatile int &>());
		REQUIRE(v.get<int>() == 5);
		REQUIRE(v.get<int &>() == 5);
		REQUIRE(v.get<const int &>() == 5);
		REQUIRE(v.get<volatile int &>() == 5);

		REQUIRE(! v.canGet<int *>());
		REQUIRE_THROWS(v.get<int *>());
	}

	SECTION("int &") {
		int n = 38;
		metapp::Variant v(metapp::Variant::reference(n));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
		REQUIRE(v.canGet<int>());
		REQUIRE(v.canGet<int &>());
		REQUIRE(v.canGet<const int &>());
		REQUIRE(v.canGet<volatile int &>());
		REQUIRE(v.get<int>() == 38);
		REQUIRE(v.get<int &>() == 38);
		REQUIRE(v.get<const int &>() == 38);
		REQUIRE(v.get<volatile int &>() == 38);
	}

	SECTION("int *") {
		int n = 38;
		int * pn = &n;
		metapp::Variant v(pn);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		REQUIRE(v.canGet<int *>());
		REQUIRE(v.canGet<int * &>());
		REQUIRE(v.canGet<const int * &>());
		REQUIRE(v.canGet<volatile int * &>());
		REQUIRE(v.get<int *>() == pn);
		REQUIRE(v.get<int * &>() == pn);
		REQUIRE(v.get<const int * &>() == pn);
		REQUIRE(v.get<volatile int * &>() == pn);
	}

	SECTION("int * &") {
		int n = 38;
		int * pn = &n;
		metapp::Variant v(metapp::Variant::reference(pn));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
		REQUIRE(v.canGet<int *>());
		REQUIRE(v.canGet<int * &>());
		REQUIRE(v.canGet<const int * &>());
		REQUIRE(v.canGet<volatile int * &>());
		REQUIRE(v.get<int *>() == pn);
		REQUIRE(v.get<int * &>() == pn);
		REQUIRE(v.get<const int * &>() == pn);
		REQUIRE(v.get<volatile int * &>() == pn);
	}
}


} // namespace
