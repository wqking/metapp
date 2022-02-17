#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct UseTypeMyClass
{
	int value;
};

namespace metapp {

template <>
struct DeclareMetaType <UseTypeMyClass> : public DeclareMetaTypeBase <UseTypeMyClass>
{
	static constexpr TypeKind typeKind = 2000;

	static const char * getName() {
		return "UseTypeMyClass";
	}

};

template <>
struct DeclareMetaType <UseTypeMyClass *> : public DeclareMetaTypeBase <UseTypeMyClass *>
{
	static constexpr TypeKind typeKind = 2001;
};

} // metapp

TEST_CASE("User type")
{
	UseTypeMyClass obj{ 38 };
	
	metapp::Variant v(obj);
	REQUIRE(v.getTypeKind() == 2000);
	REQUIRE(v.get<UseTypeMyClass>().value == 38);
	REQUIRE(v.get<UseTypeMyClass &>().value == 38);
	REQUIRE(v.get<const UseTypeMyClass &>().value == 38);

	metapp::Variant v2(&obj);
	REQUIRE(v2.getTypeKind() == 2001);
	REQUIRE(v2.get<UseTypeMyClass *>() == &obj);
}


