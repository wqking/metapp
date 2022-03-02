#include "tutorial.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <iostream>

using JsonString = std::string;
using JsonObject = std::map<std::string, metapp::Variant>;
using JsonArray = std::vector<metapp::Variant>;

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
		if(metapp::typeKindIsInteger(metapp::getTypeKind(value))) {
			stream << value.cast<long long>();
		}
		else if(metapp::typeKindIsReal(metapp::getTypeKind(value))) {
			stream << value.cast<long double>();
		}
		else if(value.canCast<JsonString>()) {
			doDumpString(value.cast<JsonString>().get<JsonString>());
		}
		else if(value.canGet<JsonArray>()) {
			doDumpArray(value, level);
		}
		else if(value.canGet<JsonObject>()) {
			doDumpObject(value, level);
		}
	}

	void doDumpString(const JsonString & s)
	{
		stream << '"' << s << '"';
	}

	void doDumpArray(const metapp::Variant & value, const int level)
	{
		stream << "[" << std::endl;
		const JsonArray & array = value.get<const JsonArray &>();
		bool firstItem = true;
		for(const auto & item : array) {
			if(! firstItem) {
				stream << ",";
				stream << std::endl;
			}
			firstItem = false;
			doDumpIndent(level + 1);
			doDump(item, level + 1);
		}
		if(! firstItem) {
			stream << std::endl;
		}
		doDumpIndent(level);
		stream << "]";
	}

	void doDumpObject(const metapp::Variant & value, const int level)
	{
		stream << "{" << std::endl;
		const JsonObject & object = value.get<const JsonObject &>();
		bool firstItem = true;
		for(const auto & item : object) {
			if(! firstItem) {
				stream << ",";
				stream << std::endl;
			}
			firstItem = false;
			doDumpIndent(level + 1);
			doDumpString(item.first);
			stream << ": ";
			doDump(item.second, level + 1);
		}
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
	metapp::Variant value(JsonObject {
		{ "first",  JsonArray{
			"abc",
			1,
			5.38,
			JsonObject {
				{ "what", "good" },
				{ "next", 12345 },
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
