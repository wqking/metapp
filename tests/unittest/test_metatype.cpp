#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

TEST_CASE("MetaType, construct")
{
	SECTION("int") {
		std::unique_ptr<int> instance((int *)metapp::getMetaType<int>()->construct());
		REQUIRE(*instance == 0);
	}
	SECTION("MyClass") {
		struct MyClass
		{
			int value = 5;
		};
		std::unique_ptr<MyClass> instance((MyClass *)metapp::getMetaType<MyClass>()->construct());
		REQUIRE(instance->value == 5);
	}
}

TEST_CASE("MetaType, copyConstruct")
{
	SECTION("int") {
		int n = 38;
		std::unique_ptr<int> instance((int *)metapp::getMetaType<int>()->copyConstruct(&n));
		REQUIRE(*instance == 38);
	}
	SECTION("MyClass") {
		struct MyClass
		{
			int value;
		};
		MyClass copyFrom { 98 };
		std::unique_ptr<MyClass> instance((MyClass *)metapp::getMetaType<MyClass>()->copyConstruct(&copyFrom));
		REQUIRE(instance->value == 98);
	}
}

TEST_CASE("MetaType, getMetaCallable()")
{
	REQUIRE(metapp::getMetaType<int>()->getMetaCallable() == nullptr);
	REQUIRE(metapp::getMetaType<void ()>()->getMetaCallable() != nullptr);
}

TEST_CASE("MetaType, arithmetic")
{
	REQUIRE(metapp::getMetaType<int>() == metapp::getMetaType<int>());
	REQUIRE(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
	REQUIRE(metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType());
	REQUIRE(metapp::getMetaType<const int>()->isConst());

	REQUIRE(metapp::getMetaType<int &>() == metapp::getMetaType<int &>());
	REQUIRE(metapp::getMetaType<int &>() != metapp::getMetaType<const int &>());
	REQUIRE(metapp::getMetaType<int &>()->getUpType()->getUnifiedType() == metapp::getMetaType<const int &>()->getUpType()->getUnifiedType());
	REQUIRE(metapp::getMetaType<const int &>()->getUpType()->isConst());
}

TEST_CASE("MetaType, std::map")
{
	auto metaType = metapp::getMetaType<std::map<int, std::string> >();
	REQUIRE(metaType->getUpTypeCount() == 2);
	REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkInt);
	REQUIRE(metaType->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

