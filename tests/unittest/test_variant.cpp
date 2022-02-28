#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

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

TEST_CASE("Variant, ctor Variant()")
{
	metapp::Variant v;
	REQUIRE(metapp::getTypeKind(v) == metapp::tkVoid);
	REQUIRE(! v.canCast<void>());
	REQUIRE(! v.canCast<int>());
	REQUIRE(! v.canCast<void *>());
}

TEST_CASE("Variant, ctor Variant(const MetaType * metaType)")
{
	metapp::Variant v(metapp::getMetaType<MyClass>());
	REQUIRE(v.get<MyClass &>().value == 9);
}

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



} // namespace
