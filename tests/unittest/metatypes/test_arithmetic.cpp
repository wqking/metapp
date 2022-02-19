#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, bool")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(true)) == metapp::tkBool);
	REQUIRE(metapp::Variant(true).get<bool>() == true);
	REQUIRE(metapp::Variant(true).getMetaType()->getName() == std::string("bool"));
}

TEST_CASE("metatypes, char")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant('c')) == metapp::tkChar);
	REQUIRE(metapp::Variant('c').get<char>() == 'c');
	REQUIRE(metapp::Variant('c').getMetaType()->getName() == std::string("char"));
}

TEST_CASE("metatypes, signed char")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant((signed char)127)) == metapp::tkSignedChar);
	REQUIRE(metapp::Variant((signed char)127).get<signed char>() == 127);
	REQUIRE(metapp::Variant((signed char)127).getMetaType()->getName() == std::string("signed char"));
}

TEST_CASE("metatypes, unsigned char")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant((unsigned char)255)) == metapp::tkUnsignedChar);
	REQUIRE(metapp::Variant((unsigned char)255).get<unsigned char>() == 255);
	REQUIRE(metapp::Variant((unsigned char)255).getMetaType()->getName() == std::string("unsigned char"));
}

TEST_CASE("metatypes, wchar_t")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(L'c')) == metapp::tkWideChar);
	REQUIRE(metapp::Variant(L'c').get<wchar_t>() == L'c');
	REQUIRE(metapp::getTypeKind(metapp::Variant((wchar_t)0x1cde)) == metapp::tkWideChar);
	REQUIRE(metapp::Variant((wchar_t)0x1cde).get<wchar_t>() == (wchar_t)0x1cde);
	REQUIRE(metapp::Variant((wchar_t)0x1cde).getMetaType()->getName() == std::string("wchar_t"));
}

TEST_CASE("metatypes, short")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant((short)5)) == metapp::tkShort);
	REQUIRE(metapp::Variant((short)5).get<short>() == 5);
	REQUIRE(metapp::getTypeKind(metapp::Variant((short)0x6789)) == metapp::tkShort);
	REQUIRE(metapp::Variant((short)0x6789).get<short>() == 0x6789);
	REQUIRE(metapp::Variant((short)0x6789).getMetaType()->getName() == std::string("short"));
}

TEST_CASE("metatypes, unsigned short")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant((unsigned short)5)) == metapp::tkUnsignedShort);
	REQUIRE(metapp::Variant((unsigned short)5).get<unsigned short>() == 5);
	REQUIRE(metapp::getTypeKind(metapp::Variant((unsigned short)0xf987)) == metapp::tkUnsignedShort);
	REQUIRE(metapp::Variant((unsigned short)0xf987).get<unsigned short>() == 0xf987);
	REQUIRE(metapp::Variant((unsigned short)0xf987).getMetaType()->getName() == std::string("unsigned short"));
}

TEST_CASE("metatypes, int")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(98)) == metapp::tkInt);
	REQUIRE(metapp::Variant(98).get<int>() == 98);
	REQUIRE(metapp::Variant(98).getMetaType()->getName() == std::string("int"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<int>::max())) == metapp::tkInt);
	REQUIRE(metapp::Variant(std::numeric_limits<int>::max()).get<int>() == std::numeric_limits<int>::max());
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<int>::min())) == metapp::tkInt);
	REQUIRE(metapp::Variant(std::numeric_limits<int>::min()).get<int>() == std::numeric_limits<int>::min());
}

TEST_CASE("metatypes, unsigned int")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(98u)) == metapp::tkUnsignedInt);
	REQUIRE(metapp::Variant(98u).get<unsigned int>() == 98u);
	REQUIRE(metapp::Variant(98u).getMetaType()->getName() == std::string("unsigned int"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned int>::max())) == metapp::tkUnsignedInt);
	REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::max()).get<unsigned int>() == std::numeric_limits<unsigned int>::max());
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned int>::min())) == metapp::tkUnsignedInt);
	REQUIRE(metapp::Variant(std::numeric_limits<unsigned int>::min()).get<unsigned int>() == std::numeric_limits<unsigned int>::min());
}

TEST_CASE("metatypes, long")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(9L)) == metapp::tkLong);
	REQUIRE(metapp::Variant(9L).get<long>() == 9L);
	REQUIRE(metapp::Variant(9L).getMetaType()->getName() == std::string("long"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long>::max())) == metapp::tkLong);
	REQUIRE(metapp::Variant(std::numeric_limits<long>::max()).get<long>() == std::numeric_limits<long>::max());
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long>::min())) == metapp::tkLong);
	REQUIRE(metapp::Variant(std::numeric_limits<long>::min()).get<long>() == std::numeric_limits<long>::min());
}

TEST_CASE("metatypes, unsigned long")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(9UL)) == metapp::tkUnsignedLong);
	REQUIRE(metapp::Variant(9UL).get<unsigned long>() == 9UL);
	REQUIRE(metapp::Variant(9UL).getMetaType()->getName() == std::string("unsigned long"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long>::max())) == metapp::tkUnsignedLong);
	REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::max()).get<unsigned long>() == std::numeric_limits<unsigned long>::max());
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long>::min())) == metapp::tkUnsignedLong);
	REQUIRE(metapp::Variant(std::numeric_limits<unsigned long>::min()).get<unsigned long>() == std::numeric_limits<unsigned long>::min());
}

TEST_CASE("metatypes, long long")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(9LL)) == metapp::tkLongLong);
	REQUIRE(metapp::Variant(9LL).get<long long>() == 9LL);
	REQUIRE(metapp::Variant(9LL).getMetaType()->getName() == std::string("long long"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long long>::max())) == metapp::tkLongLong);
	REQUIRE(metapp::Variant(std::numeric_limits<long long>::max()).get<long long>() == std::numeric_limits<long long>::max());
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long long>::min())) == metapp::tkLongLong);
	REQUIRE(metapp::Variant(std::numeric_limits<long long>::min()).get<long long>() == std::numeric_limits<long long>::min());
}

TEST_CASE("metatypes, unsigned long long")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(9ULL)) == metapp::tkUnsignedLongLong);
	REQUIRE(metapp::Variant(9ULL).get<unsigned long long>() == 9ULL);
	REQUIRE(metapp::Variant(9ULL).getMetaType()->getName() == std::string("unsigned long long"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long long>::max())) == metapp::tkUnsignedLongLong);
	REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::max()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::max());
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<unsigned long long>::min())) == metapp::tkUnsignedLongLong);
	REQUIRE(metapp::Variant(std::numeric_limits<unsigned long long>::min()).get<unsigned long long>() == std::numeric_limits<unsigned long long>::min());
}

TEST_CASE("metatypes, float")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(123.5678f)) == metapp::tkFloat);
	REQUIRE(metapp::Variant(123.5678f).get<float>() == Approx(123.5678f));
	REQUIRE(metapp::Variant(123.5678f).getMetaType()->getName() == std::string("float"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<float>::max())) == metapp::tkFloat);
	REQUIRE(metapp::Variant(std::numeric_limits<float>::max()).get<float>() == Approx(std::numeric_limits<float>::max()));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<float>::min())) == metapp::tkFloat);
	REQUIRE(metapp::Variant(std::numeric_limits<float>::min()).get<float>() == Approx(std::numeric_limits<float>::min()));
}

TEST_CASE("metatypes, double")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(123.5678)) == metapp::tkDouble);
	REQUIRE(metapp::Variant(123.5678).get<double>() == Approx(123.5678));
	REQUIRE(metapp::Variant(123.5678).getMetaType()->getName() == std::string("double"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<double>::max())) == metapp::tkDouble);
	REQUIRE(metapp::Variant(std::numeric_limits<double>::max()).get<double>() == Approx(std::numeric_limits<double>::max()));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<double>::min())) == metapp::tkDouble);
	REQUIRE(metapp::Variant(std::numeric_limits<double>::min()).get<double>() == Approx(std::numeric_limits<double>::min()));
}

TEST_CASE("metatypes, long double")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant((long double)123.5678)) == metapp::tkLongDouble);
	REQUIRE(metapp::Variant((long double)123.5678).get<long double>() == Approx((long double)123.5678));
	REQUIRE(metapp::Variant((long double)123.5678).getMetaType()->getName() == std::string("long double"));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long double>::max())) == metapp::tkLongDouble);
	REQUIRE(metapp::Variant(std::numeric_limits<long double>::max()).get<long double>() == Approx(std::numeric_limits<long double>::max()));
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::numeric_limits<long double>::min())) == metapp::tkLongDouble);
	REQUIRE(metapp::Variant(std::numeric_limits<long double>::min()).get<long double>() == Approx(std::numeric_limits<long double>::min()));
}

