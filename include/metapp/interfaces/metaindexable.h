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

#ifndef METAPP_METAINDEXABLE_H_969872685611
#define METAPP_METAINDEXABLE_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaIndexable
{
public:
	enum { unknowSize = (size_t)-1 };

	MetaIndexable() = delete;

	MetaIndexable(
		size_t (*getSize)(const Variant & var),
		Variant (*get)(const Variant & var, const size_t index),
		void (*set)(const Variant & var, const size_t index, const Variant & value)
	)
		: getSize(getSize), get(get), set(set)
	{
	}

	size_t (*getSize)(const Variant & var);
	Variant (*get)(const Variant & var, const size_t index);
	void (*set)(const Variant & var, const size_t index, const Variant & value);

};


} // namespace metapp

#endif
