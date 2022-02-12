#include "test.h"

#include <iostream>
#include "varpp/variant.h"
#include "varpp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("TypeKind and get, void")
{
	SECTION("void") {
		REQUIRE(varpp::Variant((void *)0).getVarType() == varpp::tkPointer);
	}

	SECTION("nullptr") {
		REQUIRE(varpp::Variant(nullptr).getVarType() == varpp::tkPointer);
	}
}

TEST_CASE("TypeKind and get, fundamental")
{
	SECTION("bool") {
		REQUIRE(varpp::Variant(true).getVarType() == varpp::tkBool);
		REQUIRE(varpp::Variant(true).get<bool>() == true);
	}

	SECTION("char") {
		REQUIRE(varpp::Variant('c').getVarType() == varpp::tkChar);
		REQUIRE(varpp::Variant('c').get<char>() == 'c');
	}

	SECTION("signed char") {
		REQUIRE(varpp::Variant((signed char)127).getVarType() == varpp::tkSignedChar);
		REQUIRE(varpp::Variant((signed char)127).get<signed char>() == 127);
	}

	SECTION("unsigned char") {
		REQUIRE(varpp::Variant((unsigned char)255).getVarType() == varpp::tkUnsignedChar);
		REQUIRE(varpp::Variant((unsigned char)255).get<unsigned char>() == 255);
	}

	SECTION("wchar_t") {
		REQUIRE(varpp::Variant(L'c').getVarType() == varpp::tkWideChar);
		REQUIRE(varpp::Variant(L'c').get<wchar_t>() == L'c');
		REQUIRE(varpp::Variant((wchar_t)0x1cde).getVarType() == varpp::tkWideChar);
		REQUIRE(varpp::Variant((wchar_t)0x1cde).get<wchar_t>() == (wchar_t)0x1cde);
	}

	SECTION("short") {
		REQUIRE(varpp::Variant((short)5).getVarType() == varpp::tkShort);
		REQUIRE(varpp::Variant((short)5).get<short>() == 5);
		REQUIRE(varpp::Variant((short)0x6789).getVarType() == varpp::tkShort);
		REQUIRE(varpp::Variant((short)0x6789).get<short>() == 0x6789);
	}

	SECTION("unsigned short") {
		REQUIRE(varpp::Variant((unsigned short)5).getVarType() == varpp::tkUnsignedShort);
		REQUIRE(varpp::Variant((unsigned short)5).get<unsigned short>() == 5);
		REQUIRE(varpp::Variant((unsigned short)0xf987).getVarType() == varpp::tkUnsignedShort);
		REQUIRE(varpp::Variant((unsigned short)0xf987).get<unsigned short>() == 0xf987);
	}

	SECTION("int") {
		REQUIRE(varpp::Variant(98).getVarType() == varpp::tkInt);
		REQUIRE(varpp::Variant(98).get<int>() == 98);
		REQUIRE(varpp::Variant(std::numeric_limits<int>::max()).getVarType() == varpp::tkInt);
		REQUIRE(varpp::Variant(std::numeric_limits<int>::max()).get<int>() == std::numeric_limits<int>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<int>::min()).getVarType() == varpp::tkInt);
		REQUIRE(varpp::Variant(std::numeric_limits<int>::min()).get<int>() == std::numeric_limits<int>::min());
	}

	SECTION("unsigned int") {
		REQUIRE(varpp::Variant(98u).getVarType() == varpp::tkUnsignedInt);
		REQUIRE(varpp::Variant(98u).get<unsigned int>() == 98u);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::max()).getVarType() == varpp::tkUnsignedInt);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::max()).get<unsigned int>() == std::numeric_limits<unsigned int>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::min()).getVarType() == varpp::tkUnsignedInt);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::min()).get<unsigned int>() == std::numeric_limits<unsigned int>::min());
	}

	SECTION("long") {
		REQUIRE(varpp::Variant(9L).getVarType() == varpp::tkLong);
		REQUIRE(varpp::Variant(9L).get<long>() == 9L);
		REQUIRE(varpp::Variant(std::numeric_limits<long>::max()).getVarType() == varpp::tkLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long>::max()).get<long>() == std::numeric_limits<long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<long>::min()).getVarType() == varpp::tkLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long>::min()).get<long>() == std::numeric_limits<long>::min());
	}

	SECTION("unsigned long") {
		REQUIRE(varpp::Variant(9UL).getVarType() == varpp::tkUnsignedLong);
		REQUIRE(varpp::Variant(9UL).get<unsigned long>() == 9UL);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::max()).getVarType() == varpp::tkUnsignedLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::max()).get<unsigned long>() == std::numeric_limits<unsigned long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::min()).getVarType() == varpp::tkUnsignedLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::min()).get<unsigned long>() == std::numeric_limits<unsigned long>::min());
	}

	SECTION("long long") {
		REQUIRE(varpp::Variant(9LL).getVarType() == varpp::tkLongLong);
		REQUIRE(varpp::Variant(9LL).get<long long>() == 9LL);
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::max()).getVarType() == varpp::tkLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::max()).get<long long>() == std::numeric_limits<long long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::min()).getVarType() == varpp::tkLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::min()).get<long long>() == std::numeric_limits<long long>::min());
	}

	SECTION("unsigned long long") {
		REQUIRE(varpp::Variant(9ULL).getVarType() == varpp::tkUnsignedLongLong);
		REQUIRE(varpp::Variant(9ULL).get<unsigned long long>() == 9ULL);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::max()).getVarType() == varpp::tkUnsignedLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::max()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::min()).getVarType() == varpp::tkUnsignedLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::min()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::min());
	}

	SECTION("float") {
		REQUIRE(varpp::Variant(123.5678f).getVarType() == varpp::tkFloat);
		REQUIRE(varpp::Variant(123.5678f).get<float>() == Approx(123.5678f));
		REQUIRE(varpp::Variant(std::numeric_limits<float>::max()).getVarType() == varpp::tkFloat);
		REQUIRE(varpp::Variant(std::numeric_limits<float>::max()).get<float>() == Approx(std::numeric_limits<float>::max()));
		REQUIRE(varpp::Variant(std::numeric_limits<float>::min()).getVarType() == varpp::tkFloat);
		REQUIRE(varpp::Variant(std::numeric_limits<float>::min()).get<float>() == Approx(std::numeric_limits<float>::min()));
	}

	SECTION("double") {
		REQUIRE(varpp::Variant(123.5678).getVarType() == varpp::tkDouble);
		REQUIRE(varpp::Variant(123.5678).get<double>() == Approx(123.5678));
		REQUIRE(varpp::Variant(std::numeric_limits<double>::max()).getVarType() == varpp::tkDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<double>::max()).get<double>() == Approx(std::numeric_limits<double>::max()));
		REQUIRE(varpp::Variant(std::numeric_limits<double>::min()).getVarType() == varpp::tkDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<double>::min()).get<double>() == Approx(std::numeric_limits<double>::min()));
	}

	SECTION("long double") {
		REQUIRE(varpp::Variant((long double)123.5678).getVarType() == varpp::tkLongDouble);
		REQUIRE(varpp::Variant((long double)123.5678).get<long double>() == Approx((long double)123.5678));
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::max()).getVarType() == varpp::tkLongDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::max()).get<long double>() == Approx(std::numeric_limits<long double>::max()));
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::min()).getVarType() == varpp::tkLongDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::min()).get<long double>() == Approx(std::numeric_limits<long double>::min()));
	}

}

TEST_CASE("TypeKind and get, string")
{
	SECTION("std::string") {
		REQUIRE(varpp::Variant(std::string("abc")).getVarType() == varpp::tkString);
		REQUIRE(varpp::Variant(std::string("abc")).get<std::string>() == "abc");
		std::string s("def");
		REQUIRE(varpp::Variant(s).getVarType() == varpp::tkString);
		REQUIRE(varpp::Variant(s).get<std::string>() == s);
	}

	SECTION("std::wstring") {
		REQUIRE(varpp::Variant(std::wstring(L"abc")).getVarType() == varpp::tkWideString);
		REQUIRE(varpp::Variant(std::wstring(L"abc")).get<std::wstring>() == L"abc");
		std::wstring ws(L"def");
		REQUIRE(varpp::Variant(ws).getVarType() == varpp::tkWideString);
		REQUIRE(varpp::Variant(ws).get<std::wstring>() == ws);
	}
}

TEST_CASE("TypeKind and get, pointer")
{
	SECTION("void *") {
		varpp::Variant v((void *)0);
		REQUIRE(v.getVarType() == varpp::tkPointer);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkVoid });
	}

	SECTION("const volatile void *") {
		varpp::Variant v((const volatile void *)0);
		REQUIRE(v.getVarType() == varpp::tkPointer);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkVoid });
	}
}

TEST_CASE("TypeKind and get, std::shared_ptr")
{
	SECTION("std::shared_ptr<int>") {
		std::shared_ptr<int> sp = std::make_shared<int>(38);
		varpp::Variant v(sp);
		REQUIRE(v.getVarType() == varpp::tkSharedPtr);
		REQUIRE(*(v.get<std::shared_ptr<int> >()) == 38);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkSharedPtr, tkInt });
	}
}

TEST_CASE("TypeKind and get, std::vector")
{
	SECTION("std::vector<int>") {
		std::vector<int> vec{5};
		varpp::Variant v(vec);
		REQUIRE(v.getVarType() == varpp::tkVector);
		REQUIRE(v.get<std::vector<int>>()[0] == 5);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkVector, tkInt });
	}
}


} // namespace
