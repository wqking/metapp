#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/utils/utility.h"

namespace {

struct Base
{
	Base() = default;
	Base(const int n, int * ctorCounter) : myValue(n), ctorCounter(ctorCounter) {
		++*ctorCounter;
	}
	~Base() {
		myValue = 0;
		--*ctorCounter;
	}

	int myValue;
	int * ctorCounter;

};

TEST_CASE("metatypes, constructor")
{
	int ctorCounter = 0;
	metapp::Variant ctor = metapp::Constructor<Base (int, int *)>();
	REQUIRE(metapp::getTypeKind(ctor) == metapp::tkConstructor);

	SECTION("free by Variant") {
		{
			metapp::Variant arguments[] = { 7, &ctorCounter };
			metapp::Variant obj = metapp::Variant::takeFrom(
				ctor.getMetaType()->getMetaCallable()->invoke(ctor, nullptr, arguments, 2)
			);
			REQUIRE(ctorCounter == 1);
			Base & base = obj.get<Base &>();
			REQUIRE(obj.getMetaType() == metapp::getMetaType<Base>());
			REQUIRE(base.myValue == 7);
			REQUIRE(obj.get<const Base *>()->myValue == 7);
		}
		REQUIRE(ctorCounter == 0);
	}
	SECTION("free by delete, using metapp::invokeCallable") {
		Base * base = metapp::invokeCallable(ctor, nullptr, 7, &ctorCounter).get<Base *>();
		REQUIRE(base->myValue == 7);
		REQUIRE(ctorCounter == 1);
		delete base;
		REQUIRE(ctorCounter == 0);
	}
}


} // namespace
