// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef METAPP_METATYPEDATA_H_969872685611
#define METAPP_METATYPEDATA_H_969872685611

#include "metapp/exception.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/implement/internal/construct_i.h"

#include <memory>
#include <array>
#include <type_traits>

namespace metapp {

namespace internal_ {

template <typename T>
constexpr T maxOf(T a, T b)
{
	return a > b ? a : b;
}

} // namespace internal_

class MetaTypeData
{
private:
	static constexpr size_t bufferSize = internal_::maxOf(sizeof(long long), internal_::maxOf(sizeof(long double), sizeof(void *)));
	static_assert(bufferSize >= sizeof(int), "MetaTypeData, wrong bufferSize");

	template <typename T>
	using FitBuffer = std::integral_constant<bool,
		std::is_trivial<T>::value
		&& std::is_standard_layout<T>::value
		&& sizeof(T) <= bufferSize
	>;

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
		doConstructOnObjectOrBuffer<T>(copyFrom, FitBuffer<T>());
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
		podAs<const void *>() = copyFrom;
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

	void swap(MetaTypeData & other) noexcept {
		object.swap(other.object);
		buffer.swap(other.buffer);
	}

private:
	template <typename T>
	void doConstructOnObjectOrBuffer(const void * copyFrom, std::true_type) {
		setStorageType(storageBuffer);

		if(copyFrom == nullptr) {
			doConstructOnBufferDefault<T>(
				internal_::TrueFalse<std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value>()
			);
		}
		else {
			doConstructOnBufferCopy<T>(copyFrom, std::is_copy_assignable<T>());
		}
	}

	template <typename T>
	void doConstructOnObjectOrBuffer(const void * copyFrom, std::false_type) {
		setStorageType(storageObject);
		object = std::shared_ptr<T>(internal_::constructOnHeap<T>(copyFrom));
	}

	template <typename T>
	void doConstructOnBufferDefault(std::true_type) {
		podAs<T>() = T();
	}

	template <typename T>
	void doConstructOnBufferDefault(std::false_type) {
		errorNotConstructible();
	}

	template <typename T>
	void doConstructOnBufferCopy(const void * copyFrom, std::true_type) {
		podAs<T>() = *(T *)copyFrom;
	}

	template <typename T>
	void doConstructOnBufferCopy(const void * /*copyFrom*/, std::false_type) {
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

inline void swap(MetaTypeData & a, MetaTypeData & b) noexcept
{
	a.swap(b);
}


} // namespace metapp

#endif
