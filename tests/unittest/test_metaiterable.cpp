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
#include "include/containerdataprovider.h"

#include "metapp/variant.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEMPLATE_LIST_TEST_CASE("MetaIterable forEach", "", TestTypes_Iterables)
{
	using Container = TestType;
	using ValueType = typename Container::value_type;
	auto dataProvider = TestContainerDataProvider<Container>();
	Container container = dataProvider.getContainer();
	metapp::Variant v;
	const int type = GENERATE(0, 1, 2);
	switch(type) {
	case 0:
		v = metapp::Variant(container);
		break;

	case 1:
		v = metapp::Variant(&container);
		break;

	case 2:
		v = metapp::Variant::create<const Container &>(container);
		break;
	}

	std::vector<ValueType> resultList;
	auto metaIterable = metapp::getReferredMetaType(v)->getMetaIterable();
	REQUIRE(metaIterable != nullptr);
	metaIterable->forEach(v, [&resultList](const metapp::Variant & value) -> bool {
		resultList.push_back(value.get<ValueType &>());
		return true;
	});
	REQUIRE((int)resultList.size() == getContainerSize(container));
	for(size_t i = 0; i < resultList.size(); ++i) {
		auto it = container.begin();
		std::advance(it, i);
		REQUIRE(resultList.at(i) == *it);
	}
}



} // namespace
