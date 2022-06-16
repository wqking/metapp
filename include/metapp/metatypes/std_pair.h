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

#ifndef METAPP_STD_PAIR_H_969872685611
#define METAPP_STD_PAIR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/interfaces/metaiterable.h"
#include "metapp/utilities/utility.h"

#include <utility>

namespace metapp {

template <typename T1, typename T2>
struct DeclareMetaTypeBase <std::pair<T1, T2> >
{
	using UpType = TypeList<T1, T2>;
	static constexpr TypeKind typeKind = tkStdPair;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSizeInfo,
			&metaIndexableGetValueType,
			nullptr,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

	static const MetaIterable * getMetaIterable() {
		static MetaIterable metaIterable(
			&metaIterableForEach
		);
		return &metaIterable;
	}

private:
	using PairType = std::pair<T1, T2>;

	static MetaIndexable::SizeInfo metaIndexableGetSizeInfo(const Variant & /*var*/)
	{
		MetaIndexable::SizeInfo sizeInfo { 2 };
		sizeInfo.setResizable(false);
		return sizeInfo;
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*var*/, const std::size_t index)
	{
		if(index == 0) {
			return getMetaType<T1>();
		}
		return getMetaType<T2>();
	}

	static Variant metaIndexableGet(const Variant & var, const std::size_t index)
	{
		if(index == 0) {
			return Variant::reference(var.get<PairType &>().first);
		}
		return Variant::reference(var.get<PairType &>().second);
	}

	static void metaIndexableSet(const Variant & var, const std::size_t index, const Variant & value)
	{
		if(index >= 2) {
			raiseException<OutOfRangeException>();
		}
		else {
			if(index == 0) {
				internal_::assignValue(var.get<PairType &>().first, value.cast<T1 &>().template get<T1 &>());
			}
			else {
				internal_::assignValue(var.get<PairType &>().second, value.cast<T2 &>().template get<T2 &>());
			}
		}
	}

	static void metaIterableForEach(const Variant & var, const MetaIterable::Callback & callback)
	{
		if(callback(Variant::reference(var.get<PairType &>().first))) {
			callback(Variant::reference(var.get<PairType &>().second));
		}
	}

};


} // namespace metapp


#endif

