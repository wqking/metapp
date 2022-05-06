// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testutil.h"

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"
#include "metapp/utilities/utility.h"

#include <iostream>

/*desc
# Tutorial for simple JSON dumper
desc*/

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
		if(value.getMetaType()->isPointer() && value.get<void *>() == nullptr) {
			stream << "null";
			return;
		}

		metapp::Variant ref = value.toReference();
		auto metaType = metapp::getReferredMetaType(ref.getMetaType());
		auto typeKind = metaType->getTypeKind();
		if(typeKind == metapp::tkVariant) {
			doDump(ref.get<metapp::Variant &>(), level);
			return;
		}
		if(metapp::typeKindIsIntegral(typeKind)) {
			stream << ref.cast<long long>();
		}
		else if(metapp::typeKindIsReal(typeKind)) {
			stream << ref.cast<long double>();
		}
		else if(ref.canCast<std::string>()) {
			doDumpString(ref.cast<std::string>().get<std::string>());
		}
		else if(metaType->getMetaMappable() != nullptr) {
			doDumpObject(ref, level);
		}
		else if(metaType->getMetaIterable() != nullptr) {
			doDumpArray(ref, level);
		}
	}

	void doDumpString(const std::string & s)
	{
		stream << '"' << s << '"';
	}

	void doDumpArray(const metapp::Variant & value, const int level)
	{
		auto metaType = metapp::getReferredMetaType(value.getMetaType());
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
		auto metaType = metapp::getReferredMetaType(value.getMetaType());
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
				auto indexable = metapp::getReferredMetaType(item.getMetaType())->getMetaIndexable();
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

TEST_CASE("tutorialJson")
{
	long ln = 987654321L;
	auto myTuple = std::make_tuple("def", 1, 'w');

	metapp::Variant value(std::map<std::string, metapp::Variant> {
		{ "first",  std::vector<metapp::Variant> {
			"abc",
			1,
			&ln,
			5.38,
			std::map<std::string, metapp::Variant> {
				{ "what", "good" },
				{ "next", 12345 },
			},
			std::unordered_map<std::string, std::string> {
				{ "hello", "def" },
			},
			&myTuple
		}},
		{ "second", nullptr }
	});
	JsonDumper(std::cout).dump(value);
}


