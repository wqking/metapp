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
	return static_cast<T>(n);
}

TEST_CASE("play camp")
{
	const int n = xxx<const int>((long double)5);
	std::cout << n;
	//metapp::MetaTypeDumper().dump(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
}


} // namespace
