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

#ifndef METAPP_METAPOINTERWRAPPER_H_969872685611
#define METAPP_METAPOINTERWRAPPER_H_969872685611

#include "metapp/variant.h"
#include "metapp/utilities/utility.h"

#include <functional>

namespace metapp {

class Variant;

class MetaPointerWrapper
{
public:
	using Callback = std::function<bool (const Variant &)>;

	MetaPointerWrapper(
		Variant (*getPointer)(const Variant & pointerWrapper),
		void (*setPointer)(const Variant & pointerWrapper, const Variant & pointer)
	)
		:
			getPointer(getPointer),
			setPointer(setPointer)
	{
	}

	Variant (*getPointer)(const Variant & pointerWrapper);
	void (*setPointer)(const Variant & pointerWrapper, const Variant & pointer);
};

inline Variant pointerWrapperGetPointer(const Variant & pointerWrapper)
{
	return getNonReferenceMetaType(pointerWrapper)->getMetaPointerWrapper()->getPointer(pointerWrapper);
}

inline void pointerWrapperSetPointer(const Variant & pointerWrapper, const Variant & pointer)
{
	getNonReferenceMetaType(pointerWrapper)->getMetaPointerWrapper()->setPointer(pointerWrapper, pointer);
}

} // namespace metapp

#endif
