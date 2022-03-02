#ifndef REFERENCE_H_969872685611
#define REFERENCE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metaaccessible.h"

#include <functional>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T &> : public DeclareMetaTypeObject<T &>
{
private:
	using super = DeclareMetaTypeObject<T &>;

public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->construct<T *>(&copyFrom);
		}
		return nullptr;
	}

	static void * getAddress(const MetaTypeData & data) {
		return *(void **)data.getAddress();
	}

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return (toMetaType->getTypeKind() == tkReference) || super::canCast(value, toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->getTypeKind() == tkReference) {
			return Variant(toMetaType, &value.get<int &>());
		}
		else {
			return super::cast(value, toMetaType);
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
	using WrapperType = std::reference_wrapper<T>;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			const void * p = &(T &)*(WrapperType *)copyFrom;
			data->construct<T *>(&p);
		}
		return nullptr;
	}

};


} // namespace metapp


#endif

