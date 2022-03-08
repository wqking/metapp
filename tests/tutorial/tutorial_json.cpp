#include "tutorial.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/utils/utility.h"

#include <iostream>

class JsonDumper
{
public:
	explicit JsonDumper(std::ostream & stream) : stream(stream) {}

	void dump(const metapp::Variant & value)
	{
		doDump(value, 0);
	}

private:
	void doDump(const metapp::Variant & value, const int level)
	{
		auto metaType = metapp::getNonReferenceUpType(value.getMetaType());
		auto typeKind = metaType->getTypeKind();
		if(typeKind == metapp::tkVariant) {
			doDump(value.get<metapp::Variant &>(), level);
			return;
		}
		if(metapp::typeKindIsInteger(typeKind)) {
			stream << value.cast<long long>();
		}
		else if(metapp::typeKindIsReal(typeKind)) {
			stream << value.cast<long double>();
		}
		else if(value.canCast<std::string>()) {
			doDumpString(value.cast<std::string>().get<std::string>());
		}
		else if(metaType->getMetaMap() != nullptr) {
			doDumpObject(value, level);
		}
		else if(metaType->getMetaIterable() != nullptr) {
			doDumpArray(value, level);
		}
	}

	void doDumpString(const std::string & s)
	{
		stream << '"' << s << '"';
	}

	void doDumpArray(const metapp::Variant & value, const int level)
	{
		auto metaType = metapp::getNonReferenceUpType(value.getMetaType());
		stream << "[" << std::endl;
		bool firstItem = true;
		metaType->getMetaIterable()->forEach(
			value,
			[this, &firstItem, level](const metapp::Variant & item) -> bool {
				if(! firstItem) {
					stream << ",";
					stream << std::endl;
				}
				firstItem = false;
				doDumpIndent(level + 1);
				doDump(item, level + 1);

				return true;
			}
		);
		if(! firstItem) {
			stream << std::endl;
		}
		doDumpIndent(level);
		stream << "]";
	}

	void doDumpObject(const metapp::Variant & value, const int level)
	{
		auto metaType = metapp::getNonReferenceUpType(value.getMetaType());
		stream << "{" << std::endl;
		bool firstItem = true;
		metaType->getMetaIterable()->forEach(
			value,
			[this, &firstItem, level](const metapp::Variant & item) -> bool {
				if(! firstItem) {
					stream << ",";
					stream << std::endl;
				}
				firstItem = false;
				doDumpIndent(level + 1);
				auto indexable = metapp::getNonReferenceUpType(item.getMetaType())->getMetaIndexable();
				doDumpString(indexable->get(item, 0).cast<std::string>().get<std::string>());
				stream << ": ";
				doDump(indexable->get(item, 1), level + 1);

				return true;
			}
		);
		if(! firstItem) {
			stream << std::endl;
		}
		doDumpIndent(level);
		stream << "}";
	}

	void doDumpIndent(int level) {
		while(level-- > 0) {
			stream << "  ";
		}
	};

private:
	std::ostream & stream;
};

void tutorialJson()
{
	metapp::Variant value(std::map<std::string, metapp::Variant> {
		{ "first",  std::vector<metapp::Variant> {
			"abc",
			1,
			5.38,
			std::map<std::string, metapp::Variant> {
				{ "what", "good" },
				{ "next", 12345 },
			},
			std::unordered_map<std::string, std::string> {
				{ "hello", "def" },
			}
		}}
	});
	JsonDumper(std::cout).dump(value);
}

TEST_CASE("tutorialJson")
{
	std::cout << std::endl << "tutorialJson" << std::endl;

	tutorialJson();

	std::cout << std::endl;
}
