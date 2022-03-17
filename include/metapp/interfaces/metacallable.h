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

#ifndef METAPP_METACALLABLE_H_969872685611
#define METAPP_METACALLABLE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

class Variant;

class MetaCallable
{
public:
	MetaCallable(
		size_t (*getParamCount)(),
		const MetaType * (*getReturnType)(),
		const MetaType * (*getParamType)(const size_t index),
		int (*rankInvoke)(const Variant * arguments, const size_t argumentCount),
		bool (*canInvoke)(const Variant * arguments, const size_t argumentCount),
		Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	)
		:
			getParamCount(getParamCount),
			getReturnType(getReturnType),
			getParamType(getParamType),
			rankInvoke(rankInvoke),
			canInvoke(canInvoke),
			invoke(invoke)
	{
	}

	size_t (*getParamCount)();
	const MetaType * (*getReturnType)();
	const MetaType * (*getParamType)(const size_t index);

	int (*rankInvoke)(const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount);
};


} // namespace metapp

#endif
