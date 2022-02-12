#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("TypeKind and get, void")
{
	SECTION("void") {
		REQUIRE(metapp::Variant((void *)0).getTypeKind() == metapp::tkPointer);
	}

	SECTION("nullptr") {
		REQUIRE(metapp::Variant(nullptr).getTypeKind() == metapp::tkPointer);
	}
}

TEST_CASE("TypeKind and get, fundamental")
{
	SECTION("bool") {
		REQUIRE(metapp::Variant(true).getTypeKind() == metapp::tkBool);
		REQUIRE(metapp::Variant(true).get<bool>() == true);
	}

	SECTION("char") {
		REQUIRE(metapp::Variant('c').getTypeKind() == metapp::tkChar);
		REQUIRE(metapp::Variant('c').get<char>() == 'c');
	}

	SECTION("signed char") {
		REQUIRE(metapp::Variant((signed char)127).getTypeKind() == metapp::tkSignedChar);
		REQUIRE(metapp::Variant((signed char)127).get<signed char>() == 127);
	}

	SECTION("unsigned char") {
		REQUIRE(metapp::Variant((unsigned char)255).getTypeKind() == metapp::tkUnsignedChar);
		REQUIRE(metapp::Variant((unsigned char)255).get<unsigned char>() == 255);
	}

	SECTION("wchar_t") {
		REQUIRE(metapp::Variant(L'c').getTypeKind() == metapp::tkWideChar);
		REQUIRE(metapp::Variant(L'c').get<wchar_t>() == L'c');
		REQUIRE(metapp::Variant((wchar_t)0x1cde).getTypeKind() == metapp::tkWideChar);
		REQUIRE(metapp::Variant((wchar_t)0x1cde).get<wchar_t>() == (wchar_t)0x1cde);
	}

	SECTION("short") {
		REQUIRE(metapp::Variant((short)5).getTypeKind() == metapp::tkShort);
		REQUIRE(metapp::Variant((short)5).get<short>() == 5);
		REQUIRE(metapp::Variant((short)0x6789).getTypeKind() == metapp::tkShort);
		REQUIRE(metapp::Variant((short)0x6789).get<short>() == 0x6789);
	}

	SECTION("unsigned short") {
		REQUIRE(metapp::Variant((unsigned short)5).getTypeKind() == metapp::tkUnsignedShort);
		REQUIRE(metapp::Variant((unsigned short)5).get<unsigned short>() == 5);
		REQUIRE(metapp::Variant((unsigned short)0xf987).getTypeKind() == metapp::tkUnsignedShort);
		REQUIRE(metapp::Variant((unsigned short)0xf987).get<unsigned short>() == 0xf987);
	}

	SECTION("int") {
		REQUIRE(metapp::Variant(98).getTypeKind() == metapp::tkInt);
		REQUIRE(metapp::Variant(98).get<int>() == 98);
		REQUIRE(metapp::Variant(std::numeric_limits<int>::max()).getTypeKind() == metapp::tkInt);
		REQUIRE(metapp::Variant(std::numeric_limits<int>::max()).get<int>() == std::numeric_limits<int>::max());
		REQUIRE(metapp::Variant(std::numeric_limits<int>::min()).getTypeKind() == metapp::tkInt);
		REQUIRE(metapp::Variant(std::numeric_limits<int>::min()).get<int>() == std::numeric_limits<int>::min());
	}

	SECTION("unsigned int") {
		REQUIRE(metapp::Variant(98u).getTypeKind() == metapp::tkUnsignedInt);
		REQUIRE(metapp::Variant(98u).get<unsigned int>() == 98u);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::max()).getTypeKind() == metapp::tkUnsignedInt);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::max()).get<unsigned int>() == std::numeric_limits<unsigned int>::max());
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::min()).getTypeKind() == metapp::tkUnsignedInt);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::min()).get<unsigned int>() == std::numeric_limits<unsigned int>::min());
	}

	SECTION("long") {
		REQUIRE(metapp::Variant(9L).getTypeKind() == metapp::tkLong);
		REQUIRE(metapp::Variant(9L).get<long>() == 9L);
		REQUIRE(metapp::Variant(std::numeric_limits<long>::max()).getTypeKind() == metapp::tkLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long>::max()).get<long>() == std::numeric_limits<long>::max());
		REQUIRE(metapp::Variant(std::numeric_limits<long>::min()).getTypeKind() == metapp::tkLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long>::min()).get<long>() == std::numeric_limits<long>::min());
	}

	SECTION("unsigned long") {
		REQUIRE(metapp::Variant(9UL).getTypeKind() == metapp::tkUnsignedLong);
		REQUIRE(metapp::Variant(9UL).get<unsigned long>() == 9UL);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::max()).getTypeKind() == metapp::tkUnsignedLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::max()).get<unsigned long>() == std::numeric_limits<unsigned long>::max());
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::min()).getTypeKind() == metapp::tkUnsignedLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::min()).get<unsigned long>() == std::numeric_limits<unsigned long>::min());
	}

	SECTION("long long") {
		REQUIRE(metapp::Variant(9LL).getTypeKind() == metapp::tkLongLong);
		REQUIRE(metapp::Variant(9LL).get<long long>() == 9LL);
		REQUIRE(metapp::Variant(std::numeric_limits<long long>::max()).getTypeKind() == metapp::tkLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long long>::max()).get<long long>() == std::numeric_limits<long long>::max());
		REQUIRE(metapp::Variant(std::numeric_limits<long long>::min()).getTypeKind() == metapp::tkLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long long>::min()).get<long long>() == std::numeric_limits<long long>::min());
	}

	SECTION("unsigned long long") {
		REQUIRE(metapp::Variant(9ULL).getTypeKind() == metapp::tkUnsignedLongLong);
		REQUIRE(metapp::Variant(9ULL).get<unsigned long long>() == 9ULL);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::max()).getTypeKind() == metapp::tkUnsignedLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::max()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::max());
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::min()).getTypeKind() == metapp::tkUnsignedLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::min()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::min());
	}

	SECTION("float") {
		REQUIRE(metapp::Variant(123.5678f).getTypeKind() == metapp::tkFloat);
		REQUIRE(metapp::Variant(123.5678f).get<float>() == Approx(123.5678f));
		REQUIRE(metapp::Variant(std::numeric_limits<float>::max()).getTypeKind() == metapp::tkFloat);
		REQUIRE(metapp::Variant(std::numeric_limits<float>::max()).get<float>() == Approx(std::numeric_limits<float>::max()));
		REQUIRE(metapp::Variant(std::numeric_limits<float>::min()).getTypeKind() == metapp::tkFloat);
		REQUIRE(metapp::Variant(std::numeric_limits<float>::min()).get<float>() == Approx(std::numeric_limits<float>::min()));
	}

	SECTION("double") {
		REQUIRE(metapp::Variant(123.5678).getTypeKind() == metapp::tkDouble);
		REQUIRE(metapp::Variant(123.5678).get<double>() == Approx(123.5678));
		REQUIRE(metapp::Variant(std::numeric_limits<double>::max()).getTypeKind() == metapp::tkDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<double>::max()).get<double>() == Approx(std::numeric_limits<double>::max()));
		REQUIRE(metapp::Variant(std::numeric_limits<double>::min()).getTypeKind() == metapp::tkDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<double>::min()).get<double>() == Approx(std::numeric_limits<double>::min()));
	}

	SECTION("long double") {
		REQUIRE(metapp::Variant((long double)123.5678).getTypeKind() == metapp::tkLongDouble);
		REQUIRE(metapp::Variant((long double)123.5678).get<long double>() == Approx((long double)123.5678));
		REQUIRE(metapp::Variant(std::numeric_limits<long double>::max()).getTypeKind() == metapp::tkLongDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<long double>::max()).get<long double>() == Approx(std::numeric_limits<long double>::max()));
		REQUIRE(metapp::Variant(std::numeric_limits<long double>::min()).getTypeKind() == metapp::tkLongDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<long double>::min()).get<long double>() == Approx(std::numeric_limits<long double>::min()));
	}

}

TEST_CASE("TypeKind and get, string")
{
	SECTION("std::string") {
		REQUIRE(metapp::Variant(std::string("abc")).getTypeKind() == metapp::tkString);
		REQUIRE(metapp::Variant(std::string("abc")).get<std::string>() == "abc");
		std::string s("def");
		REQUIRE(metapp::Variant(s).getTypeKind() == metapp::tkString);
		REQUIRE(metapp::Variant(s).get<std::string>() == s);
	}

	SECTION("std::wstring") {
		REQUIRE(metapp::Variant(std::wstring(L"abc")).getTypeKind() == metapp::tkWideString);
		REQUIRE(metapp::Variant(std::wstring(L"abc")).get<std::wstring>() == L"abc");
		std::wstring ws(L"def");
		REQUIRE(metapp::Variant(ws).getTypeKind() == metapp::tkWideString);
		REQUIRE(metapp::Variant(ws).get<std::wstring>() == ws);
	}
}

TEST_CASE("TypeKind and get, pointer")
{
	SECTION("void *") {
		metapp::Variant v((void *)0);
		REQUIRE(v.getTypeKind() == metapp::tkPointer);
		using namespace metapp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkVoid });
	}

	SECTION("const volatile void *") {
		metapp::Variant v((const volatile void *)0);
		REQUIRE(v.getTypeKind() == metapp::tkPointer);
		using namespace metapp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkVoid });
		REQUIRE(v.getMetaType()->getUpType()->isConst());
		REQUIRE(v.getMetaType()->getUpType()->isVolatile());
	}

	SECTION("int **") {
		metapp::Variant v((int **)0);
		REQUIRE(v.getTypeKind() == metapp::tkPointer);
		REQUIRE(v.canGet<int **>(true));
		REQUIRE(v.canGet<int **>(false));
		REQUIRE(! v.canGet<int *>(true));
		using namespace metapp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkPointer, tkPointer, tkInt });
	}

}

TEST_CASE("TypeKind and get, std::shared_ptr")
{
	SECTION("std::shared_ptr<int>") {
		std::shared_ptr<int> sp = std::make_shared<int>(38);
		metapp::Variant v(sp);
		REQUIRE(v.getTypeKind() == metapp::tkSharedPtr);
		REQUIRE(v.canGet<std::shared_ptr<int> >());
		REQUIRE(*(v.get<std::shared_ptr<int> >()) == 38);
		using namespace metapp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkSharedPtr, tkInt });
	}
}

TEST_CASE("TypeKind and get, std::vector")
{
	SECTION("std::vector<int>") {
		std::vector<int> vec{5};
		metapp::Variant v(vec);
		REQUIRE(v.getTypeKind() == metapp::tkVector);
		REQUIRE(v.get<std::vector<int>>()[0] == 5);
		using namespace metapp;
		const auto varTypeList = getUpTypeVarTypes(v.getMetaType());
		REQUIRE(varTypeList == std::vector<TypeKind>{ tkVector, tkInt });
	}
}


} // namespace
