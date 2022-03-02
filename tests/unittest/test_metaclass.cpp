#include "test.h"

#include "metapp/variant.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct BaseFirst
{
	BaseFirst() : first(0) {
	}

	explicit BaseFirst(const int value) : first(value) {
	}

	int first;
};

struct BaseSecond
{
	BaseSecond() : second(0) {
	}

	explicit BaseSecond(const int value) : second(value) {
	}

	int second;
};

struct SonFirst : BaseFirst
{
};

struct SonFirstSecond : BaseFirst, BaseSecond
{
};

namespace metapp {

constexpr TypeKind tkBaseFirst = tkUser;
constexpr TypeKind tkBaseSecond = tkUser + 1;
constexpr TypeKind tkSonFirst = tkUser + 10;
constexpr TypeKind tkSonFirstSecond = tkUser + 11;

template <>
struct DeclareMetaType <BaseFirst> : public DeclareMetaTypeBase <BaseFirst>
{
	static constexpr TypeKind typeKind = tkBaseFirst;

	static const MetaClass * getMetaClass() {
		static const DeclareMetaClass<BaseFirst> metaClass(
			[](DeclareMetaClass<BaseFirst> & mc) {
				mc.addConstructor(metapp::Constructor<BaseFirst()>());
				mc.addConstructor(metapp::Constructor<BaseFirst(int)>());
				mc.addField("first", &BaseFirst::first);
			}
		);
		return &metaClass;
	}

};

template <>
struct DeclareMetaType <SonFirst> : public DeclareMetaTypeBase <SonFirst>
{
	static constexpr TypeKind typeKind = tkSonFirst;

	static const MetaClass * getMetaClass() {
		static const DeclareMetaClass<SonFirst> metaClass(
			[](DeclareMetaClass<SonFirst> & mc) {
				mc.addBase<BaseFirst>();
			}
		);
		return &metaClass;
	}

};

template <>
struct DeclareMetaType <SonFirstSecond> : public DeclareMetaTypeBase <SonFirstSecond>
{
	static constexpr TypeKind typeKind = tkSonFirstSecond;

	static const MetaClass * getMetaClass() {
		static const DeclareMetaClass<SonFirstSecond> metaClass(
			[](DeclareMetaClass<SonFirstSecond> & mc) {
				mc.addBase<BaseFirst, BaseSecond>();
			}
		);
		return &metaClass;
	}

};


} // namespace metapp

TEST_CASE("MetaClass, cast")
{
	SonFirstSecond obj;
	BaseFirst * first = &obj;
	BaseSecond * second = &obj;
	void * castedFirst = metapp::getMetaType<SonFirstSecond>()->getMetaClass()->castToBase(&obj, 0);
	void * castedSecond = metapp::getMetaType<SonFirstSecond>()->getMetaClass()->castToBase(&obj, 1);
	REQUIRE(first == castedFirst);
	REQUIRE(second == castedSecond);
}

