#include "test.h"

#include <iostream>
#include "varpp/variant.h"
#include "varpp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct MyClass
{
	int value;
};

namespace varpp {

template <>
struct DeclareMetaType <MyClass> : public DeclareObjectMetaType<MyClass>
{
	static constexpr TypeKind typeKind = 2000;
};

template <>
struct DeclareMetaType <MyClass *> : public DeclareObjectMetaType<MyClass *>
{
	static constexpr TypeKind typeKind = 2001;
};

} // varpp

namespace {

TEST_CASE("User type")
{
	MyClass obj{ 38 };
	
	varpp::Variant v(obj);
	REQUIRE(v.getTypeKind() == 2000);
	REQUIRE(v.get<MyClass>().value == 38);
	REQUIRE(v.get<MyClass &>().value == 38);
	REQUIRE(v.get<const MyClass &>().value == 38);

	varpp::Variant v2(&obj);
	REQUIRE(v2.getTypeKind() == 2001);
	REQUIRE(v2.get<MyClass *>() == &obj);
}



} // namespace
