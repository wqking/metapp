#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

TEST_CASE("TypeKind, void")
{
	SECTION("void") {
		REQUIRE(metapp::getTypeKind(metapp::Variant((void *)0)) == metapp::tkPointer);
		REQUIRE(metapp::Variant((void *)0).getMetaType()->getUpType()->getName() == std::string("void"));
	}

	SECTION("nullptr") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(nullptr)) == metapp::tkPointer);
	}
}

TEST_CASE("TypeKind, fundamental")
{
	SECTION("bool") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(true)) == metapp::tkBool);
		REQUIRE(metapp::Variant(true).get<bool>() == true);
		REQUIRE(metapp::Variant(true).getMetaType()->getName() == std::string("bool"));
	}

	SECTION("char") {
		REQUIRE(metapp::getTypeKind(metapp::Variant('c')) == metapp::tkChar);
		REQUIRE(metapp::Variant('c').get<char>() == 'c');
		REQUIRE(metapp::Variant('c').getMetaType()->getName() == std::string("char"));
	}

	SECTION("signed char") {
		REQUIRE(metapp::getTypeKind(metapp::Variant((signed char)127)) == metapp::tkSignedChar);
		REQUIRE(metapp::Variant((signed char)127).get<signed char>() == 127);
		REQUIRE(metapp::Variant((signed char)127).getMetaType()->getName() == std::string("signed char"));
	}

	SECTION("unsigned char") {
		REQUIRE(metapp::getTypeKind(metapp::Variant((unsigned char)255)) == metapp::tkUnsignedChar);
		REQUIRE(metapp::Variant((unsigned char)255).get<unsigned char>() == 255);
		REQUIRE(metapp::Variant((unsigned char)255).getMetaType()->getName() == std::string("unsigned char"));
	}

	SECTION("wchar_t") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(L'c')) == metapp::tkWideChar);
		REQUIRE(metapp::Variant(L'c').get<wchar_t>() == L'c');
		REQUIRE(metapp::getTypeKind(metapp::Variant((wchar_t)0x1cde)) == metapp::tkWideChar);
		REQUIRE(metapp::Variant((wchar_t)0x1cde).get<wchar_t>() == (wchar_t)0x1cde);
		REQUIRE(metapp::Variant((wchar_t)0x1cde).getMetaType()->getName() == std::string("wchar_t"));
	}

	SECTION("short") {
		REQUIRE(metapp::getTypeKind(metapp::Variant((short)5)) == metapp::tkShort);
		REQUIRE(metapp::Variant((short)5).get<short>() == 5);
		REQUIRE(metapp::getTypeKind(metapp::Variant((short)0x6789)) == metapp::tkShort);
		REQUIRE(metapp::Variant((short)0x6789).get<short>() == 0x6789);
		REQUIRE(metapp::Variant((short)0x6789).getMetaType()->getName() == std::string("short"));
	}

	SECTION("unsigned short") {
		REQUIRE(metapp::getTypeKind(metapp::Variant((unsigned short)5)) == metapp::tkUnsignedShort);
		REQUIRE(metapp::Variant((unsigned short)5).get<unsigned short>() == 5);
		REQUIRE(metapp::getTypeKind(metapp::Variant((unsigned short)0xf987)) == metapp::tkUnsignedShort);
		REQUIRE(metapp::Variant((unsigned short)0xf987).get<unsigned short>() == 0xf987);
		REQUIRE(metapp::Variant((unsigned short)0xf987).getMetaType()->getName() == std::string("unsigned short"));
	}

	SECTION("int") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(98)) == metapp::tkInt);
		REQUIRE(metapp::Variant(98).get<int>() == 98);
		REQUIRE(metapp::Variant(98).getMetaType()->getName() == std::string("int"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<int>::max())) == metapp::tkInt);
		REQUIRE(metapp::Variant(std::numeric_limits<int>::max()).get<int>() == std::numeric_limits<int>::max());
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<int>::min())) == metapp::tkInt);
		REQUIRE(metapp::Variant(std::numeric_limits<int>::min()).get<int>() == std::numeric_limits<int>::min());
	}

	SECTION("unsigned int") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(98u)) == metapp::tkUnsignedInt);
		REQUIRE(metapp::Variant(98u).get<unsigned int>() == 98u);
		REQUIRE(metapp::Variant(98u).getMetaType()->getName() == std::string("unsigned int"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned int>::max())) == metapp::tkUnsignedInt);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::max()).get<unsigned int>() == std::numeric_limits<unsigned int>::max());
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned int>::min())) == metapp::tkUnsignedInt);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::min()).get<unsigned int>() == std::numeric_limits<unsigned int>::min());
	}

	SECTION("long") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(9L)) == metapp::tkLong);
		REQUIRE(metapp::Variant(9L).get<long>() == 9L);
		REQUIRE(metapp::Variant(9L).getMetaType()->getName() == std::string("long"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long>::max())) == metapp::tkLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long>::max()).get<long>() == std::numeric_limits<long>::max());
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long>::min())) == metapp::tkLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long>::min()).get<long>() == std::numeric_limits<long>::min());
	}

	SECTION("unsigned long") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(9UL)) == metapp::tkUnsignedLong);
		REQUIRE(metapp::Variant(9UL).get<unsigned long>() == 9UL);
		REQUIRE(metapp::Variant(9UL).getMetaType()->getName() == std::string("unsigned long"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long>::max())) == metapp::tkUnsignedLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::max()).get<unsigned long>() == std::numeric_limits<unsigned long>::max());
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long>::min())) == metapp::tkUnsignedLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::min()).get<unsigned long>() == std::numeric_limits<unsigned long>::min());
	}

	SECTION("long long") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(9LL)) == metapp::tkLongLong);
		REQUIRE(metapp::Variant(9LL).get<long long>() == 9LL);
		REQUIRE(metapp::Variant(9LL).getMetaType()->getName() == std::string("long long"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long long>::max())) == metapp::tkLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long long>::max()).get<long long>() == std::numeric_limits<long long>::max());
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long long>::min())) == metapp::tkLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<long long>::min()).get<long long>() == std::numeric_limits<long long>::min());
	}

	SECTION("unsigned long long") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(9ULL)) == metapp::tkUnsignedLongLong);
		REQUIRE(metapp::Variant(9ULL).get<unsigned long long>() == 9ULL);
		REQUIRE(metapp::Variant(9ULL).getMetaType()->getName() == std::string("unsigned long long"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long long>::max())) == metapp::tkUnsignedLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::max()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::max());
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long long>::min())) == metapp::tkUnsignedLongLong);
		REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::min()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::min());
	}

	SECTION("float") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(123.5678f)) == metapp::tkFloat);
		REQUIRE(metapp::Variant(123.5678f).get<float>() == Approx(123.5678f));
		REQUIRE(metapp::Variant(123.5678f).getMetaType()->getName() == std::string("float"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<float>::max())) == metapp::tkFloat);
		REQUIRE(metapp::Variant(std::numeric_limits<float>::max()).get<float>() == Approx(std::numeric_limits<float>::max()));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<float>::min())) == metapp::tkFloat);
		REQUIRE(metapp::Variant(std::numeric_limits<float>::min()).get<float>() == Approx(std::numeric_limits<float>::min()));
	}

	SECTION("double") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(123.5678)) == metapp::tkDouble);
		REQUIRE(metapp::Variant(123.5678).get<double>() == Approx(123.5678));
		REQUIRE(metapp::Variant(123.5678).getMetaType()->getName() == std::string("double"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<double>::max())) == metapp::tkDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<double>::max()).get<double>() == Approx(std::numeric_limits<double>::max()));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<double>::min())) == metapp::tkDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<double>::min()).get<double>() == Approx(std::numeric_limits<double>::min()));
	}

	SECTION("long double") {
		REQUIRE(metapp::getTypeKind(metapp::Variant((long double)123.5678)) == metapp::tkLongDouble);
		REQUIRE(metapp::Variant((long double)123.5678).get<long double>() == Approx((long double)123.5678));
		REQUIRE(metapp::Variant((long double)123.5678).getMetaType()->getName() == std::string("long double"));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long double>::max())) == metapp::tkLongDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<long double>::max()).get<long double>() == Approx(std::numeric_limits<long double>::max()));
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long double>::min())) == metapp::tkLongDouble);
		REQUIRE(metapp::Variant(std::numeric_limits<long double>::min()).get<long double>() == Approx(std::numeric_limits<long double>::min()));
	}

}

TEST_CASE("TypeKind, string")
{
	SECTION("std::string") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::string("abc"))) == metapp::tkString);
		REQUIRE(metapp::Variant(std::string("abc")).get<std::string>() == "abc");
		std::string s("def");
		REQUIRE(metapp::getTypeKind(metapp::Variant(s)) == metapp::tkString);
		REQUIRE(metapp::Variant(s).get<std::string>() == s);
	}

	SECTION("std::wstring") {
		REQUIRE(metapp::getTypeKind(metapp::Variant(std::wstring(L"abc"))) == metapp::tkWideString);
		REQUIRE(metapp::Variant(std::wstring(L"abc")).get<std::wstring>() == L"abc");
		std::wstring ws(L"def");
		REQUIRE(metapp::getTypeKind(metapp::Variant(ws)) == metapp::tkWideString);
		REQUIRE(metapp::Variant(ws).get<std::wstring>() == ws);
	}
}

TEST_CASE("TypeKind, pointer")
{
	SECTION("void *") {
		metapp::Variant v((void *)0);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		using namespace metapp;
		REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
	}

	SECTION("const volatile void *") {
		metapp::Variant v((const volatile void *)0);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		using namespace metapp;
		REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
		REQUIRE(v.getMetaType()->getUpType()->isConst());
		REQUIRE(v.getMetaType()->getUpType()->isVolatile());
	}

	SECTION("int **") {
		metapp::Variant v((int **)0);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		REQUIRE(v.canGet<int **>(true));
		REQUIRE(v.canGet<int **>(false));
		REQUIRE(! v.canGet<int *>(true));
		using namespace metapp;
		REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkInt }));
	}

}

TEST_CASE("TypeKind, void ***")
{
	void *** p = nullptr;
	metapp::Variant v(p);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkPointer, tkVoid }));
}

TEST_CASE("TypeKind, std::shared_ptr")
{
	SECTION("std::shared_ptr<int>") {
		std::shared_ptr<int> sp = std::make_shared<int>(38);
		metapp::Variant v(sp);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkSharedPtr);
		REQUIRE(v.canGet<std::shared_ptr<int> >());
		REQUIRE(*(v.get<std::shared_ptr<int> >()) == 38);
		using namespace metapp;
		REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkSharedPtr, tkInt }));
	}
}

TEST_CASE("TypeKind, std::vector")
{
	SECTION("std::vector<int>") {
		std::vector<int> vec{ 5 };
		metapp::Variant v(vec);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkVector);
		REQUIRE(v.get<std::vector<int>>()[0] == 5);
		v.get<std::vector<int> &>().push_back(10);
		REQUIRE(v.get<std::vector<int>>().back() == 10);
		using namespace metapp;
		REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkVector, tkInt }));
	}
}

TEST_CASE("TypeKind, std::list")
{
	SECTION("std::list<std::string>") {
		std::list<std::string> list{ "hello", "world" };
		metapp::Variant v(list);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkList);
		REQUIRE(v.get<const std::list<std::string> &>().front() == "hello");
		REQUIRE(v.get<const std::list<std::string> &>().back() == "world");
		v.get<std::list<std::string> &>().push_back("good");
		REQUIRE(v.get<const std::list<std::string> &>().back() == "good");
		using namespace metapp;
		REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkList, tkString }));
	}
}

const void * func1(int, const std::vector<int> &) { return nullptr; }

TEST_CASE("TypeKind, function pointer")
{
	metapp::Variant v(&func1);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
	REQUIRE(v.get<void *>() == (void *)&func1);

	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkReference, tkVector, tkInt }));
	
	metaType = metapp::getMetaType<const void * (int, const std::vector<int> &)>();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkReference, tkVector, tkInt }));
}

struct Class1
{
	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
};

namespace metapp {
template <>
struct DeclareMetaType <Class1> : public DeclareMetaTypeBase<Class1>
{
	static constexpr TypeKind typeKind = 2000;
};
} // namespace metapp

TEST_CASE("TypeKind, member data")
{
	metapp::Variant v(&Class1::data);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkArray, tkInt }));
}

TEST_CASE("TypeKind, member function")
{
	metapp::Variant v(&Class1::func);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberFunction);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(3), { tkReference, tkVector, tkInt }));
}

