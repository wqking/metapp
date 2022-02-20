#ifndef POINTER_H_969872685611
#define POINTER_H_969872685611

#include "metapp/metatype.h"
#include "metapp/utility.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T *> : public DeclarePodMetaType<T *>
{
	using UpType = T;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer;
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		return Variant(toMetaType, value.getMetaTypeData());
	}

	static Variant accessibleGet(const Variant & accessible, const void * instance) {
		return doAccessibleGet<T>(accessible, instance);
	}
	
	static void accessibleSet(const Variant & accessible, void * instance, const Variant & value) {
		doAccessibleSet<T>(accessible, instance, value);
	}

private:
	template <typename U>
	static Variant doAccessibleGet(
			const Variant & /*accessible*/,
			const void * /*instance*/,
			typename std::enable_if<std::is_void<U>::value>::type * = nullptr
		) {
		return Variant();
	}

	template <typename U>
	static Variant doAccessibleGet(
			const Variant & accessible,
			const void * /*instance*/,
			typename std::enable_if<! std::is_void<U>::value>::type * = nullptr
		) {
		return *accessible.get<U *>();
	}

	template <typename U>
	static void doAccessibleSet(
			const Variant & /*accessible*/,
			void * /*instance*/,
			const Variant & /*value*/,
			//typename std::enable_if<std::is_void<U>::value || IsPointerToConst<U>::value>::type * = nullptr
			typename std::enable_if<! AccessibleIsAssignable<U *>::value>::type * = nullptr
		) {
	}

	template <typename U>
	static void doAccessibleSet(
			const Variant & accessible,
			void * /*instance*/,
			const Variant & value,
			//typename std::enable_if<! std::is_void<U>::value && ! IsPointerToConst<U>::value>::type * = nullptr
			typename std::enable_if<AccessibleIsAssignable<U *>::value>::type * = nullptr
		) {
		*(accessible.get<U *>()) = value.cast<U>().template get<const U &>();
	}

};

template <>
struct DeclareMetaTypeBase <std::nullptr_t> : public DeclareMetaTypeBase<void *>
{
};


} // namespace metapp


#endif

