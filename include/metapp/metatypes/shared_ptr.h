#ifndef SHARED_PTR_H_969872685611
#define SHARED_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::shared_ptr<T> >
	: DeclareMetaTypeRoot<std::shared_ptr<T> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkSharedPtr;

	static void * construct(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			if(copyFrom == nullptr) {
				data->object = std::static_pointer_cast<void>(std::shared_ptr<T>());
			}
			else {
				data->object = std::static_pointer_cast<void>(*(std::shared_ptr<T> *)copyFrom);
			}
			return nullptr;
		}
		else {
			if(copyFrom == nullptr) {
				return new std::shared_ptr<T>();
			}
			else {
				return new std::shared_ptr<T>(*(std::shared_ptr<T> *)copyFrom);
			}
		}
	}

	static void * getAddress(const MetaTypeData & data) {
		return (void *)&data.object;
	}

};


} // namespace metapp


#endif

