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

#ifndef METAPP_REGISTEREDMETHOD_H_969872685611
#define METAPP_REGISTEREDMETHOD_H_969872685611

#include "metapp/registration/annotation.h"
#include "metapp/interfaces/metacallable.h"

#include <string>
#include <vector>

namespace metapp {

namespace internal_ {

class RegisteredMethodBase
{
public:
	explicit RegisteredMethodBase(const Variant & method)
		: method(method)
	{
	}

	bool isEmpty() const {
		return method.isEmpty();
	}

	size_t getParamCount() const {
		return method.getMetaType()->getMetaCallable()->getParamCount();
	}

	const MetaType * getReturnType() const {
		return method.getMetaType()->getMetaCallable()->getReturnType();
	}

	const MetaType * getParamType(const size_t index) const {
		return method.getMetaType()->getMetaCallable()->getParamType(index);
	}

	int rankInvoke(const Variant * arguments, const size_t argumentCount) const {
		return method.getMetaType()->getMetaCallable()->rankInvoke(arguments, argumentCount);
	}

	bool canInvoke(const Variant * arguments, const size_t argumentCount) const {
		return method.getMetaType()->getMetaCallable()->canInvoke(arguments, argumentCount);
	}

	Variant invoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount) const {
		return method.getMetaType()->getMetaCallable()->invoke(func, instance, arguments, argumentCount);
	}

protected:
	const Variant & doGetMethod() const {
		return method;
	}

private:
	Variant method;
};

} // namespace internal_

class RegisteredMethod : public internal_::RegisteredMethodBase, public Annotation
{
private:
	using super = internal_::RegisteredMethodBase;

public:
	static const RegisteredMethod & getEmpty()
	{
		static RegisteredMethod emptyRegisteredMethod = RegisteredMethod(std::string(), Variant());
		return emptyRegisteredMethod;
	}

public:
	RegisteredMethod(const std::string & name, const Variant & method)
		: name(name), super(method)
	{
	}

	const std::string & getName() const {
		return name;
	}

	const Variant & getMethod() const {
		return super::doGetMethod();
	}

private:
	std::string name;
};

using RegisteredMethodList = std::vector<std::reference_wrapper<const RegisteredMethod> >;

} // namespace metapp


#endif
