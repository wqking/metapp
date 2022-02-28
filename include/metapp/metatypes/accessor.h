#ifndef ACCESSOR_H_969872685611
#define ACCESSOR_H_969872685611

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
struct DeclareMetaType <accessorpp::Accessor<T, Policies> >
	: public DeclareMetaTypeBase <accessorpp::Accessor<T, Policies> >
{
private:
	using super = DeclareMetaTypeBase <accessorpp::Accessor<T, Policies> >;
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

