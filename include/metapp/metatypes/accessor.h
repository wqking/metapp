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
#include "metapp/variant.h"
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
	return accessorpp::createAccessor<T>(std::forward<G>(getter), std::forward<S>(setter), internal_::AccessorPolicies());
}

template <typename G, typename S>
auto createAccessor(G && getter, S && setter)
	-> decltype(accessorpp::createAccessor(std::forward<G>(getter), std::forward<S>(setter), internal_::AccessorPolicies()))
{
	return accessorpp::createAccessor(std::forward<G>(getter), std::forward<S>(setter), internal_::AccessorPolicies());
}

template <typename T, typename G>
Accessor<T> createReadOnlyAccessor(G && getter)
{
	return accessorpp::createReadOnlyAccessor<T>(std::forward<G>(getter), internal_::AccessorPolicies());
}

template <typename G>
auto createReadOnlyAccessor(G && getter)
	-> decltype(accessorpp::createReadOnlyAccessor(std::forward<G>(getter), accessorpp::noSetter, internal_::AccessorPolicies()))
{
	return accessorpp::createReadOnlyAccessor(std::forward<G>(getter), internal_::AccessorPolicies());
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
			&accessibleGetValueType,
			&accessibleIsReadOnly,
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

	static const MetaType * accessibleGetValueType(const Variant & /*accessible*/) {
		return getMetaType<T>();
	}

	static bool accessibleIsReadOnly(const Variant & accessible) {
		AccessorType & accessor = accessible.get<AccessorType &>();
		return accessor.isReadOnly();
	}

	static Variant accessibleGet(const Variant & accessible, const void * instance) {
		const AccessorType & accessor = accessible.get<const AccessorType &>();
		return Variant::create<T>(accessor.get(instance));
	}

	static void accessibleSet(const Variant & accessible, void * instance, const Variant & value) {
		AccessorType & accessor = accessible.get<AccessorType &>();
		if(accessor.isReadOnly()) {
			errorUnwritable();
			return;
		}
		accessor.set(value.cast<T>().template get<T>(), instance);
	}

};


} // namespace metapp


#endif

