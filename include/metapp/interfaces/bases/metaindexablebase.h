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

#ifndef METAPP_METAINDEXABLEBASE_H_969872685611
#define METAPP_METAINDEXABLEBASE_H_969872685611

#include "metapp/interfaces/metaindexable.h"
#include "metapp/metatype.h"
#include "metapp/exception.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename ContainerType>
struct MetaIndexableBase
{
	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

private:
	using ValueType = decltype(std::declval<ContainerType &>()[0]);

	static size_t metaIndexableGetSize(const Variant & var)
	{
		return var.toReference().get<ContainerType &>().size();
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		return Variant::create<ValueType>(var.toReference().get<ContainerType &>()[index]);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		const Variant ref = var.toReference();

		verifyVariantWritable(ref);

		if(index >= metaIndexableGetSize(ref)) {
			errorInvalidIndex();
		}
		else {
			assignValue(ref.toReference().get<ContainerType &>()[index], value.get<ValueType &>());
		}
	}

};


} // namespace metapp


#endif

