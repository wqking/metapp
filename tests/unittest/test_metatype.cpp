#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

TEST_CASE("MetaType, isCallable()")
{
	REQUIRE(! metapp::getMetaType<int>()->isCallable());
	REQUIRE(metapp::getMetaType<void ()>()->isCallable());
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

