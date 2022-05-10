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

#ifndef ACCESSORPP_ACCESSOR_H_578722158669
#define ACCESSORPP_ACCESSOR_H_578722158669

#include "metapp/thirdparty/accessorpp/getter.h"
#include "metapp/thirdparty/accessorpp/setter.h"
#include "metapp/thirdparty/accessorpp/common.h"

#include <functional>
#include <type_traits>
#include <iostream> 
#include <cstddef>
#include <stdexcept>

namespace accessorpp {

namespace private_ {

struct DefaultGetter {};
struct DefaultSetter {};
struct NoSetter {};

} // namespace private_

constexpr private_::DefaultGetter defaultGetter;
constexpr private_::DefaultSetter defaultSetter;
constexpr private_::NoSetter noSetter;

struct InternalStorage {};
struct ExternalStorage {};

#include "metapp/thirdparty/accessorpp/internal/accessor_i.h"

template <
	typename Type,
	typename PoliciesType = DefaultPolicies
>
class Accessor :
	public private_::AccessorBase<
			Type,
			typename private_::SelectStorage<PoliciesType, private_::HasTypeStorage<PoliciesType>::value, InternalStorage>::Type,
			PoliciesType
		>,
	public private_::OnChangingCallback<
			typename private_::SelectOnChangingCallback<PoliciesType, private_::HasTypeOnChangingCallback<PoliciesType>::value>::Type,
			typename private_::SelectCallbackData<PoliciesType, private_::HasTypeCallbackData<PoliciesType>::value>::Type
		>,
	public private_::OnChangedCallback<
			typename private_::SelectOnChangedCallback<PoliciesType, private_::HasTypeOnChangedCallback<PoliciesType>::value>::Type,
			typename private_::SelectCallbackData<PoliciesType, private_::HasTypeCallbackData<PoliciesType>::value>::Type
		>
{
private:
	using BaseType = private_::AccessorBase<
			Type,
			typename private_::SelectStorage<PoliciesType, private_::HasTypeStorage<PoliciesType>::value, InternalStorage>::Type,
			PoliciesType
		>;
	using OnChangingCallbackType = private_::OnChangingCallback<
			typename private_::SelectOnChangingCallback<PoliciesType, private_::HasTypeOnChangingCallback<PoliciesType>::value>::Type,
			typename private_::SelectCallbackData<PoliciesType, private_::HasTypeCallbackData<PoliciesType>::value>::Type
		>;
	using OnChangedCallbackType = private_::OnChangedCallback<
			typename private_::SelectOnChangedCallback<PoliciesType, private_::HasTypeOnChangedCallback<PoliciesType>::value>::Type,
			typename private_::SelectCallbackData<PoliciesType, private_::HasTypeCallbackData<PoliciesType>::value>::Type
		>;

public:
	using ValueType = Type;
	using GetterType = typename BaseType::GetterType;
	using SetterType = typename BaseType::SetterType;

	static constexpr bool internalStorage = std::is_same<
		typename private_::SelectStorage<PoliciesType, private_::HasTypeStorage<PoliciesType>::value, InternalStorage>::Type,
		InternalStorage>::value;

public:
	Accessor() noexcept
		: BaseType()
	{
	}

	// The explict static_cast is required, otherwise it will call
	// template <typename P1> explicit AccessorBase(P1 && p2)
	Accessor(const Accessor & other)
		: BaseType(static_cast<const BaseType &>(other)) {
	}

	using BaseType::BaseType;

	Accessor & operator = (const Accessor & other) {
		*this = other.get();
		return *this;
	}

	Accessor & operator = (const ValueType & newValue) {
		return this->set(newValue);
	}

	Accessor & set(const ValueType & newValue, void * instance = nullptr) {
		this->doCheckWritable();

		this->OnChangingCallbackType::invokeCallback(newValue);
		this->setter.set(newValue, instance);
		this->OnChangedCallbackType::invokeCallback(newValue);
		return *this;
	}

	template <typename CD>
	Accessor & setWithCallbackData(const ValueType & newValue, CD && callbackData, void * instance = nullptr) {
		this->doCheckWritable();

		this->OnChangingCallbackType::invokeCallback(newValue, std::forward<CD>(callbackData));
		this->setter.set(newValue, instance);
		this->OnChangedCallbackType::invokeCallback(newValue, std::forward<CD>(callbackData));
		return *this;
	}

	ValueType get(const void * instance = nullptr) const {
		return this->getter.get(instance);
	}

	operator ValueType() const {
		return get();
	}

	template <typename F>
	void setGetter(const F & newGetter) {
		this->getter = GetterType(newGetter);
	}

	template <typename F>
	void setSetter(const F & newSetter) {
		this->setter = SetterType(newSetter);
	}

private:
};

template <typename T>
struct IsAccessor : std::false_type
{
};

template <
	typename Type,
	typename PoliciesType
>
struct IsAccessor <Accessor<Type, PoliciesType> > : std::true_type
{
};

template <typename T>
struct AccessorValueType
{
	using Type = typename std::decay<T>::type;
};

template <
	typename T,
	typename PoliciesType
>
struct AccessorValueType <Accessor<T, PoliciesType> >
{
	using Type = typename Accessor<T, PoliciesType>::ValueType;
};

template <typename T, typename G, typename S, typename Policies>
Accessor<T, Policies> createAccessor(G && getter, S && setter, Policies = Policies())
{
	return Accessor<T, Policies>(std::forward<G>(getter), std::forward<S>(setter));
}

template <
	typename T,
	typename G, typename IG, typename S, typename IS,
	typename Policies = DefaultPolicies
>
Accessor<T, Policies> createAccessor(
		G && getter, IG && getterInstance,
		S && setter, IS && setterInstance,
		Policies = Policies()
	)
{
	return Accessor<T, Policies>(
		std::forward<G>(getter), std::forward<IG>(getterInstance),
		std::forward<S>(setter), std::forward<IS>(setterInstance)
	);
}

template <typename G, typename S, typename Policies = DefaultPolicies>
auto createAccessor(G && getter, S && setter, Policies = Policies())
	-> Accessor<typename private_::DetectValueType<G>::Type, Policies>
{
	using A = Accessor<typename private_::DetectValueType<G>::Type, Policies>;
	return A(
		std::forward<G>(getter),
		std::forward<S>(setter)
	);
}

template <
	typename G, typename IG, typename S, typename IS,
	typename Policies = DefaultPolicies
>
auto createAccessor(
		G && getter, IG && getterInstance,
		S && setter, IS && setterInstance,
		Policies = Policies()
	)
	-> Accessor<typename private_::DetectValueType<G>::Type, Policies>
{
	using A = Accessor<typename private_::DetectValueType<G>::Type, Policies>;
	return A(
		std::forward<G>(getter), std::forward<IG>(getterInstance),
		std::forward<S>(setter), std::forward<IS>(setterInstance)
	);
}

template <typename T, typename G, typename Policies = DefaultPolicies>
Accessor<T, Policies> createReadOnlyAccessor(G && getter, Policies = Policies())
{
	using A = Accessor<T, Policies>;
	return A(getter, noSetter);
}

template <typename T, typename G, typename IG, typename Policies>
Accessor<T, Policies> createReadOnlyAccessor(G && getter, IG && getterInstance, Policies)
{
	using A = Accessor<T, Policies>;
	return A(A::GetterType(std::forward<G>(getter), std::forward<IG>(getterInstance)), noSetter);
}

template <typename G, typename Policies = DefaultPolicies>
auto createReadOnlyAccessor(G && getter, Policies = Policies())
	-> Accessor<typename private_::DetectValueType<G>::Type, Policies>
{
	using A = Accessor<typename private_::DetectValueType<G>::Type, Policies>;
	return A(std::forward<G>(getter), noSetter);
}

template <typename G, typename IG, typename Policies>
auto createReadOnlyAccessor(G && getter, IG && getterInstance, Policies)
	-> Accessor<typename private_::DetectValueType<G>::Type, Policies>
{
	using A = Accessor<typename private_::DetectValueType<G>::Type, Policies>;
	return A(A::GetterType(std::forward<G>(getter), std::forward<IG>(getterInstance)), noSetter);
}


template <typename T>
auto operator << (std::ostream & stream, const T & accessor)
	-> typename std::enable_if<IsAccessor<T>::value, std::ostream &>::type
{
	stream << accessor.get();
	return stream;
}

template <typename T>
auto operator >> (std::istream & stream, T & accessor)
	-> typename std::enable_if<IsAccessor<T>::value, std::istream &>::type
{
	typename T::ValueType value;
	stream >> value;
	accessor = value;
	return stream;
}


// Unary operators

template <typename T>
auto operator ++ (T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a += 1;
	return a;
}

template <typename T>
auto operator ++ (T & a, int)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result;
	result = a;
	++a;
	return result;
}

template <typename T>
auto operator -- (T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a -= 1;
	return a;
}

template <typename T>
auto operator -- (T & a, int)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result;
	result = a;
	--a;
	return result;
}

template <typename T>
auto operator ! (const T & a)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result;
	result = a;
	result = ! a.get();
	return result;
}

template <typename T>
auto operator + (T & a)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result;
	result = a;
	result = +(typename AccessorValueType<T>::Type)(a);
	return result;
}

template <typename T>
auto operator - (T & a)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result;
	result = a;
	result = -(typename AccessorValueType<T>::Type)(a);
	return result;
}




// Below operators are generated from tool generateops.py.
// Using C style cast because static_cast may fail on some case, such as char[]



// Logic operators

template <typename T, typename U>
auto operator == (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) == (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator != (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) != (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator > (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) > (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator >= (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) >= (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator < (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) < (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator <= (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) <= (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator && (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) && (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator || (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) || (typename AccessorValueType<U>::Type)(b);
}
// Binary operators

template <typename T, typename U>
auto operator + (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) + (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator - (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) - (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator * (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) * (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator / (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) / (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator % (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) % (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator & (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) & (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator | (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) | (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator ^ (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) ^ (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator << (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) << (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator >> (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::internalStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) >> (typename AccessorValueType<U>::Type)(b);
	return result;
}
// Binary assignment operators

template <typename T, typename U>
auto operator += (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) + (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator -= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) - (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator *= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) * (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator /= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) / (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator %= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) % (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator &= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) & (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator |= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) | (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator ^= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) ^ (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator <<= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) << (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator >>= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) >> (typename AccessorValueType<U>::Type)(b);
	return a;
}


} // namespace accessorpp

#endif
