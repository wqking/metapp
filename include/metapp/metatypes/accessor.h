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

#ifndef METAPP_ACCESSOR_H_969872685611
#define METAPP_ACCESSOR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/thirdparty/accessorpp/accessor.h"

namespace metapp {

namespace internal_ {

struct AccessorPolicies
{
	using Storage = accessorpp::ExternalStorage;
};

} // namespace internal_

template <typename T>
using Accessor = accessorpp::Accessor <T, internal_::AccessorPolicies>; 

template <typename T, typename G, typename S>
Accessor<T> createAccessor(G && getter, S && setter)
{
	return Accessor<T>(std::forward<G>(getter), std::forward<S>(setter));
}

template <typename T, typename G>
Accessor<T> createReadOnlyAccessor(G && getter)
{
	return Accessor<T>(std::forward<G>(getter), accessorpp::noSetter);
}

template <typename T, typename Policies>
struct DeclareMetaTypeBase <accessorpp::Accessor<T, Policies> >
{
private:
	using AccessorType = accessorpp::Accessor<T, Policies>;

public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkAccessor;

	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

	static Variant accessibleGet(const Variant & accessible, const void * instance) {
		const AccessorType & accessor = accessible.get<const AccessorType &>();
		return accessor.get(instance);
	}

	static void accessibleSet(const Variant & accessible, void * instance, const Variant & value) {
		AccessorType & accessor = accessible.get<AccessorType &>();
		if(accessor.isReadOnly()) {
			return;
		}
		accessor.set(value.cast<T>().template get<T>(), instance);
	}

};


} // namespace metapp


#endif

