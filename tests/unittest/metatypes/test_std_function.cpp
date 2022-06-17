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
#include "metapp/allmetatypes.h"

TEST_CASE("metatypes, std::function<int (int)>, types")
{
	std::function<int (int)> f([](const int n) { return n * 2; });
	metapp::Variant v(f);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdFunction);
	metapp::Variant arguments[1] = { 5 };
	REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments).get<int>() == 10);

	auto metaType = v.getMetaType();
	REQUIRE(metaType->getUpType(0)->equal(metapp::getMetaType<int>()));
	REQUIRE(metaType->getUpType(1)->equal(metapp::getMetaType<int>()));
}

TEST_CASE("metatypes, std::function<void (int &, std::string &)>, invoke")
{
	std::function<void (int &, std::string &)> func([](int & a, std::string & b) {
		a = 38;
		b = "hello";
		});
	metapp::Variant v(func);
	int a = 0;
	std::string b;
	metapp::Variant arguments[2] = { metapp::Variant::reference(a), metapp::Variant::reference(b) };
	v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments);
	REQUIRE(a == 38);
	REQUIRE(b == "hello");
}

TEST_CASE("metatypes, std::function<int & (int &)>, invoke")
{
	std::function<int & (int &)> func([](int & a) -> int & {
		return a;
	});
	metapp::Variant v(func);
	int a = 0;
	metapp::Variant arguments[1] = { metapp::Variant::reference(a) };
	metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments);
	REQUIRE(result.getMetaType()->isReference());
}

TEST_CASE("metatypes, std::function<std::unique_ptr<int> ()>, invoke")
{
	std::function<std::unique_ptr<int> ()> func([]() -> std::unique_ptr<int> {
		return std::unique_ptr<int>(new int{5});
	});
	metapp::Variant v(func);
	metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, {});
	REQUIRE(result.getMetaType()->equal(metapp::getMetaType<std::unique_ptr<int> >()));
}

TEST_CASE("metatypes, std::function<int (int)>, cast from other MetaCallable")
{
	struct X {
		static int add(const double value) {
			return (int)value + 1;
		}
	};
	metapp::Variant v(X::add);
	using FT = std::function<int (int)>;
	REQUIRE(v.canCast<FT>());
	FT f = v.cast<FT>().get<FT &>();
	REQUIRE(f(2) == 3);
}

TEST_CASE("metatypes, std::function<std::unique_ptr<int> (int)>, cast from other MetaCallable")
{
	struct X {
		static std::unique_ptr<int> func(const double value) {
			return std::unique_ptr<int>(new int((int)value + 5));
		}
	};
	metapp::Variant v(X::func);
	using FT = std::function<std::unique_ptr<int> (int)>;
	REQUIRE(v.canCast<FT>());
	FT f = v.cast<FT>().get<FT &>();
	REQUIRE(*f(2) == 7);
}

TEST_CASE("metatypes, std::function<int & ()>, cast from other MetaCallable")
{
	struct X {
		static int & func() {
			static int x = 38;
			return x;
		}
	};
	metapp::Variant v(X::func);
	using FT = std::function<int & ()>;
	REQUIRE(v.canCast<FT>());
	FT f = v.cast<FT>().get<FT &>();
	REQUIRE(f() == 38);
	REQUIRE(X::func() == 38);
	f() = 5;
	REQUIRE(f() == 5);
	REQUIRE(X::func() == 5);
}

TEST_CASE("metatypes, std::function<int (int)>, can't cast from std::string")
{
	metapp::Variant v(std::string("abc"));
	using FT = std::function<int (int)>;
	REQUIRE(! v.canCast<FT>());
}

TEST_CASE("metatypes, std::function<int (int)>, can't cast from int(int, int)")
{
	struct X {
		static int add(const int a, const int b) {
			return a + b + 1;
		}
	};
	metapp::Variant v(X::add);
	using FT = std::function<int (int)>;
	REQUIRE(! v.canCast<FT>());
}

TEST_CASE("metatypes, std::function, cast from default args function")
{
	struct X {
		static std::string concat(const int a, const std::string & b, const long c) {
			return std::to_string(a) + b + std::to_string(c);
		}
	};
	metapp::Variant v(metapp::createDefaultArgsFunction(&X::concat, { 38, "hello" }));
	
	SECTION("Can't cast to 0 argument") {
		REQUIRE(! v.canCast<std::function<std::string ()> >());
	}
	
	SECTION("Cast to 1 argument") {
		using FT = std::function<std::string (int)>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()(5) == "5hello38");
	}

	SECTION("Cast to 2 arguments") {
		using FT = std::function<std::string (int, std::string)>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()(6, "good") == "6good38");
	}

	SECTION("Cast to 3 arguments") {
		using FT = std::function<std::string (int, std::string, long)>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()(6, "good", 9) == "6good9");
	}

	SECTION("Can't cast to 4 argument") {
		REQUIRE(! v.canCast<std::function<std::string (int, std::string, long, int)> >());
	}
}

TEST_CASE("metatypes, std::function, cast from variadic function")
{
	struct X {
		static int sum(const metapp::ArgumentSpan & arguments)
		{
			int total = 0;
			for(const auto & argument : arguments) {
				total += argument.cast<int>().get<int>();
			}
			return total;
		}

	};

	metapp::Variant v(metapp::createVariadicFunction(&X::sum));

	SECTION("Cast to 0 argument") {
		using FT = std::function<int ()>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()() == 0);
	}

	SECTION("Cast to 1 argument") {
		using FT = std::function<int (int)>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()(5) == 5);
	}

	SECTION("Cast to 2 arguments") {
		using FT = std::function<int (int, int)>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()(5, 6) == 11);
	}

	SECTION("Cast to 10 arguments") {
		using FT = std::function<int (int, int, int, int, int, int, int, int, int, int)>;
		REQUIRE(v.canCast<FT>());
		REQUIRE(v.cast<FT>().get<FT &>()(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 55);
	}

}

