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

#ifndef TEST_H
#define TEST_H

#include "metapp/metatype.h"
#include "metapp/typekind.h"
#include "metapp/utilities/typelist.h"

#include "../catch.hpp"

#include <vector>
#include <unordered_set>
#include <tuple>
#include <initializer_list>

template <typename Container>
int getContainerSize(const Container & container)
{
	// some container such as std::forward_list doesn't support size() function
	int size = 0;
	for(auto it = std::begin(container); it != std::end(container); ++it) {
		++size;
	}
	return size;
}


#endif
