// accessorpp library
// Copyright (C) 2022 Wang Qi (wqking)
// Github: https://github.com/wqking/accessorpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ACCESSORPP_SETTER_H_578722158669
#define ACCESSORPP_SETTER_H_578722158669

#include "internal/typeutil_i.h"

#include <functional>
#include <type_traits>
#include <istream> 

namespace accessorpp {

template <typename Type_>
class Setter
{
public:
	using Type = Type_;
	using ValueType = typename private_::GetUnderlyingType<Type>::Type;

public:
	Setter()
		: setterFunc([](void *, const ValueType &) {})
	{
	}

	template <typename U>
	explicit Setter(U * address,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr)
		: setterFunc([address](void *, const ValueType & value) { *address = (U)(value); })
	{
	}

	template <typename U, typename C>
	Setter(U C::* address, C * instance,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr)
		: setterFunc([address, instance](void *, const ValueType & value) { instance->*address = (U)(value); })
	{
	}

	template <typename U, typename C>
	Setter(U C::* address,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr)
		: setterFunc([address](void * instance, const ValueType & value) { static_cast<C *>(instance)->*address = (U)(value); })
	{
	}

	template <typename F>
	explicit Setter(F func,
		typename std::enable_if<private_::CanInvoke<F, ValueType>::value>::type * = nullptr)
		: setterFunc([func](void *, const ValueType & value) { func(value); })
	{
	}

	template <typename F, typename C>
	Setter(F func, C * instance,
		typename std::enable_if<std::is_member_function_pointer<F>::value>::type * = nullptr)
		: setterFunc([func, instance](void *, const ValueType & value) { (instance->*func)(value); })
	{
	}

	template <typename F>
	Setter(F func,
		typename std::enable_if<std::is_member_function_pointer<F>::value>::type * = nullptr)
		: setterFunc([func](void * instance, const ValueType & value) {
			(static_cast<typename private_::CallableTypeChecker<F>::ClassType *>(instance)->*func)(value);
		})
	{
	}

	Setter(const Setter & other)
		: setterFunc(other.setterFunc)
	{
	}

	Setter(Setter && other)
		: setterFunc(std::move(other.setterFunc))
	{
	}

	Setter & operator = (const Setter & other) {
		setterFunc = other.setterFunc;
		return *this;
	}

	Setter & operator = (Setter && other) {
		setterFunc = std::move(other.setterFunc);
		return *this;
	}

	Setter & operator = (const ValueType & value) {
		set(value);
		return *this;
	}

	void set(const ValueType & value, void * instance = nullptr) {
		setterFunc(instance, value);
	}

private:
	std::function<void (void *, const ValueType &)> setterFunc;
};

template <typename T>
struct IsSetter : std::false_type
{
};

template <typename Type>
struct IsSetter <Setter<Type> > : std::true_type
{
};

template <typename Type>
std::istream & operator >> (std::istream & stream, Setter<Type> & setter)
{
	Type value;
	stream >> value;
	setter = value;
	return stream;
}


} // namespace accessorpp

#endif
