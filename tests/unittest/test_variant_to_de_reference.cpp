#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <cstring>
#include <iostream>

namespace {

TEST_CASE("Variant, toReference, int *")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(! v.canGet<int>());
	REQUIRE(v.canGet<int *>());
	REQUIRE(*v.get<int *>() == 5);

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.canGet<int>());
	REQUIRE(ref.get<int>() == 5);

	n = 38;
	REQUIRE(ref.get<int>() == 38);
	REQUIRE(*v.get<int *>() == 38);
}

TEST_CASE("Variant, toReference, int &")
{
	int n = 5;
	metapp::Variant v(metapp::Variant::create<int &>(n));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.canGet<int>());
	REQUIRE(ref.get<int>() == 5);

	n = 38;
	REQUIRE(ref.get<int>() == 38);
	REQUIRE(v.get<int>() == 38);
}

TEST_CASE("Variant, toReference, int")
{
	metapp::Variant v(5);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.canGet<int>());
	REQUIRE(ref.get<int>() == 5);

	v.get<int &>() = 38;
	REQUIRE(ref.get<int>() == 38);
	REQUIRE(v.get<int>() == 38);
}

TEST_CASE("Variant, toReference, std::string")
{
	std::string s = "hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);
	REQUIRE(v.canGet<std::string>());
	REQUIRE(v.get<std::string &>() == "hello");

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.canGet<std::string>());
	REQUIRE(ref.get<std::string>() == "hello");
	REQUIRE(ref.cast<std::string>().get<std::string>() == "hello");

	v.get<std::string &>() = "world";
	REQUIRE(ref.get<std::string>() == "world");
	REQUIRE(v.get<std::string>() == "world");
}

TEST_CASE("Variant, toReference, const std::string, constness")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<const std::string>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);
	REQUIRE(v.canGet<std::string>());
	REQUIRE(v.get<std::string &>() == "hello");
	REQUIRE(v.getMetaType()->isConst());

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.canGet<std::string>());
	REQUIRE(ref.get<std::string>() == "hello");
	REQUIRE(ref.cast<std::string>().get<std::string>() == "hello");
	REQUIRE(ref.getMetaType()->getUpType()->isConst());
}

struct MyClass
{
	std::string text;
	int data[100]; // be sure it can't be stored in Variant internal buffer
};

TEST_CASE("Variant, toReference, MyClass *")
{
	MyClass n { "hello", {0} };
	metapp::Variant v(&n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(! v.canGet<MyClass>());
	REQUIRE(v.canGet<MyClass *>());
	REQUIRE(v.get<MyClass *>()->text == "hello");

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
	REQUIRE(ref.canGet<MyClass>());
	REQUIRE(ref.get<MyClass &>().text == "hello");

	n.text = "world";
	REQUIRE(ref.get<MyClass &>().text == "world");
	REQUIRE(v.get<MyClass *>()->text == "world");
}

TEST_CASE("Variant, toReference, MyClass &")
{
	MyClass n { "hello", {0} };
	metapp::Variant v(metapp::Variant::create<MyClass &>(n));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<MyClass>());
	REQUIRE(v.get<MyClass &>().text == "hello");

	metapp::Variant ref(v.toReference());
	REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
	REQUIRE(ref.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
	REQUIRE(ref.canGet<MyClass>());
	REQUIRE(ref.get<MyClass &>().text == "hello");

	n.text = "world";
	REQUIRE(ref.get<MyClass &>().text == "world");
	REQUIRE(v.get<MyClass &>().text == "world");
}

TEST_CASE("Variant, dereference()")
{
	SECTION("pointer, int *") {
		int n = 5;
		metapp::Variant v(&n);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		REQUIRE(! v.canGet<int>());
		REQUIRE(v.canGet<int *>());
		REQUIRE(*v.get<int *>() == 5);

		metapp::Variant deref(v.dereference());
		REQUIRE(metapp::getTypeKind(deref) == metapp::tkInt);
		REQUIRE(deref.canGet<int>());
		REQUIRE(deref.get<int>() == 5);

		n = 38;
		REQUIRE(deref.get<int>() == 5);
		REQUIRE(*v.get<int *>() == 38);
	}

	SECTION("reference, int &") {
		int n = 5;
		metapp::Variant v(metapp::Variant::create<int &>(n));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
		REQUIRE(v.canGet<int>());
		REQUIRE(v.get<int>() == 5);

		metapp::Variant deref(v.dereference());
		REQUIRE(metapp::getTypeKind(deref) == metapp::tkInt);
		REQUIRE(deref.canGet<int>());
		REQUIRE(deref.get<int>() == 5);

		n = 38;
		REQUIRE(deref.get<int>() == 5);
		REQUIRE(v.get<int>() == 38);
	}

	struct MyClass
	{
		std::string text;
		int data[100]; // be sure it can't be stored in Variant internal buffer
	};

	SECTION("pointer, MyClass *") {
		MyClass n { "hello", {0} };
		metapp::Variant v(&n);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		REQUIRE(! v.canGet<MyClass>());
		REQUIRE(v.canGet<MyClass *>());
		REQUIRE(v.get<MyClass *>()->text == "hello");

		metapp::Variant deref(v.dereference());
		REQUIRE(metapp::getTypeKind(deref) == metapp::tkObject);
		REQUIRE(deref.getMetaType() == metapp::getMetaType<MyClass>());
		REQUIRE(deref.canGet<MyClass>());
		REQUIRE(deref.get<MyClass &>().text == "hello");

		n.text = "world";
		REQUIRE(deref.get<MyClass &>().text == "hello");
		REQUIRE(v.get<MyClass *>()->text == "world");
	}

	SECTION("reference, MyClass &") {
		MyClass n { "hello", {0} };
		metapp::Variant v(metapp::Variant::create<MyClass &>(n));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
		REQUIRE(v.canGet<MyClass>());
		REQUIRE(v.get<MyClass &>().text == "hello");

		metapp::Variant deref(v.dereference());
		REQUIRE(metapp::getTypeKind(deref) == metapp::tkObject);
		REQUIRE(deref.getMetaType() == metapp::getMetaType<MyClass>());
		REQUIRE(deref.canGet<MyClass>());
		REQUIRE(deref.get<MyClass &>().text == "hello");

		n.text = "world";
		REQUIRE(deref.get<MyClass &>().text == "hello");
		REQUIRE(v.get<MyClass &>().text == "world");
	}

}

} // namespace
