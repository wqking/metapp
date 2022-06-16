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

#ifndef METAPP_METAITERABLE_H_969872685611
#define METAPP_METAITERABLE_H_969872685611

#include "metapp/variant.h"
#include "metapp/utilities/utility.h"

#include <functional>

namespace metapp {

class Variant;

class MetaIterable
{
public:
	using Callback = std::function<bool (const Variant &)>;

	explicit MetaIterable(
			void (*forEach)(const Variant & iterable, const Callback & callback)
		) : forEach(forEach)
	{
	}

	void (*forEach)(const Variant & iterable, const Callback & callback);
};

inline void iterableForEach(const Variant & iterable, const MetaIterable::Callback & callback)
{
	getNonReferenceMetaType(iterable)->getMetaIterable()->forEach(iterable, callback);
}


} // namespace metapp

#endif
