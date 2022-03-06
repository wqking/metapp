#ifndef METAPP_ARRAY_H_969872685611
#define METAPP_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename T, typename U, size_t length>
struct DeclareMetaTypeArrayBase
	: public DeclareMetaTypeBase <U>
{
	static constexpr TypeKind typeKind = tkArray;
	static constexpr TypeFlags typeFlags = DeclareMetaTypeBase<U>::typeFlags
		| (std::is_const<U>::value ? tfConst : 0)
		| (std::is_volatile<U>::value ? tfVolatile : 0)
	;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

private:
	using ElementType = typename std::remove_pointer<
			typename std::remove_cv<U>::type
		>::type;

	static size_t metaIndexableGetSize(const Variant & /*var*/)
	{
		return length;
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		return Variant::create<ElementType &>(var.get<U>()[index]);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		if(index >= metaIndexableGetSize(var)) {
			errorInvalidIndex();
		}
		else {
			assignValue(var.get<U>()[index], value.get<ElementType &>());
		}
	}

};

template <typename T>
struct DeclareMetaTypeBase <T[]>
	: public DeclareMetaTypeArrayBase <T[], T *, MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <T[N]>
	: public DeclareMetaTypeArrayBase <T[N], T *, N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const T[]>
	: public DeclareMetaTypeArrayBase <const T[], T * const, MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const T[N]>
	: public DeclareMetaTypeArrayBase <const T[N], T * const, N>
{
};

template <typename T>
struct DeclareMetaTypeBase <volatile T[]>
	: public DeclareMetaTypeArrayBase <volatile T[], T * volatile, MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <volatile T[N]>
	: public DeclareMetaTypeArrayBase <volatile T[N], T * volatile, N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T[]>
	: public DeclareMetaTypeArrayBase <const volatile T[], T * const volatile, MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const volatile T[N]>
	: public DeclareMetaTypeArrayBase <const volatile T[N], T * const volatile, N>
{
};


} // namespace metapp


#endif

