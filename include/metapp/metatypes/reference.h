#ifndef METAPP_REFERENCE_H_969872685611
#define METAPP_REFERENCE_H_969872685611

#include "metapp/metatype.h"

#include <functional>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T &>
{
	using Common = CommonDeclareMetaType<T &>;
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->constructReference(copyFrom);
		}
		return nullptr;
	}

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return (toMetaType->getTypeKind() == tkReference) || Common::canCast(value, toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->getTypeKind() == tkReference) {
			return Variant(toMetaType, &value.get<int &>());
		}
		else {
			return Common::cast(value, toMetaType);
		}
	}

};

template <typename T>
struct DeclareMetaTypeBase <T &&> : public DeclareMetaTypeBase<T &>
{
};

template <typename T>
struct DeclareMetaTypeBase <std::reference_wrapper<T> >
	: public DeclareMetaTypeBase<T &>
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;
	using Common = CommonDeclareMetaType<T &>;
	static constexpr TypeFlags typeFlags = tfReference | Common::typeFlags;

	using WrapperType = std::reference_wrapper<T>;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->constructReference(&(T &)*(WrapperType *)copyFrom);
		}
		return nullptr;
	}

};


} // namespace metapp


#endif

