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

#include "metapp/registration/registeredannotation.h"
#include "metapp/interfaces/metacallable.h"

#include <string>
#include <deque>

namespace metapp {

class RegisteredMethod : public RegisteredAnnotation
{
private:
	struct Data
	{
		Data(const std::string & name, const Variant & method)
			: name(name), method(method)
		{
		}

		std::string name;
		Variant method;
	};

public:
	static const RegisteredMethod & getEmpty()
	{
		static RegisteredMethod emptyRegisteredMethod{};
		return emptyRegisteredMethod;
	}

public:
	RegisteredMethod()
		: data()
	{
	}

	RegisteredMethod(const std::string & name, const Variant & method)
		: data(std::make_shared<Data>(name, method))
	{
	}

	const std::string & getName() const {
		return data->name;
	}

	const Variant & getTarget() const {
		return data->method;
	}

	bool isEmpty() const {
		return ! data;
	}

	size_t getParamCount() const {
		return data->method.getMetaType()->getMetaCallable()->getParamCount(data->method);
	}

	const MetaType * getReturnType() const {
		return data->method.getMetaType()->getMetaCallable()->getReturnType(data->method);
	}

	const MetaType * getParamType(const size_t index) const {
		return data->method.getMetaType()->getMetaCallable()->getParamType(data->method, index);
	}

	int rankInvoke(const Variant * arguments, const size_t argumentCount) const {
		return data->method.getMetaType()->getMetaCallable()->rankInvoke(data->method, arguments, argumentCount);
	}

	bool canInvoke(const Variant * arguments, const size_t argumentCount) const {
		return data->method.getMetaType()->getMetaCallable()->canInvoke(data->method, arguments, argumentCount);
	}

	Variant invoke(void * instance, const Variant * arguments, const size_t argumentCount) const {
		return data->method.getMetaType()->getMetaCallable()->invoke(getTarget(), instance, arguments, argumentCount);
	}

	operator const Variant & () const {
		return getTarget();
	}

private:
	std::shared_ptr<Data> data;
};

using RegisteredMethodList = std::deque<RegisteredMethod>;

} // namespace metapp


#endif
