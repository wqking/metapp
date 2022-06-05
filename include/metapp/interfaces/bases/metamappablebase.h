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

#ifndef METAPP_METAMAPPABLEBASE_H_969872685611
#define METAPP_METAMAPPABLEBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metamappable.h"

namespace metapp {

template <typename ContainerType>
struct MetaMappableBase
{
	static const MetaMappable * getMetaMappable() {
		static MetaMappable metaMap(
			&metaMapGetValueType,
			&metaMapGet,
			&metaMapSet
		);
		return &metaMap;
	}

private:
	using KeyType = typename ContainerType::key_type;
	using MappedType = typename ContainerType::mapped_type;
	using ValueType = typename ContainerType::value_type;

	static const MetaType * metaMapGetValueType(const Variant & /*mappable*/)
	{
		return getMetaType<ValueType>();
	}

	static Variant metaMapGet(const Variant & mappable, const Variant & key)
	{
		const auto & container = mappable.get<const ContainerType &>();
		auto it = container.find(key.get<const KeyType &>());
		if(it != container.end()) {
			return Variant::create<const MappedType &>(it->second);
		}
		return Variant();
	}

	static void metaMapSet(const Variant & mappable, const Variant & key, const Variant & value)
	{
		auto & container = mappable.get<ContainerType &>();
		const KeyType & nativeKey = key.get<const KeyType &>();
		const MappedType & nativeValue = value.get<const MappedType &>();
		auto it = container.find(nativeKey);
		if(it != container.end()) {
			internal_::assignValue(it->second, nativeValue);
		}
		else {
			container.insert(std::make_pair(nativeKey, nativeValue));
		}
	}

};


} // namespace metapp


#endif

