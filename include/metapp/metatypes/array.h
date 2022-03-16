#ifndef METAPP_ARRAY_H_969872685611
#define METAPP_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename T, size_t length>
struct DeclareMetaTypeArrayBase
{
	using UpType = typename std::remove_extent<typename std::remove_cv<T>::type>::type;

	static constexpr TypeKind typeKind = tkArray;
	static constexpr TypeFlags typeFlags = 0
		| (std::is_const<T>::value ? tfConst : 0)
		| (std::is_volatile<T>::value ? tfVolatile : 0)
	;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		return doConstructData<length != MetaIndexable::unknowSize>(data, copyFrom);
	}

private:
	using Common = CommonDeclareMetaType<T &>;
	struct ArrayWrapper
	{
		T data;
	};

	template <bool hasLength>
	static void * doConstructData(MetaTypeData * data, const void * copyFrom, typename std::enable_if<hasLength>::type * = nullptr) {
		if(data != nullptr && copyFrom != nullptr) {
			data->construct<ArrayWrapper>(*(ArrayWrapper **)copyFrom);
		}
		else {
			return Common::constructData(data, copyFrom);
		}
		return nullptr;
	}

	template <bool hasLength>
	static void * doConstructData(MetaTypeData * /*data*/, const void * /*copyFrom*/, typename std::enable_if<! hasLength>::type * = nullptr) {
		errorNotConstructible();
		return nullptr;
	}

	using ElementType = typename std::remove_extent<
			typename std::remove_cv<T>::type
		>::type;

	static size_t metaIndexableGetSize(const Variant & /*var*/)
	{
		return length;
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		return Variant::create<ElementType &>(var.get<T &>()[index]);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		verifyVariantWritable(var);

		if(index >= metaIndexableGetSize(var)) {
			errorInvalidIndex();
		}
		else {
			assignValue(var.get<T &>()[index], value.cast<ElementType>().template get<ElementType &>());
		}
	}

};

template <typename T>
struct DeclareMetaTypeBase <T[]>
	: public DeclareMetaTypeArrayBase <T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <T[N]>
	: public DeclareMetaTypeArrayBase <T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const T[]>
	: public DeclareMetaTypeArrayBase <const T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const T[N]>
	: public DeclareMetaTypeArrayBase <const T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <volatile T[]>
	: public DeclareMetaTypeArrayBase <volatile T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <volatile T[N]>
	: public DeclareMetaTypeArrayBase <volatile T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T[]>
	: public DeclareMetaTypeArrayBase <const volatile T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const volatile T[N]>
	: public DeclareMetaTypeArrayBase <const volatile T[N], N>
{
};


} // namespace metapp


#endif

