#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/metarepository.h"

#include <string>
#include <iostream>
#include <climits>

inline void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType)
{
	struct Dumper
	{
		static void printIndent(std::ostream & stream, int level) {
			while(level-- > 0) {
				stream << "  ";
			}
		};

		static void doDump(std::ostream & stream, const metapp::MetaType * mt, const int level) {
			if(mt == nullptr) {
				return;
			}
			printIndent(stream, level);
			stream << "Type: " << mt->getTypeKind();
			std::string name;
			name = metapp::getMetaRepository()->getNameByKind(mt->getTypeKind());
			if(name.empty()) {
				name = metapp::getNameByTypeKind(mt->getTypeKind());
			}
			if(! name.empty()) {
				stream << "(" << name << ")";
			}
			if(mt->isConst() || mt->isVolatile()) {
				stream << ", CV:";
				if(mt->isConst()) {
					stream << " const";
				}
				if(mt->isVolatile()) {
					stream << " volatile";
				}
			}
			const size_t upCount = mt->getUpTypeCount();
			stream << ", UpCount: " << upCount;
			stream << std::endl;
			for(size_t i = 0; i < upCount; ++i) {
				doDump(stream, mt->getUpType(i), level + 1);
			}
		};
	};

	Dumper::doDump(stream, metaType, 0);
}

namespace {

TEST_CASE("play camp")
{
	dumpMetaType(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
}

TEST_CASE("aaa")
{
	{
		metapp::Variant v(true);
		REQUIRE(v.get<bool>() == true);
		v.cast<bool>();
		REQUIRE(v.cast<bool>().get<bool>() == true);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkBool);
	}
	{
		metapp::Variant v((char)38);
		REQUIRE(v.get<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>().get<bool>() == true);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkChar);
	}
	{
		int n = 5;
		int * p = &n;
		metapp::Variant v(p);
		REQUIRE(v.get<int *>() == p);
		REQUIRE(*v.get<int *>() == 5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
	}
	{
		int n = 5;
		int & p = n;
		metapp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.get<int &>() == 5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	}
	{
		metapp::Variant v("abc");
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	}
	{
		char s[] = "abc";
		metapp::Variant v;
		v.set<char[]>(s);
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	}

	{
		std::string s("abc");
		metapp::Variant v(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkString);
	}

	{
		std::string s("abc");
		metapp::Variant v;
		v.set<const std::string>(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkString);
	}

	{
		std::wstring s(L"abc");
		metapp::Variant v(s);
		REQUIRE(v.get<std::wstring>() == s);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkWideString);
	}

}

} // namespace
