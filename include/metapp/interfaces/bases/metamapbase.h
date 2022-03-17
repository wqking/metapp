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

#ifndef METAPP_METAMAPBASE_H_969872685611
#define METAPP_METAMAPBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metamap.h"

namespace metapp {

template <typename ContainerType>
struct MetaMapBase
{
	static const MetaMap * getMetaMap() {
		static MetaMap metaMap(
			&metaMapGet,
			&metaMapSet
		);
		return &metaMap;
	}

private:
	using KeyType = typename ContainerType::key_type;
	using MappedType = typename ContainerType::mapped_type;

	static Variant metaMapGet(const Variant & var, const Variant & key)
	{
		const auto & container = var.get<const ContainerType &>();
		auto it = container.find(key.get<const KeyType &>());
		if(it != container.end()) {
			return Variant::create<const MappedType &>(it->second);
		}
		return Variant();
	}

	static void metaMapSet(const Variant & var, const Variant & key, const Variant & value)
	{
		auto & container = var.get<ContainerType &>();
		auto it = container.find(key.get<const KeyType &>());
		if(it != container.end()) {
			assignValue(it->second, value.get<const MappedType &>());
		}
		else {
			errorUnwritable();
		}
	}

};


} // namespace metapp


#endif

