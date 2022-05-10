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

#ifndef ACCESSORPP_GETTER_H_578722158669
#define ACCESSORPP_GETTER_H_578722158669

#include "internal/typeutil_i.h"
#include "metapp/thirdparty/accessorpp/common.h"

#include <functional>
#include <type_traits>
#include <ostream> 

namespace accessorpp {

namespace private_ {

class DefaultClassTypeSetter
{
protected:
	template <typename C>
	void setClassType() {
	}
};

} // namespace private_

template <typename Type_, typename PoliciesType = DefaultPolicies>
class Getter :
	public private_::SelectClassTypeSetter<PoliciesType, private_::HasTypeClassTypeSetter<PoliciesType>::value, private_::DefaultClassTypeSetter>::Type
{
public:
	using Type = Type_;
	using ValueType = typename private_::GetUnderlyingType<Type>::Type;

public:
	Getter()
		: getterFunc()
	{
	}

	template <typename U>
	explicit Getter(const U * address,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr)
		: getterFunc([address](const void *)->Type { return (Type)*address; })
	{
	}

	template <typename U, typename C>
	Getter(const U C::* address, const C * instance,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr)
		: getterFunc([address, instance](const void *)->Type { return (Type)(instance->*address); })
	{
		this->template setClassType<C>();
	}

	template <typename U, typename C>
	Getter(const U C::* address,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr)
		: getterFunc([address](const void * instance)->Type { return (Type)(static_cast<const C *>(instance)->*address); })
	{
		this->template setClassType<C>();
	}

	explicit Getter(const Type & value)
		: getterFunc([value](const void *)->Type { return value; })
	{
	}

	template <typename F>
	explicit Getter(F func,
		typename std::enable_if<private_::CanInvoke<F>::value>::type * = nullptr)
		: getterFunc([func](const void *)->Type { return (Type)func(); })
	{
	}

	template <typename F, typename C>
	Getter(F func, C * instance,
		typename std::enable_if<
			private_::CallableTypeChecker<F>::isClassMember
			&& std::is_convertible<typename private_::CallableTypeChecker<F>::ResultType, ValueType>::value
		>::type * = nullptr)
		: getterFunc([func, instance](const void *)->Type { return (Type)((instance->*func)()); })
	{
		this->template setClassType<typename private_::CallableTypeChecker<F>::ClassType>();
	}

	template <typename F>
	Getter(F func,
		typename std::enable_if<
			private_::CallableTypeChecker<F>::isClassMember
			&& std::is_convertible<typename private_::CallableTypeChecker<F>::ResultType, ValueType>::value
		>::type * = nullptr)
		: getterFunc([func](const void * instance)->Type {
			return (Type)((static_cast<const typename private_::CallableTypeChecker<F>::ClassType *>(instance)->*func)());
		})
	{
		this->template setClassType<typename private_::CallableTypeChecker<F>::ClassType>();
	}

	Getter(const Getter & other)
		: getterFunc(other.getterFunc)
	{
	}

	Getter(Getter && other)
		: getterFunc(std::move(other.getterFunc))
	{
	}

	Getter & operator = (const Getter & other) {
		getterFunc = other.getterFunc;
		return *this;
	}

	Getter & operator = (Getter && other) {
		getterFunc = std::move(other.getterFunc);
		return *this;
	}

	Type get(const void * instance = nullptr) const {
		return getterFunc(instance);
	}

	operator Type() const {
		return get();
	}

private:
	std::function<Type (const void *)> getterFunc;
};

template <typename T>
struct IsGetter : std::false_type
{
};

template <typename Type>
struct IsGetter <Getter<Type> > : std::true_type
{
};

template <typename Type>
std::ostream & operator << (std::ostream & stream, const Getter<Type> & getter)
{
	stream << getter.get();
	return stream;
}


} // namespace accessorpp

#endif
