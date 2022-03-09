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

TEST_CASE("Variant, ctor Variant(const MetaType * metaType, nullptr)")
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
		};
		metapp::Variant v(metapp::getMetaType<MyClass>(), nullptr);
		REQUIRE(v.getMetaType() == metapp::getMetaType<MyClass>());
		REQUIRE(v.get<MyClass &>().value == 5);
	}
}

TEST_CASE("Variant, ctor Variant(const MetaType * metaType, copyFrom)")
{
	SECTION("int") {
		int n = 38;
		metapp::Variant v(metapp::getMetaType<int>(), &n);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkInt);
		REQUIRE(v.get<int>() == 38);
	}
	SECTION("MyClass") {
		struct MyClass
		{
			int value;
		};
		MyClass copyFrom { 98 };
		metapp::Variant v(metapp::getMetaType<MyClass>(), &copyFrom);
		REQUIRE(v.getMetaType() == metapp::getMetaType<MyClass>());
		REQUIRE(v.get<MyClass &>().value == 98);
	}
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
	}

	SECTION("int &") {
		int n = 38;
		metapp::Variant v(metapp::Variant::create<int &>(n));
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
}

TEST_CASE("Variant, toReference()")
{
	SECTION("pointer, int *") {
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

	SECTION("reference, int &") {
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

	SECTION("value, int") {
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

	SECTION("value, std::string") {
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

		metapp::Variant ref(v.toReference());
		REQUIRE(metapp::getTypeKind(ref) == metapp::tkReference);
		REQUIRE(ref.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
		REQUIRE(ref.canGet<MyClass>());
		REQUIRE(ref.get<MyClass &>().text == "hello");

		n.text = "world";
		REQUIRE(ref.get<MyClass &>().text == "world");
		REQUIRE(v.get<MyClass *>()->text == "world");
	}

	SECTION("reference, MyClass &") {
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
