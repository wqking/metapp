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

#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "../catch.hpp"

#include <iostream>

// Alias REQUIRE to ASSERT for easy understanding
// for users who are not familar with Catch2
#define ASSERT REQUIRE

#define I_UFN_STRINGIZE(s) #s
#define UFN_STRINGIZE(s) I_UFN_STRINGIZE(s)
#define I_UFN_CONCAT(a, b) a ## b
#define UFN_CONCAT(a, b) I_UFN_CONCAT(a, b)
#define UFN TEST_CASE(UFN_STRINGIZE(UFN_CONCAT(__FILE__, __LINE__)))

#endif
