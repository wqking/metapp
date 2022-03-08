#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/metarepository.h"
#include "metapp/utils/metatypedumper.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

template <typename T, typename F>
T xxx(F n)
{
	return T {n};
	//return static_cast<T>(n);
}

//using X = std::unordered_map<std::string,std::string>;
using X = std::vector<metapp::Variant>;
static_assert(metapp::internal_::CanStaticCast<long double, X>::value, "a");
static_assert(std::is_class<X>::value, "e");
//static_assert(! metapp::internal_::CanCastSafely<long double, X>::value, "b");
//static_assert(metapp::internal_::IsNarrowingCast<long double, X>::value, "c");
static_assert(! metapp::internal_::IsNarrowingCast<char, int>::value, "d");

TEST_CASE("play camp")
{
	//xxx<X>((long double)5);
	//xxx<char>(5);
	//const int n = xxx<const int>((long double)5);
	//std::cout << n;
	//metapp::MetaTypeDumper().dump(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
}


} // namespace
