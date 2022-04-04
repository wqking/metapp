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

#ifndef METAPP_REGISTEREDCONSTRUCTOR_H_969872685611
#define METAPP_REGISTEREDCONSTRUCTOR_H_969872685611

#include "metapp/registration/registeredannotation.h"
#include "metapp/interfaces/metacallable.h"

#include <vector>

namespace metapp {

class RegisteredConstructor : public RegisteredAnnotation
{
private:
	struct Data
	{
		explicit Data(const Variant & method)
			: method(method)
		{
		}

		Variant method;
	};

public:
	RegisteredConstructor()
		: data()
	{
	}

	explicit RegisteredConstructor(const Variant & constructor)
		: data(std::make_shared<Data>(constructor))
	{
	}

	const Variant & getConstructor() const {
		return data->method;
	}

	bool isEmpty() const {
		return ! data;
	}

	size_t getParamCount() const {
		return data->method.getMetaType()->getMetaCallable()->getParamCount();
	}

	const MetaType * getReturnType() const {
		return data->method.getMetaType()->getMetaCallable()->getReturnType();
	}

	const MetaType * getParamType(const size_t index) const {
		return data->method.getMetaType()->getMetaCallable()->getParamType(index);
	}

	int rankInvoke(const Variant * arguments, const size_t argumentCount) const {
		return data->method.getMetaType()->getMetaCallable()->rankInvoke(arguments, argumentCount);
	}

	bool canInvoke(const Variant * arguments, const size_t argumentCount) const {
		return data->method.getMetaType()->getMetaCallable()->canInvoke(arguments, argumentCount);
	}

	Variant invoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount) const {
		return data->method.getMetaType()->getMetaCallable()->invoke(func, instance, arguments, argumentCount);
	}

	operator const Variant & () {
		return data->method;
	}

private:
	std::shared_ptr<Data> data;
};

using RegisteredConstructorList = std::vector<RegisteredConstructor>;

} // namespace metapp


#endif
