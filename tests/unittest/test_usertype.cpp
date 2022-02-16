#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct MyClass
{
	int value;
};

namespace metapp {

template <>
struct DeclareMetaType <MyClass> : public DeclareMetaTypeBase<MyClass>
{
	static constexpr TypeKind typeKind = 2000;
};

template <>
struct DeclareMetaType <MyClass *> : public DeclareMetaTypeBase<MyClass *>
{
	static constexpr TypeKind typeKind = 2001;
};

} // metapp

TEST_CASE("User type")
{
	MyClass obj{ 38 };
	
	metapp::Variant v(obj);
	REQUIRE(v.getTypeKind() == 2000);
	REQUIRE(v.get<MyClass>().value == 38);
	REQUIRE(v.get<MyClass &>().value == 38);
	REQUIRE(v.get<const MyClass &>().value == 38);

	metapp::Variant v2(&obj);
	REQUIRE(v2.getTypeKind() == 2001);
	REQUIRE(v2.get<MyClass *>() == &obj);
}


