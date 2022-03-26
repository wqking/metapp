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
		doConstructOnObjectOrBuffer<T>(FitBuffer<T>::value, copyFrom);
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

	void swap(MetaTypeData & other) noexcept {
		object.swap(other.object);
		buffer.swap(other.buffer);
	}

private:
	template <typename T>
	void doConstructOnObjectOrBuffer(const bool onBuffer, const void * copyFrom) {
		setStorageType(onBuffer ? storageBuffer : storageObject);

		if(copyFrom == nullptr) {
			doConstructOnObjectOrBufferDefault<T>(onBuffer);
		}
		else {
			doConstructOnObjectOrBufferCopy<T>(onBuffer, copyFrom);
		}
	}

	template <typename T>
	void doConstructOnObjectOrBufferDefault(
		const bool onBuffer,
		typename std::enable_if<
		std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value
		>::type * = nullptr
	) {
		if(onBuffer) {
			podAs<T>() = T();
		}
		else {
			object = std::make_shared<T>();
		}
	}

	template <typename T>
	void doConstructOnObjectOrBufferDefault(
		const bool /*onBuffer*/,
		typename std::enable_if<
		! (std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value)
		>::type * = nullptr
	) {
		errorNotConstructible();
	}

	template <typename T>
	void doConstructOnObjectOrBufferCopy(
		const bool onBuffer,
		const void * copyFrom,
		typename std::enable_if<std::is_copy_assignable<T>::value>::type * = nullptr
	) {
		if(onBuffer) {
			podAs<T>() = *(T *)copyFrom;
		}
		else {
			object = std::make_shared<T>(*(T *)copyFrom);
		}
	}

	template <typename T>
	void doConstructOnObjectOrBufferCopy(
		const bool onBuffer,
		const void * copyFrom,
		typename std::enable_if<! std::is_copy_assignable<T>::value>::type * = nullptr
	) {
		doConstructOnObjectOrBufferMove<T>(onBuffer, copyFrom);
	}

	template <typename T>
	void doConstructOnObjectOrBufferMove(
		const bool onBuffer,
		const void * copyFrom,
		typename std::enable_if<std::is_move_assignable<T>::value>::type * = nullptr
	) {
		if(onBuffer) {
			podAs<T>() = std::move(*(T *)copyFrom);
		}
		else {
			object = std::make_shared<T>(std::move(*(T *)copyFrom));
		}
	}

	template <typename T>
	void doConstructOnObjectOrBufferMove(
		const bool /*onBuffer*/,
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

inline void swap(MetaTypeData & a, MetaTypeData & b) noexcept
{
	a.swap(b);
}


} // namespace metapp

#endif
