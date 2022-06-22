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

#ifndef METAPP_CONSTRUCT_I_H_969872685611
#define METAPP_CONSTRUCT_I_H_969872685611

#include "metapp/exception.h"

#include <type_traits>
#include <memory>

namespace metapp {

enum class CopyStrategy
{
	autoDetect,
	copy,
	move
};

namespace internal_ {

template <typename T>
T * constructOnHeapDefault(void * memory, std::true_type)
{
	if(memory == nullptr) {
		return new T();
	}
	else {
		return new (memory) T();
	}
}

template <typename T>
T * constructOnHeapDefault(void * /*memory*/, std::false_type)
{
	raiseException<NotConstructibleException>();
	return nullptr;
}

template <typename T, typename U>
T * constructOnHeapAutoDetect(const void * copyFrom, void * memory, std::true_type, U)
{
	if(memory == nullptr) {
		return new T(*(T *)copyFrom);
	}
	else {
		return new (memory) T(*(T *)copyFrom);
	}
}

template <typename T>
T * constructOnHeapAutoDetect(const void * copyFrom, void * memory, std::false_type, std::true_type)
{
	if(memory == nullptr) {
		return new T(std::move(*(T *)copyFrom));
	}
	else {
		return new (memory) T(std::move(*(T *)copyFrom));
	}
}

template <typename T>
T * constructOnHeapAutoDetect(const void * /*copyFrom*/, void * /*memory*/, std::false_type, std::false_type)
{
	raiseException<NotConstructibleException>();
	return nullptr;
}

template <typename T>
T * constructOnHeapCopy(const void * copyFrom, void * memory, std::true_type)
{
	if(memory == nullptr) {
		return new T(*(T *)copyFrom);
	}
	else {
		return new (memory) T(*(T *)copyFrom);
	}
}

template <typename T>
T * constructOnHeapCopy(const void * /*copyFrom*/, void * /*memory*/, std::false_type)
{
	raiseException<NotConstructibleException>();
	return nullptr;
}

template <typename T>
T * constructOnHeapMove(const void * copyFrom, void * memory, std::true_type)
{
	if(memory == nullptr) {
		return new T(std::move(*(T *)copyFrom));
	}
	else {
		return new (memory) T(std::move(*(T *)copyFrom));
	}
}

template <typename T>
T * constructOnHeapMove(const void * /*copyFrom*/, void * /*memory*/, std::false_type)
{
	raiseException<NotConstructibleException>();
	return nullptr;
}

template <typename T>
T * constructOnHeap(const void * copyFrom, void * memory, const CopyStrategy copyStrategy)
{
	if(copyFrom == nullptr) {
		return constructOnHeapDefault<T>(memory, std::is_default_constructible<T>());
	}
	else {
		switch(copyStrategy) {
		case CopyStrategy::copy:
			return constructOnHeapCopy<T>(copyFrom, memory, std::is_copy_constructible<T>());

		case CopyStrategy::move:
			return constructOnHeapMove<T>(copyFrom, memory, std::is_move_constructible<T>());

		default:
			return constructOnHeapAutoDetect<T>(copyFrom, memory, std::is_copy_constructible<T>(), std::is_move_constructible<T>());
		}
	}
}

template <typename T>
std::shared_ptr<T> constructSharedPtrDefault(std::true_type)
{
	return std::make_shared<T>();
}

template <typename T>
std::shared_ptr<T> constructSharedPtrDefault(std::false_type)
{
	raiseException<NotConstructibleException>();
	return std::shared_ptr<T>();
}

template <typename T, typename U>
std::shared_ptr<T> constructSharedPtrAutoDetect(const void * copyFrom, std::true_type, U)
{
	return std::make_shared<T>(*(T *)copyFrom);
}

template <typename T>
std::shared_ptr<T> constructSharedPtrAutoDetect(const void * copyFrom, std::false_type, std::true_type)
{
	return std::make_shared<T>(std::move(*(T *)copyFrom));
}

template <typename T>
std::shared_ptr<T> constructSharedPtrAutoDetect(const void * /*copyFrom*/, std::false_type, std::false_type)
{
	raiseException<NotConstructibleException>();
	return std::shared_ptr<T>();
}

template <typename T>
std::shared_ptr<T> constructSharedPtrCopy(const void * copyFrom, std::true_type)
{
	return std::make_shared<T>(*(T *)copyFrom);
}

template <typename T>
std::shared_ptr<T> constructSharedPtrCopy(const void * /*copyFrom*/, std::false_type)
{
	raiseException<NotConstructibleException>();
	return std::shared_ptr<T>();
}

template <typename T>
std::shared_ptr<T> constructSharedPtrMove(const void * copyFrom, std::true_type)
{
	return std::make_shared<T>(std::move(*(T *)copyFrom));
}

template <typename T>
std::shared_ptr<T> constructSharedPtrMove(const void * /*copyFrom*/, std::false_type)
{
	raiseException<NotConstructibleException>();
	return std::shared_ptr<T>();
}

template <typename T>
std::shared_ptr<T> constructSharedPtr(const void * copyFrom, const CopyStrategy copyStrategy)
{
	if(copyFrom == nullptr) {
		return constructSharedPtrDefault<T>(std::is_default_constructible<T>());
	}
	else {
		switch(copyStrategy) {
		case CopyStrategy::copy:
			return constructSharedPtrCopy<T>(copyFrom, std::is_copy_constructible<T>());

		case CopyStrategy::move:
			return constructSharedPtrMove<T>(copyFrom, std::is_move_constructible<T>());

		default:
			return constructSharedPtrAutoDetect<T>(copyFrom, std::is_copy_constructible<T>(), std::is_move_constructible<T>());
		}
	}
}

template <typename T, typename Enabled = void>
struct DeleterDtor
{
	static void callDelete(void * p)
	{
#if defined(METAPP_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
#if defined(METAPP_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-abstract-non-virtual-dtor"
#endif

		delete static_cast<T *>(p);

#if defined(METAPP_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif
#if defined(METAPP_COMPILER_GCC)
#pragma GCC diagnostic pop
#endif
	}

	static void callDtor(void * p)
	{
		static_cast<T *>(p)->~T();
	}
};

template <typename T>
struct DeleterDtor <T, typename std::enable_if<! std::is_destructible<T>::value>::type>
{
	static void callDelete(void * /*p*/)
	{
		raiseException<NotConstructibleException>("Not destructible");
	}

	static void callDtor(void * /*p*/)
	{
		raiseException<NotConstructibleException>("Not destructible");
	}
};


} // namespace internal_

} // namespace metapp

#endif
