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
	female
};

namespace metapp {

template <>
struct DeclareMetaType <EnumHuman> : public DeclareMetaTypeBase <EnumHuman>
{
	static constexpr TypeKind typeKind = 2000;

	static const MetaEnum * getMetaEnum() {
		static const MetaEnum metaEnum([](MetaEnum & me) {
			me.addValue("male", EnumHuman::male);
			me.addValue("female", EnumHuman::female);
		});
		return &metaEnum;
	}
};

} // metapp


TEST_CASE("metatypes, enum class")
{
	auto metaType = metapp::getMetaType<EnumHuman>();
	REQUIRE(metaType->getTypeKind() == 2000);
	REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkChar);
	REQUIRE(metaType->getMetaEnum() != nullptr);
	REQUIRE(EnumHuman(metaType->getMetaEnum()->getValue("male")) == EnumHuman::male);
	REQUIRE(EnumHuman(metaType->getMetaEnum()->getValue("female")) == EnumHuman::female);
}
