#include "test.h"

#include "metapp/metarepository.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

TEST_CASE("MetaRepository")
{
	REQUIRE(metapp::getMetaRepository()->getTypeByKind(metapp::tkInt) != nullptr);
	REQUIRE(metapp::getMetaRepository()->getTypeByName("int") != nullptr);
}


