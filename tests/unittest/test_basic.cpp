#include "test.h"

#include "varpp/variant.h"

TEST_CASE("aaa")
{
	{
		varpp::Variant v(38);
		REQUIRE(v.getAs<int>() == 38);
		REQUIRE(v.getVarType() == varpp::vtSignedInt);
		REQUIRE(! v.isPointer());
	}
	{
		int n = 5;
		int * p = &n;
		varpp::Variant v(p);
		REQUIRE(v.getAs<int *>() == p);
		REQUIRE(*v.getAs<int *>() == 5);
		REQUIRE(v.getVarType() == varpp::vtSignedInt);
		REQUIRE(v.isPointer());
	}
	{
		int n = 5;
		int & p = n;
		varpp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.getAs<int &>() == 5);
		REQUIRE(v.getVarType() == varpp::vtSignedInt);
		REQUIRE(v.isReference());
	}
	{
		varpp::Variant v("abc");
		REQUIRE(v.getAs<const char *>() == std::string("abc"));
		REQUIRE(v.getVarType() == varpp::vtChar);
		REQUIRE(v.isPointer());
	}

}
