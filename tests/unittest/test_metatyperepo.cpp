#include "test.h"

#include "metapp/metatyperepo.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("MetaTypeRepo")
{
	metapp::getMetaTypeRepo()->getMetaType(metapp::tkInt);
	REQUIRE(metapp::getMetaTypeRepo()->getMetaType(metapp::tkInt) != nullptr);
}


} // namespace
