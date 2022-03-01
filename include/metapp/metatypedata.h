#ifndef METATYPEDATA_H_969872685611
#define METATYPEDATA_H_969872685611

#include "metapp/exception.h"
#include "metapp/internal/typeutil_i.h"

#include <memory>
#include <array>
#include <type_traits>

namespace metapp {

class MetaTypeData
{
private:
	static constexpr size_t bufferSize = internal_::MaxOfInt<
		sizeof(long long),
		sizeof(long double),
		sizeof(void *)
	>::value;

	static constexpr uint8_t storageNone = 0;
	static constexpr uint8_t storageObject = 1;
	static constexpr uint8_t storageBuffer = 2;
	static constexpr uint8_t storageSharedPtr = 3;

public:
	MetaTypeData()
		: object(), buffer()
	{
	}

	template <typename T>
	void construct(const void * copyFrom) {
		if(fitBuffer<T>()) {
			doConstructOnBuffer<T>(copyFrom);
		}
		else {
			doConstructOnObject<T>(copyFrom);
		}
	}

	void constructObject(const std::shared_ptr<void> & obj) {
		object = obj;
		setStorageType(storageObject);
	}

	void constructSharedPtr(const std::shared_ptr<void> & sharedPtr) {
		object = sharedPtr;
		setStorageType(storageSharedPtr);
	}

	void * getAddress() const {
		switch(getStorageType()) {
		case storageObject:
			return (void *)(object.get());

		case storageBuffer:
			return (void *)(buffer.data());

		case storageSharedPtr:
			return (void *)&object;
		}

		return nullptr;
	}

	void reset() {
		setStorageType(storageNone);
		object.reset();
	}

private:
	template <typename T>
	void doConstructOnBuffer(const void * copyFrom) {
		if(copyFrom == nullptr) {
			podAs<T>() = T();
		}
		else {
			podAs<T>() = *(T *)copyFrom;
		}
		setStorageType(storageBuffer);
	}

	template <typename T>
	void doConstructOnObject(const void * copyFrom) {
		if(copyFrom == nullptr) {
			object = std::make_shared<T>();
		}
		else {
			object = std::make_shared<T>(*(T *)copyFrom);
		}
		setStorageType(storageObject);
	}

	template <typename T>
	static constexpr bool fitBuffer() {
		return std::is_trivial<T>::value
			&& std::is_standard_layout<T>::value
			&& sizeof(T) <= bufferSize
		;
	}

	template <typename T>
	T & podAs() const {
		return *(T *)(buffer.data());
	}

	uint8_t getStorageType() const {
		return buffer[bufferSize];
	}
	void setStorageType(const uint8_t value) {
		buffer[bufferSize] = value;
	}


private:
	std::shared_ptr<void> object;
	std::array<uint8_t, bufferSize + 1> buffer;
};


} // namespace metapp

#endif
