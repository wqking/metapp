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

#ifndef METAPP_CONSTRUCT_I_H_969872685611
#define METAPP_CONSTRUCT_I_H_969872685611

#include "metapp/exception.h"

#include <type_traits>

namespace metapp {

namespace internal_ {

template <typename T>
T * constructOnHeapDefault(std::true_type)
{
	return new T();
}

template <typename T>
T * constructOnHeapDefault(std::false_type)
{
	errorNotConstructible();
	return nullptr;
}

template <typename T, typename U>
T * constructOnHeapCopy(const void * copyFrom, std::true_type, U)
{
	return new T(*(T *)copyFrom);
}

template <typename T>
T * constructOnHeapCopy(const void * copyFrom, std::false_type, std::true_type)
{
	return new T(std::move(*(T *)copyFrom));
}

template <typename T>
T * constructOnHeapCopy(const void * copyFrom, std::false_type, std::false_type)
{
	errorNotConstructible();
	return nullptr;
}

template <typename T>
T * constructOnHeap(const void * copyFrom)
{
	if(copyFrom == nullptr) {
		return constructOnHeapDefault<T>(std::is_default_constructible<T>());
	}
	else {
		return constructOnHeapCopy<T>(copyFrom, std::is_copy_assignable<T>(), std::is_move_assignable<T>());
	}
}

} // namespace internal_

} // namespace metapp

#endif
