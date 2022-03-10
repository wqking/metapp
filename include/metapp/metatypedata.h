#ifndef METAPP_METATYPEDATA_H_969872685611
#define METAPP_METATYPEDATA_H_969872685611

#include "metapp/exception.h"
#include "metapp/implement/internal/typeutil_i.h"

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

	template <typename T>
	struct FitBuffer
	{
		static constexpr bool value = 
			std::is_trivial<T>::value
			&& std::is_standard_layout<T>::value
			&& sizeof(T) <= bufferSize
		;
	};

	static constexpr uint8_t storageNone = 0;
	static constexpr uint8_t storageObject = 1;
	static constexpr uint8_t storageBuffer = 2;
	static constexpr uint8_t storageSharedPtr = 3;
	static constexpr uint8_t storageReference = 4;

public:
	MetaTypeData()
		: object(), buffer()
	{
	}

	template <typename T>
	void construct(const void * copyFrom) {
		if(FitBuffer<T>::value) {
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

	void constructReference(const void * copyFrom) {
		podAs<void *>() = *(void **)&copyFrom;
		setStorageType(storageReference);
	}

	void * getAddress() const {
		switch(getStorageType()) {
		case storageObject:
			return (void *)(object.get());

		case storageBuffer:
			return (void *)(buffer.data());

		case storageSharedPtr:
			return (void *)&object;

		case storageReference:
			return *(void **)(buffer.data());
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
			doConstructOnBufferDefault<T>();
		}
		else {
			doConstructOnBufferCopy<T>(copyFrom);
		}
		setStorageType(storageBuffer);
	}

	template <typename T>
	void doConstructOnBufferDefault(
		typename std::enable_if<std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value>::type * = nullptr
		) {
		podAs<T>() = T();
	}

	template <typename T>
	void doConstructOnBufferDefault(
		typename std::enable_if<! (std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value)>::type * = nullptr
		) {
	}

	template <typename T>
	void doConstructOnBufferCopy(
		const void * copyFrom,
		typename std::enable_if<std::is_copy_assignable<T>::value>::type * = nullptr
	) {
		podAs<T>() = *(T *)copyFrom;
	}

	template <typename T>
	void doConstructOnBufferCopy(
		const void * /*copyFrom*/,
		typename std::enable_if<! std::is_copy_assignable<T>::value>::type * = nullptr
	) {
	}

	template <typename T>
	void doConstructOnObject(const void * copyFrom) {
		setStorageType(storageObject);

		if(copyFrom == nullptr) {
			doConstructOnObjectDefault<T>();
		}
		else {
			doConstructOnObjectCopy<T>(copyFrom);
		}
	}

	template <typename T>
	void doConstructOnObjectDefault(
		typename std::enable_if<std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value>::type * = nullptr
	) {
		object = std::make_shared<T>();
	}

	template <typename T>
	void doConstructOnObjectDefault(
		typename std::enable_if<! (std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value)>::type * = nullptr
	) {
		errorNotConstructible();
	}

	template <typename T>
	void doConstructOnObjectCopy(
		const void * copyFrom,
		typename std::enable_if<std::is_copy_assignable<T>::value>::type * = nullptr
	) {
		object = std::make_shared<T>(*(T *)copyFrom);
	}

	template <typename T>
	void doConstructOnObjectCopy(
		const void * copyFrom,
		typename std::enable_if<! std::is_copy_assignable<T>::value>::type * = nullptr
	) {
		doConstructOnObjectMove<T>(copyFrom);
	}

	template <typename T>
	void doConstructOnObjectMove(
		const void * copyFrom,
		typename std::enable_if<std::is_move_assignable<T>::value>::type * = nullptr
	) {
		object = std::make_shared<T>(std::move(*(T *)copyFrom));
	}

	template <typename T>
	void doConstructOnObjectMove(
		const void * /*copyFrom*/,
		typename std::enable_if<! std::is_move_assignable<T>::value>::type * = nullptr
	) {
		errorNotConstructible();
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
