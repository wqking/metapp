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

#ifndef ACCESSOR_I_H_578722158669
#define ACCESSOR_I_H_578722158669

namespace private_ {

template <typename CallbackType>
struct ChangeCallbackBase
{
	CallbackType & getCallback() {
		return callback;
	}

	const CallbackType & getCallback() const {
		return callback;
	}

	CallbackType callback;
};

template <typename CallbackType, typename CallbackDataType>
class ChangeCallback : protected ChangeCallbackBase <CallbackType>
{
protected:
	template <typename ValueType>
	void invokeCallback(
			const ValueType & newValue
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue, CallbackDataType());
	}

	template <typename ValueType>
	void invokeCallback(
			const ValueType & newValue,
			const CallbackDataType & data
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue, data);
	}

private:
	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & /*newValue*/,
			const CallbackDataType & /*data*/
		)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		this->callback();
	}

	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & newValue,
			const CallbackDataType & /*data*/
		)
		-> typename std::enable_if<CanInvoke<C, const ValueType &>::value, void>::type
	{
		this->callback(newValue);
	}

	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & newValue,
			const CallbackDataType & data
		)
		-> typename std::enable_if<CanInvoke<C, const ValueType &, const CallbackDataType &>::value, void>::type
	{
		this->callback(newValue, data);
	}
};

template <typename CallbackType>
class ChangeCallback <CallbackType, void> : protected ChangeCallbackBase <CallbackType>
{
protected:
	template <typename ValueType>
	void invokeCallback(
			const ValueType & newValue
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue);
	}

	template <typename ValueType, typename Data>
	void invokeCallback(
			const ValueType & newValue,
			Data &&
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue);
	}

private:
	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & /*newValue*/
		)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		this->callback();
	}

	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & newValue
		)
		-> typename std::enable_if<CanInvoke<C, const ValueType &>::value, void>::type
	{
		this->callback(newValue);
	}
};

class DummyChangeCallback
{
protected:
	template <typename ValueType>
	void invokeCallback(const ValueType & /*newValue*/)
	{
	}

	template <typename ValueType, typename Data>
	void invokeCallback(const ValueType & /*newValue*/, Data &&)
	{
	}

};

template <typename CallbackType, typename CallbackDataType>
class OnChangingCallback : public ChangeCallback <CallbackType, CallbackDataType>
{
private:
	using super = ChangeCallback <CallbackType, CallbackDataType>;

public:
	using super::super;

	const CallbackType & onChanging() const {
		return super::getCallback();
	}

	CallbackType & onChanging() {
		return super::getCallback();
	}
};

template <typename CallbackDataType>
class OnChangingCallback <void, CallbackDataType> : public DummyChangeCallback
{
};

template <typename CallbackType, typename CallbackDataType>
class OnChangedCallback : public ChangeCallback <CallbackType, CallbackDataType>
{
private:
	using super = ChangeCallback <CallbackType, CallbackDataType>;

public:
	using super::super;

	const CallbackType & onChanged() const {
		return super::getCallback();
	}

	CallbackType & onChanged() {
		return super::getCallback();
	}
};

template <typename CallbackDataType>
class OnChangedCallback <void, CallbackDataType> : public DummyChangeCallback
{
};

template <typename Type_>
class AccessorRoot
{
protected:
	using GetterType = Getter<Type_>;
	using SetterType = Setter<Type_>;

public:
	AccessorRoot() noexcept
		:
			getter(),
			setter(),
			readOnly(false)
	{
	}

	AccessorRoot(const AccessorRoot & other) noexcept
		:
			getter(other.getter),
			setter(other.setter),
			readOnly(other.readOnly)
	{
	}

	AccessorRoot(AccessorRoot && other) noexcept
		:
			getter(std::move(other.getter)),
			setter(std::move(other.setter)),
			readOnly(std::move(other.readOnly))
	{
	}

	template <typename G, typename S>
	AccessorRoot(G && getter, S && setter) noexcept
		:
			getter(std::forward<G>(getter)),
			setter(std::forward<S>(setter)),
			readOnly(false)
	{
	}

	template <typename G>
	AccessorRoot(G && getter, NoSetter) noexcept
		:
			getter(std::forward<G>(getter)),
			setter(),
			readOnly(true)
	{
	}

	template <typename G, typename IG, typename S, typename IS>
	AccessorRoot(
			G && getter, IG && getterInstance,
			S && setter, IS && setterInstance
		) noexcept
		:
			getter(std::forward<G>(getter), std::forward<IG>(getterInstance)),
			setter(std::forward<S>(setter), std::forward<IS>(setterInstance)),
			readOnly(false)
	{
	}

	constexpr bool isReadOnly() const {
		return readOnly;
	}

protected:
	void doCheckWritable() const {
		if(readOnly) {
			throw std::logic_error("Can't set to read-only accessor.");
		}
	}

protected:
	GetterType getter;
	SetterType setter;
	const bool readOnly;
};

template <typename Type_, typename Storage>
class AccessorBase;

template <typename Type_>
class AccessorBase <Type_, InternalStorage> : public AccessorRoot<Type_>
{
private:
	using super = AccessorRoot<Type_>;
	using ValueType = typename std::remove_cv<typename std::remove_reference<Type_>::type>::type;

public:
	using GetterType = typename super::GetterType;
	using SetterType = typename super::SetterType;

public:
	AccessorBase(const ValueType & newValue = ValueType()) noexcept
		:
			super(&AccessorBase::value, this, &AccessorBase::value, this),
			value(newValue)
	{
	}

	AccessorBase(const AccessorBase & other) noexcept
		:
			super(&AccessorBase::value, this, &AccessorBase::value, this),
			value(other.value)
	{
	}

	AccessorBase(AccessorBase && other) noexcept
		:
			super(static_cast<super &&>(other)),
			value(std::move(other.value))
	{
	}

	template <typename G, typename S>
	AccessorBase(G && getter, S && setter, const ValueType & newValue = ValueType()) noexcept
		:
			super(std::forward<G>(getter),
				std::forward<S>(setter)),
			value(newValue)
	{
	}

	template <typename S>
	AccessorBase(DefaultGetter, S && setter, const ValueType & newValue = ValueType()) noexcept
		:
			super(GetterType(&AccessorBase::value, this),
				std::forward<S>(setter)),
			value(newValue)
	{
	}

	template <typename G>
	AccessorBase(G && getter, DefaultSetter, const ValueType & newValue = ValueType()) noexcept
		:
			super(GetterType(std::forward<G>(getter)),
				SetterType(&AccessorBase::value, this)),
			value(newValue)
	{
	}

	AccessorBase(DefaultGetter, DefaultSetter, const ValueType & newValue = ValueType()) noexcept
		:
			super(GetterType(&AccessorBase::value, this),
				SetterType(&AccessorBase::value, this)),
			value(newValue)
	{
	}

	template <typename G, typename IG, typename S, typename IS>
	AccessorBase(
			G && getter, IG && getterInstance,
			S && setter, IS && setterInstance,
			const ValueType & newValue = ValueType()
		) noexcept
		:
			super(std::forward<G>(getter), std::forward<IG>(getterInstance),
				std::forward<S>(setter), std::forward<IS>(setterInstance)),
			value(newValue)
	{
	}

	// The functions directGet and directSet should be used to implement getter/setter,
	// They don't respect "readOnly", and don't trigger any events.
	const ValueType & directGet() const {
		return value;
	}

	ValueType & directGet() {
		return value;
	}

	// This doesn't respect "readOnly".
	void directSet(const ValueType & newValue) {
		value = newValue;
	}

private:
	ValueType value;
};

template <typename Type_>
class AccessorBase <Type_, ExternalStorage> : public AccessorRoot<Type_>
{
private:
	using super = AccessorRoot<Type_>;

public:
	using GetterType = typename super::GetterType;
	using SetterType = typename super::SetterType;

public:
	using super::super;
};


} // namespace private_



#endif
