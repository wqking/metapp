#ifndef METAPP_MEMBER_DATA_H_969872685611
#define METAPP_MEMBER_DATA_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaaccessible.h"
#include "metapp/interfaces/bases/metamemberbase.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename Class, typename T>
struct DeclareMetaTypeBase <T Class::*, typename std::enable_if<! std::is_function<T>::value>::type>
	: public DeclareMetaTypeObject<T Class::*>,
		public MetaMemberBase<Class>
{
	using UpType = TypeList <Class, T>;
	static constexpr TypeKind typeKind = tkMemberPointer;

	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

	static Variant accessibleGet(const Variant & accessible, const void * instance) {
		return ((const Class *)instance)->*(accessible.get<T Class::*>());
	}

	static void accessibleSet(const Variant & accessible, void * instance, const Variant & value) {
		assignValue(((Class *)instance)->*(accessible.get<T Class::*>()), value.cast<T>().template get<const T &>());
	}

};


} // namespace metapp


#endif

