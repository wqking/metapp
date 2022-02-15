#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("MetaType")
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


} // namespace
