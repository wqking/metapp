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
		REQUIRE(varpp::Variant((void *)0).getTypeKind() == varpp::tkPointer);
	}

	SECTION("nullptr") {
		REQUIRE(varpp::Variant(nullptr).getTypeKind() == varpp::tkPointer);
	}
}

TEST_CASE("TypeKind and get, fundamental")
{
	SECTION("bool") {
		REQUIRE(varpp::Variant(true).getTypeKind() == varpp::tkBool);
		REQUIRE(varpp::Variant(true).get<bool>() == true);
	}

	SECTION("char") {
		REQUIRE(varpp::Variant('c').getTypeKind() == varpp::tkChar);
		REQUIRE(varpp::Variant('c').get<char>() == 'c');
	}

	SECTION("signed char") {
		REQUIRE(varpp::Variant((signed char)127).getTypeKind() == varpp::tkSignedChar);
		REQUIRE(varpp::Variant((signed char)127).get<signed char>() == 127);
	}

	SECTION("unsigned char") {
		REQUIRE(varpp::Variant((unsigned char)255).getTypeKind() == varpp::tkUnsignedChar);
		REQUIRE(varpp::Variant((unsigned char)255).get<unsigned char>() == 255);
	}

	SECTION("wchar_t") {
		REQUIRE(varpp::Variant(L'c').getTypeKind() == varpp::tkWideChar);
		REQUIRE(varpp::Variant(L'c').get<wchar_t>() == L'c');
		REQUIRE(varpp::Variant((wchar_t)0x1cde).getTypeKind() == varpp::tkWideChar);
		REQUIRE(varpp::Variant((wchar_t)0x1cde).get<wchar_t>() == (wchar_t)0x1cde);
	}

	SECTION("short") {
		REQUIRE(varpp::Variant((short)5).getTypeKind() == varpp::tkShort);
		REQUIRE(varpp::Variant((short)5).get<short>() == 5);
		REQUIRE(varpp::Variant((short)0x6789).getTypeKind() == varpp::tkShort);
		REQUIRE(varpp::Variant((short)0x6789).get<short>() == 0x6789);
	}

	SECTION("unsigned short") {
		REQUIRE(varpp::Variant((unsigned short)5).getTypeKind() == varpp::tkUnsignedShort);
		REQUIRE(varpp::Variant((unsigned short)5).get<unsigned short>() == 5);
		REQUIRE(varpp::Variant((unsigned short)0xf987).getTypeKind() == varpp::tkUnsignedShort);
		REQUIRE(varpp::Variant((unsigned short)0xf987).get<unsigned short>() == 0xf987);
	}

	SECTION("int") {
		REQUIRE(varpp::Variant(98).getTypeKind() == varpp::tkInt);
		REQUIRE(varpp::Variant(98).get<int>() == 98);
		REQUIRE(varpp::Variant(std::numeric_limits<int>::max()).getTypeKind() == varpp::tkInt);
		REQUIRE(varpp::Variant(std::numeric_limits<int>::max()).get<int>() == std::numeric_limits<int>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<int>::min()).getTypeKind() == varpp::tkInt);
		REQUIRE(varpp::Variant(std::numeric_limits<int>::min()).get<int>() == std::numeric_limits<int>::min());
	}

	SECTION("unsigned int") {
		REQUIRE(varpp::Variant(98u).getTypeKind() == varpp::tkUnsignedInt);
		REQUIRE(varpp::Variant(98u).get<unsigned int>() == 98u);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::max()).getTypeKind() == varpp::tkUnsignedInt);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::max()).get<unsigned int>() == std::numeric_limits<unsigned int>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::min()).getTypeKind() == varpp::tkUnsignedInt);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned int>::min()).get<unsigned int>() == std::numeric_limits<unsigned int>::min());
	}

	SECTION("long") {
		REQUIRE(varpp::Variant(9L).getTypeKind() == varpp::tkLong);
		REQUIRE(varpp::Variant(9L).get<long>() == 9L);
		REQUIRE(varpp::Variant(std::numeric_limits<long>::max()).getTypeKind() == varpp::tkLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long>::max()).get<long>() == std::numeric_limits<long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<long>::min()).getTypeKind() == varpp::tkLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long>::min()).get<long>() == std::numeric_limits<long>::min());
	}

	SECTION("unsigned long") {
		REQUIRE(varpp::Variant(9UL).getTypeKind() == varpp::tkUnsignedLong);
		REQUIRE(varpp::Variant(9UL).get<unsigned long>() == 9UL);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::max()).getTypeKind() == varpp::tkUnsignedLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::max()).get<unsigned long>() == std::numeric_limits<unsigned long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::min()).getTypeKind() == varpp::tkUnsignedLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long>::min()).get<unsigned long>() == std::numeric_limits<unsigned long>::min());
	}

	SECTION("long long") {
		REQUIRE(varpp::Variant(9LL).getTypeKind() == varpp::tkLongLong);
		REQUIRE(varpp::Variant(9LL).get<long long>() == 9LL);
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::max()).getTypeKind() == varpp::tkLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::max()).get<long long>() == std::numeric_limits<long long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::min()).getTypeKind() == varpp::tkLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<long long>::min()).get<long long>() == std::numeric_limits<long long>::min());
	}

	SECTION("unsigned long long") {
		REQUIRE(varpp::Variant(9ULL).getTypeKind() == varpp::tkUnsignedLongLong);
		REQUIRE(varpp::Variant(9ULL).get<unsigned long long>() == 9ULL);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::max()).getTypeKind() == varpp::tkUnsignedLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::max()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::max());
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::min()).getTypeKind() == varpp::tkUnsignedLongLong);
		REQUIRE(varpp::Variant(std::numeric_limits<unsigned long long>::min()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::min());
	}

	SECTION("float") {
		REQUIRE(varpp::Variant(123.5678f).getTypeKind() == varpp::tkFloat);
		REQUIRE(varpp::Variant(123.5678f).get<float>() == Approx(123.5678f));
		REQUIRE(varpp::Variant(std::numeric_limits<float>::max()).getTypeKind() == varpp::tkFloat);
		REQUIRE(varpp::Variant(std::numeric_limits<float>::max()).get<float>() == Approx(std::numeric_limits<float>::max()));
		REQUIRE(varpp::Variant(std::numeric_limits<float>::min()).getTypeKind() == varpp::tkFloat);
		REQUIRE(varpp::Variant(std::numeric_limits<float>::min()).get<float>() == Approx(std::numeric_limits<float>::min()));
	}

	SECTION("double") {
		REQUIRE(varpp::Variant(123.5678).getTypeKind() == varpp::tkDouble);
		REQUIRE(varpp::Variant(123.5678).get<double>() == Approx(123.5678));
		REQUIRE(varpp::Variant(std::numeric_limits<double>::max()).getTypeKind() == varpp::tkDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<double>::max()).get<double>() == Approx(std::numeric_limits<double>::max()));
		REQUIRE(varpp::Variant(std::numeric_limits<double>::min()).getTypeKind() == varpp::tkDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<double>::min()).get<double>() == Approx(std::numeric_limits<double>::min()));
	}

	SECTION("long double") {
		REQUIRE(varpp::Variant((long double)123.5678).getTypeKind() == varpp::tkLongDouble);
		REQUIRE(varpp::Variant((long double)123.5678).get<long double>() == Approx((long double)123.5678));
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::max()).getTypeKind() == varpp::tkLongDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::max()).get<long double>() == Approx(std::numeric_limits<long double>::max()));
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::min()).getTypeKind() == varpp::tkLongDouble);
		REQUIRE(varpp::Variant(std::numeric_limits<long double>::min()).get<long double>() == Approx(std::numeric_limits<long double>::min()));
	}

}

TEST_CASE("TypeKind and get, string")
{
	SECTION("std::string") {
		REQUIRE(varpp::Variant(std::string("abc")).getTypeKind() == varpp::tkString);
		REQUIRE(varpp::Variant(std::string("abc")).get<std::string>() == "abc");
		std::string s("def");
		REQUIRE(varpp::Variant(s).getTypeKind() == varpp::tkString);
		REQUIRE(varpp::Variant(s).get<std::string>() == s);
	}

	SECTION("std::wstring") {
		REQUIRE(varpp::Variant(std::wstring(L"abc")).getTypeKind() == varpp::tkWideString);
		REQUIRE(varpp::Variant(std::wstring(L"abc")).get<std::wstring>() == L"abc");
		std::wstring ws(L"def");
		REQUIRE(varpp::Variant(ws).getTypeKind() == varpp::tkWideString);
		REQUIRE(varpp::Variant(ws).get<std::wstring>() == ws);
	}
}

TEST_CASE("TypeKind and get, pointer")
{
	SECTION("void *") {
		varpp::Variant v((void *)0);
		REQUIRE(v.getTypeKind() == varpp::tkPointer);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkVoid });
	}

	SECTION("const volatile void *") {
		varpp::Variant v((const volatile void *)0);
		REQUIRE(v.getTypeKind() == varpp::tkPointer);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkVoid });
	}

	SECTION("int **") {
		varpp::Variant v((int **)0);
		REQUIRE(v.getTypeKind() == varpp::tkPointer);
		REQUIRE(v.canGet<int **>(true));
		REQUIRE(v.canGet<int **>(false));
		REQUIRE(! v.canGet<int *>(true));
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkPointer, tkInt });
	}

}

TEST_CASE("TypeKind and get, std::shared_ptr")
{
	SECTION("std::shared_ptr<int>") {
		std::shared_ptr<int> sp = std::make_shared<int>(38);
		varpp::Variant v(sp);
		REQUIRE(v.getTypeKind() == varpp::tkSharedPtr);
		REQUIRE(v.canGet<std::shared_ptr<int> >());
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
		REQUIRE(v.getTypeKind() == varpp::tkVector);
		REQUIRE(v.get<std::vector<int>>()[0] == 5);
		using namespace varpp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkVector, tkInt });
	}
}


} // namespace
