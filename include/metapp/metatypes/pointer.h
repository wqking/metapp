#ifndef METAPP_POINTER_H_969872685611
#define METAPP_POINTER_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/declareutil.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypePointerBase
{
	using Fallback = DeclareMetaTypeObject<T>;
	using UpType = typename std::remove_pointer<T>::type;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer || Fallback::canCast(value, toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->getTypeKind() == tkPointer) {
			return Variant(toMetaType, &value.get<void * &>());
		}
		else {
			return Fallback::cast(value, toMetaType);
		}
	}

};

template <typename T>
struct DeclareMetaTypeBase <T *> : public DeclareMetaTypePointerBase<T *>
{
private:
	static_assert(! std::is_void<T>::value, "T should not be void in T * specialization.");

public:
	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

	static Variant accessibleGet(const Variant & accessible, const void * /*instance*/) {
		return *accessible.get<T *>();
	}

	static void accessibleSet(const Variant & accessible, void * /*instance*/, const Variant & value) {
		assignValue(*(accessible.get<T *>()), value.cast<T>().template get<const T &>());
	}

};

template <typename T>
struct DeclareMetaTypeVoidPtrBase : public DeclareMetaTypePointerBase<T>
{
public:
};

template <>
struct DeclareMetaTypeBase <void *> : public DeclareMetaTypeVoidPtrBase<void *>
{
};

template <>
struct DeclareMetaTypeBase <const void *> : public DeclareMetaTypeVoidPtrBase<const void *>
{
};

template <>
struct DeclareMetaTypeBase <volatile void *> : public DeclareMetaTypeVoidPtrBase<volatile void *>
{
};

template <>
struct DeclareMetaTypeBase <const volatile void *> : public DeclareMetaTypeVoidPtrBase<const volatile void *>
{
};

template <>
struct DeclareMetaTypeBase <std::nullptr_t> : public DeclareMetaTypeBase<void *>
{
};


} // namespace metapp


#endif

