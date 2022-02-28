#ifndef METATYPEDATA_H_969872685611
#define METATYPEDATA_H_969872685611

#include "metapp/exception.h"
#include "metapp/internal/typeutil_i.h"

#include <memory>
#include <array>
#include <type_traits>

namespace metapp {

constexpr size_t podSize = internal_::MaxOfInt<
		sizeof(long long),
		sizeof(long double),
		sizeof(void *)
	>::value;

struct MetaTypeData
{
	std::shared_ptr<void> object;
	std::array<uint8_t, podSize + 1> pod;

	template <typename T>
	T & podAs() const {
		return *(T *)(pod.data());
	}

	bool isObjectStorage() const {
		return pod[podSize] != 0;
	}
	void setObjectStorage(const bool value) {
		pod[podSize] = (value ? 1 : 0);
	}

	template <typename T>
	void construct(const void * copyFrom) {
		if(fitPod<T>()) {
			constructPod<T>(copyFrom);
		}
		else {
			constructObject<T>(copyFrom);
		}
	}

	void * getAddress() const {
		if(isObjectStorage()) {
			return (void *)(object.get());
		}
		else {
			return (void *)(pod.data());
		}
	}

private:
	template <typename T>
	void constructPod(const void * copyFrom) {
		if(copyFrom == nullptr) {
			podAs<T>() = T();
		}
		else {
			podAs<T>() = *(T *)copyFrom;
		}
		setObjectStorage(false);
	}

	template <typename T>
	void constructObject(const void * copyFrom) {
		if(copyFrom == nullptr) {
			object = std::make_shared<T>();
		}
		else {
			object = std::make_shared<T>(*(T *)copyFrom);
		}
		setObjectStorage(true);
	}

	template <typename T>
	static constexpr bool fitPod() {
		return std::is_trivial<T>::value
			&& std::is_standard_layout<T>::value
			&& sizeof(T) <= podSize
		;
	}
};


} // namespace metapp

#endif
