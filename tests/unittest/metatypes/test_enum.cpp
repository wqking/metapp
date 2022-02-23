#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

enum EnumAnimal {
	cat,
	dog
};

TEST_CASE("metatypes, enum")
{
	auto metaType = metapp::getMetaType<EnumAnimal>();
	REQUIRE(metaType->getTypeKind() == metapp::tkEnum);
	// the underlying type may be different, on MSVC it's tkInt, on GCC it's tkUnsignedInt
	//REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkInt);
}

enum class EnumHuman : char {
	male,
	femail
};

TEST_CASE("metatypes, enum class")
{
	auto metaType = metapp::getMetaType<EnumHuman>();
	REQUIRE(metaType->getTypeKind() == metapp::tkEnum);
	REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkChar);
}

