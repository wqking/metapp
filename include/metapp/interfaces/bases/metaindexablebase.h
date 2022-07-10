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

template <typename T>
struct ResizeHelper
{
	using ContainerType = T;

	static constexpr bool resizable = true;

	static void resize(const Variant & indexable, const std::size_t size)
	{
		indexable.get<ContainerType &>().resize(size);
	}
};

template <typename T, std::size_t length>
struct ResizeHelper <std::array<T, length> >
{
	static constexpr bool resizable = false;

	static void resize(const Variant & /*indexable*/, const std::size_t /*size*/)
	{
	}
};

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
	using ValueType = typename ContainerType::value_type;

	static MetaIndexable::SizeInfo metaIndexableGetSizeInfo(const Variant & indexable)
	{
		MetaIndexable::SizeInfo sizeInfo { indexable.get<ContainerType &>().size() };
		sizeInfo.setResizable(internal_::ResizeHelper<ContainerType>::resizable);
		return sizeInfo;
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*indexable*/, const std::size_t /*index*/)
	{
		return getMetaType<ValueType>();
	}

	static void metaIndexableResize(const Variant & indexable, const std::size_t size)
	{
		internal_::ResizeHelper<ContainerType>::resize(indexable, size);
	}

	static Variant metaIndexableGet(const Variant & indexable, const std::size_t index)
	{
		if(index >= metaIndexableGetSizeInfo(indexable).getSize()) {
			raiseException<OutOfRangeException>();
		}
		return Variant::reference(indexable.get<ContainerType &>()[index]);
	}

	static void metaIndexableSet(const Variant & indexable, const std::size_t index, const Variant & value)
	{
		requireMutable(indexable);

		if(index >= metaIndexableGetSizeInfo(indexable).getSize()) {
			raiseException<OutOfRangeException>();
		}
		else {
			internal_::assignValue(indexable.get<ContainerType &>()[index], value.cast<ValueType &>().template get<ValueType &>());
		}
	}

};


} // namespace metapp


#endif

