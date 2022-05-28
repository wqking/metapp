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
#include "metapp/interfaces/metaaccessible.h"
#include "metapp/thirdparty/accessorpp/accessor.h"
#include "metapp/utilities/utility.h"

namespace metapp {

namespace internal_ {

class AccessorClassTypeSetter
{
public:
	AccessorClassTypeSetter()
		: classMetaType(voidMetaType)
	{
	}

	const MetaType * getClassMetaType() const {
		return classMetaType;
	}

protected:
	template <typename C>
	void setClassType() {
		classMetaType = getMetaType<C>();
	}

private:
	const MetaType * classMetaType;
};

struct AccessorPolicies
{
	using Storage = accessorpp::ExternalStorage;
	using ClassTypeSetter = AccessorClassTypeSetter;
};

} // namespace internal_

template <typename T>
using Accessor = accessorpp::Accessor <T, internal_::AccessorPolicies>; 

template <typename T, typename G, typename S>
Accessor<T> createAccessor(G && getter, S && setter)
{
	return accessorpp::createAccessor<T>(std::forward<G>(getter), std::forward<S>(setter), internal_::AccessorPolicies());
}

template <typename T, typename GS>
Accessor<T> createAccessor(GS && getterSetter)
{
	return accessorpp::createAccessor<T>(std::forward<GS>(getterSetter), std::forward<GS>(getterSetter), internal_::AccessorPolicies());
}

template <typename G, typename S>
auto createAccessor(G && getter, S && setter)
	-> decltype(accessorpp::createAccessor(std::forward<G>(getter), std::forward<S>(setter), internal_::AccessorPolicies()))
{
	return accessorpp::createAccessor(std::forward<G>(getter), std::forward<S>(setter), internal_::AccessorPolicies());
}

template <typename GS>
auto createAccessor(GS && getterSetter)
	-> decltype(accessorpp::createAccessor(std::forward<GS>(getterSetter), std::forward<GS>(getterSetter), internal_::AccessorPolicies()))
{
	return accessorpp::createAccessor(std::forward<GS>(getterSetter), std::forward<GS>(getterSetter), internal_::AccessorPolicies());
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

template <typename T>
struct DeclareMetaTypeBase <Accessor<T> >
{
private:
	using AccessorType = Accessor<T>;

public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkAccessor;

	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGetValueType,
			&accessibleIsReadOnly,
			&accessibleGetClassType,
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

	static const MetaType * accessibleGetClassType(const Variant & accessible) {
		AccessorType & accessor = accessible.get<AccessorType &>();
		return accessor.getGetter().getClassMetaType();
	}

	static Variant accessibleGet(const Variant & accessible, const Variant & instance) {
		const AccessorType & accessor = accessible.get<const AccessorType &>();
		return Variant::create<T>(accessor.get(getPointer(instance)));
	}

	static void accessibleSet(const Variant & accessible, const Variant & instance, const Variant & value) {
		AccessorType & accessor = accessible.get<AccessorType &>();
		if(accessor.isReadOnly()) {
			raiseException<UnwritableException>();
			return;
		}
		accessor.set(value.cast<T>().template get<T>(), getPointer(instance));
	}

};


} // namespace metapp


#endif

