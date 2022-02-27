#include "test.h"

#include "metapp/variant.h"
#include "metapp/metaclass.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct Base
{
	Base() : value(0) {
	}

	explicit Base(const int value) : value(value) {
	}

	int value;
};

struct DerivedFirst : Base
{
};

namespace metapp {

constexpr TypeKind tkBase = tkUser;
constexpr TypeKind tkDerivedFirst = tkUser + 1;

template <>
struct DeclareMetaType <Base> : public DeclareMetaTypeBase <Base>
{
	static constexpr TypeKind typeKind = tkBase;

	static const MetaClass * getMetaClass() {
		static const DeclareMetaClass<Base> metaClass(
			[](DeclareMetaClass<Base> & mc) {
				mc.addConstructor(metapp::Constructor<Base()>());
				mc.addConstructor(metapp::Constructor<Base(int)>());
				mc.addField("value", &Base::value);
			}
		);
		return &metaClass;
	}

};

template <>
struct DeclareMetaType <DerivedFirst> : public DeclareMetaTypeBase <DerivedFirst>
{
	static constexpr TypeKind typeKind = tkDerivedFirst;

	static const MetaClass * getMetaClass() {
		static const DeclareMetaClass<DerivedFirst> metaClass(
			[](DeclareMetaClass<DerivedFirst> & mc) {
				mc.addBase<Base>();
			}
		);
		return &metaClass;
	}

};


} // namespace metapp

TEST_CASE("MetaClass")
{
}

