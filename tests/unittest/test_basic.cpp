#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/metarepository.h"
#include "metapp/utilities/metatypedumper.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("play camp")
{
	//metapp::MetaTypeDumper().dump(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
	
	REQUIRE(metapp::getMetaType<int *>()->getTypeKind() == metapp::tkPointer);
	REQUIRE(metapp::getMetaType<int *>()->getUpTypeCount() == 1);
	REQUIRE(metapp::getMetaType<int *>()->getUpType()->getTypeKind() == metapp::tkInt);

	REQUIRE(metapp::getMetaType<std::vector<char> >()->getTypeKind() == metapp::tkStdVector);
	REQUIRE(metapp::getMetaType<std::vector<char> >()->getUpTypeCount() == 1);
	REQUIRE(metapp::getMetaType<std::vector<char> >()->getUpType()->getTypeKind() == metapp::tkChar);

	REQUIRE(metapp::getMetaType<void (int *)>()->getTypeKind() == metapp::tkFunction);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpTypeCount() == 2);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpType(0)->getTypeKind() == metapp::tkVoid);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpType(1)->getTypeKind() == metapp::tkPointer);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpType(1)->getUpType()->getTypeKind() == metapp::tkInt);
}


} // namespace
