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
#include "metapp/utilities/utility.h"

#include <deque>
#include <vector>
#include <array>

namespace metapp {

namespace internal_ {

template <typename T, typename Allocator>
void doResize(std::deque<T, Allocator> & container, const size_t size)
{
	container.resize(size);
}

template <typename T, typename Allocator>
void doResize(std::vector<T, Allocator> & container, const size_t size)
{
	container.resize(size);
}

template <typename T, size_t length>
void doResize(std::array<T, length> & /*container*/, const size_t /*size*/)
{
}

} // namespace internal_

template <typename ContainerType>
struct MetaIndexableBase
{
	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSizeInfo,
			&metaIndexableGetValueType,
			&metaIndexableResize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

private:
	using ValueType = decltype(std::declval<ContainerType &>()[0]);

	static MetaIndexable::SizeInfo metaIndexableGetSizeInfo(const Variant & indexable)
	{
		return MetaIndexable::SizeInfo { indexable.get<ContainerType &>().size() };
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*indexable*/, const size_t /*index*/)
	{
		return getMetaType<ValueType>();
	}

	static void metaIndexableResize(const Variant & indexable, const size_t size)
	{
		internal_::doResize(indexable.get<ContainerType &>(), size);
	}

	static Variant metaIndexableGet(const Variant & indexable, const size_t index)
	{
		if(index >= metaIndexableGetSizeInfo(indexable).getSize()) {
			errorInvalidIndex();
		}
		return Variant::create<ValueType>(indexable.get<ContainerType &>()[index]);
	}

	static void metaIndexableSet(const Variant & indexable, const size_t index, const Variant & value)
	{
		internal_::verifyVariantWritable(indexable);

		if(index >= metaIndexableGetSizeInfo(indexable).getSize()) {
			errorInvalidIndex();
		}
		else {
			internal_::assignValue(indexable.get<ContainerType &>()[index], value.cast<ValueType &>().template get<ValueType &>());
		}
	}

};


} // namespace metapp


#endif

