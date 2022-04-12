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
			&metaIndexableGetSize,
			&metaIndexableGetValueType,
			nullptr,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

private:
	using PairType = std::pair<T1, T2>;

	static size_t metaIndexableGetSize(const Variant & /*var*/)
	{
		return 2;
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*var*/, const size_t index)
	{
		if(index == 0) {
			return getMetaType<T1>();
		}
		return getMetaType<T2>();
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		if(index == 0) {
			return Variant::create<T1 &>(var.get<PairType &>().first);
		}
		return Variant::create<T2 &>(var.get<PairType &>().second);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		if(index >= 2) {
			errorInvalidIndex();
		}
		else {
			if(index == 0) {
				internal_::assignValue(var.get<PairType &>().first, value.get<T1 &>());
			}
			else {
				internal_::assignValue(var.get<PairType &>().second, value.get<T2 &>());
			}
		}
	}

};


} // namespace metapp


#endif

