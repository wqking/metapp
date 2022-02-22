#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

namespace {

std::string func1(const int a, const std::string & b)
{
	return b + std::to_string(a);
}

TEST_CASE("metatypes, DefaultArgsFunction")
{
	metapp::DefaultArgsFunction<decltype(&func1), 1> func;
}


} // namespace
