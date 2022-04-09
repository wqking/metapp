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

#include "test.h"

#include "metapp/variant.h"
#include "metapp/metarepo.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <sstream>

TEST_CASE("Variant, input stream")
{
	std::stringstream ss;
	SECTION("bool") {
		metapp::Variant v(metapp::getMetaRepo()->getType(metapp::tkBool).getTarget());
		REQUIRE(v.get<bool>() == false);
		ss >> std::boolalpha;
		ss.str("true");
		ss >> v;
		REQUIRE(v.get<bool>() == true);
	}

	SECTION("int") {
		metapp::Variant v(metapp::getMetaType<int>());
		REQUIRE(v.get<int>() == 0);
		ss.str("138");
		ss >> v;
		REQUIRE(v.get<int>() == 138);
	}
	
	SECTION("std::string") {
		metapp::Variant v(metapp::getMetaType<std::string>());
		REQUIRE(v.get<std::string>() == "");
		ss.str("abcxyz");
		ss >> v;
		REQUIRE(v.get<std::string>() == "abcxyz");
	}
}

TEST_CASE("Variant, output stream")
{
	std::stringstream ss;
	SECTION("bool") {
		metapp::Variant v(false);
		ss << std::boolalpha;
		ss << v;
		REQUIRE(ss.str() == "false");

		v = true;
		ss << std::boolalpha;
		ss << v;
		REQUIRE(ss.str() == "falsetrue");
	}

	SECTION("char") {
		metapp::Variant v((char)(int)'w');
		ss << v;
		REQUIRE(ss.str() == "w");
	}

	SECTION("float") {
		metapp::Variant v(3.1415f);
		ss << v;
		REQUIRE(ss.str() == "3.1415");
	}

	SECTION("double") {
		metapp::Variant v(3.1415);
		ss << v;
		REQUIRE(ss.str() == "3.1415");
	}

	SECTION("std::string") {
		std::string s("abcdef");
		metapp::Variant v(s);
		ss << v;
		REQUIRE(ss.str() == s);
	}

}


