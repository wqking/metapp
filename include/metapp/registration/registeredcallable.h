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

#ifndef METAPP_REGISTEREDCALLABLE_H_969872685611
#define METAPP_REGISTEREDCALLABLE_H_969872685611

#include "metapp/registration/registeredannotation.h"
#include "metapp/interfaces/metacallable.h"

#include <string>
#include <deque>

namespace metapp {

class RegisteredCallable : public RegisteredAnnotation
{
private:
	struct Data
	{
		Data(const std::string & name, const Variant & callable)
			: name(name), callable(callable)
		{
		}

		std::string name;
		Variant callable;
	};

public:
	RegisteredCallable()
		: data()
	{
	}

	RegisteredCallable(const std::string & name, const Variant & callable)
		: data(std::make_shared<Data>(name, callable))
	{
	}

	const std::string & getName() const {
		return data->name;
	}

	const Variant & getTarget() const {
		return data->callable;
	}

	bool isEmpty() const {
		return ! data;
	}

	operator const Variant & () const {
		return getTarget();
	}

private:
	std::shared_ptr<Data> data;
};

using RegisteredCallableList = std::deque<RegisteredCallable>;

} // namespace metapp


#endif
