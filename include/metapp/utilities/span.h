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

#ifndef METAPP_SPAN_H_969872685611
#define METAPP_SPAN_H_969872685611

#include "metapp/compiler.h"

#ifdef METAPP_SUPPORT_STD_SPAN

#include <span>

namespace metapp {

template<typename T, std::size_t Extent = std::dynamic_extent>
using span = std::span<T, Extent>;

} // namespace metapp

#else

#include "metapp/thirdparty/tcb/span.hpp"

namespace metapp {

template<typename T, std::size_t Extent = tcb::dynamic_extent>
using span = tcb::span<T, Extent>;

} // namespace metapp

#endif

#endif
