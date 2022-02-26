#include "test.h"

#include "metapp/variant.h"
#include "metapp/metaclass.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct Base1
{
};

struct Derived1 : Base1
{
};

namespace metapp {

constexpr TypeKind tkDerived = tkUser;

template <>
struct DeclareMetaType <Derived1> : public DeclareMetaTypeBase <Base1>
{
	static constexpr TypeKind typeKind = tkDerived;

	static const MetaClass * getMetaClass() {
		static const DeclareMetaClass<Derived1> metaClass(
			[](DeclareMetaClass<Derived1> & mc) {
				mc.addBase<Base1>();
			}
		);
		return &metaClass;
	}

};


} // namespace metapp

TEST_CASE("MetaClass")
{
}

