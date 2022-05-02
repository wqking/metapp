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

#include "metapp/implement/internal/disjointview_i.h"

#include <deque>
#include <array>
#include <iostream>

namespace {

TEST_CASE("DisjointView")
{
	using ListType = std::deque<int>;
	using ViewType = metapp::internal_::DisjointView<int, ListType>;

	std::array<ListType, 2> lists {
		ListType { 1, 2, 3, 4 },
		ListType { 5, 6, 7, 8, 9 },
	};
	ViewType view;
	view.addContainer(&lists[0]);
	view.addContainer(&lists[1]);
	
	REQUIRE(view.size() == 9);

	REQUIRE(*(view.begin()) == 1);
	auto it = view.begin();
	REQUIRE(*++it == 2);
	REQUIRE(*it++ == 2);
	REQUIRE(*it == 3);
}

TEST_CASE("DisjointView, [], at")
{
	using ListType = std::deque<int>;
	using ViewType = metapp::internal_::DisjointView<int, ListType>;

	std::array<ListType, 6> lists {
		ListType { 1, 2, 3, 4 },
		ListType { 5, 6, 7, 8, 9 },
		ListType {},
		ListType { 10, 11 },
		ListType { 12, 13, 14, 15 },
		ListType { 16 },
	};
	ViewType view;
	for(auto it = lists.begin(); it != lists.end(); ++it) {
		view.addContainer(&*it);
	}
	
	REQUIRE(view.size() == 16);
	for(int i = 0; i < 16; ++i) {
		REQUIRE(view[i] == i + 1);
		REQUIRE(view.at(i) == i + 1);
	}
}

TEST_CASE("DisjointView, end")
{
	using ListType = std::deque<int>;
	using ViewType = metapp::internal_::DisjointView<int, ListType>;

	std::array<ListType, 2> lists {
		ListType { 1 },
		ListType { 2, 3 },
	};
	ViewType view;
	for(auto it = lists.begin(); it != lists.end(); ++it) {
		view.addContainer(&*it);
	}
	auto begin = view.begin();
	auto end = view.end();
	REQUIRE(begin != end);
	++begin;
	++begin;
	++begin;
	REQUIRE(begin == end);
}

TEST_CASE("DisjointView, for loop")
{
	using ListType = std::deque<int>;
	using ViewType = metapp::internal_::DisjointView<int, ListType>;

	std::array<ListType, 2> lists {
		ListType { 1, 2, 3, 4 },
		ListType { 5, 6, 7, 8, 9 },
	};
	ViewType view;
	for(auto it = lists.begin(); it != lists.end(); ++it) {
		view.addContainer(&*it);
	}
	
	int expected = 1;
	for(auto it = view.begin(); it != view.end(); ++it) {
		REQUIRE(*it == expected);
		++expected;
	}
}


} // namespace
