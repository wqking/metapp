#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, cv, top level")
{
	const metapp::MetaType * metaType;

	metaType = metapp::getMetaType<const int>();
	REQUIRE(metaType->isConst());
	
	metaType = metapp::getMetaType<volatile int>();
	REQUIRE(metaType->isVolatile());
	
	metaType = metapp::getMetaType<const volatile int>();
	REQUIRE(metaType->isConst());
	REQUIRE(metaType->isVolatile());
}

TEST_CASE("metatypes, cv, reference")
{
	const metapp::MetaType * metaType;
	
	metaType = metapp::getMetaType<const int &>();
	REQUIRE(! metaType->isConst());
	REQUIRE(metaType->getUpType()->isConst());
	
	metaType = metapp::getMetaType<volatile int &>();
	REQUIRE(! metaType->isVolatile());
	REQUIRE(metaType->getUpType()->isVolatile());
	
	metaType = metapp::getMetaType<const volatile int &>();
	REQUIRE(! metaType->isConst());
	REQUIRE(! metaType->isVolatile());
	REQUIRE(metaType->getUpType()->isConst());
	REQUIRE(metaType->getUpType()->isVolatile());
}

