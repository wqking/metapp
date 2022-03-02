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
	//dumpMetaType(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
	
	REQUIRE(metapp::getMetaType<int *>()->getTypeKind() == metapp::tkPointer);
	REQUIRE(metapp::getMetaType<int *>()->getUpTypeCount() == 1);
	REQUIRE(metapp::getMetaType<int *>()->getUpType()->getTypeKind() == metapp::tkInt);

	REQUIRE(metapp::getMetaType<std::vector<char> >()->getTypeKind() == metapp::tkStdVector);
	REQUIRE(metapp::getMetaType<std::vector<char> >()->getUpTypeCount() == 1);
	REQUIRE(metapp::getMetaType<std::vector<char> >()->getUpType()->getTypeKind() == metapp::tkChar);

	REQUIRE(metapp::getMetaType<void (int *)>()->getTypeKind() == metapp::tkFunction);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpTypeCount() == 2);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpType(0)->getTypeKind() == metapp::tkVoid);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpType(1)->getTypeKind() == metapp::tkPointer);
	REQUIRE(metapp::getMetaType<void (int *)>()->getUpType(1)->getUpType()->getTypeKind() == metapp::tkInt);
}


} // namespace
