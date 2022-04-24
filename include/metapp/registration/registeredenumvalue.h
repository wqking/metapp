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

#ifndef METAPP_REGISTEREDENUMVALUE_H_969872685611
#define METAPP_REGISTEREDENUMVALUE_H_969872685611

#include "metapp/registration/registeredannotation.h"

#include <string>
#include <deque>
#include <memory>

namespace metapp {

class RegisteredEnumValue : public RegisteredAnnotation
{
public:
	using ValueType = long long;

private:
	struct Data
	{
		Data(const std::string & name, const ValueType value)
			: name(name), value(value)
		{
		}

		std::string name;
		ValueType value;
	};

public:
	RegisteredEnumValue()
		: data()
	{
	}

	RegisteredEnumValue(const std::string & name, const ValueType value)
		: data(std::make_shared<Data>(name, value))
	{
	}

	const std::string & getName() const {
		return data ? data->name : internal_::emptyString;
	}

	ValueType getTarget() const {
		return data ? data->value : ValueType();
	}

	bool isEmpty() const {
		return ! data;
	}

	operator ValueType () const {
		return getTarget();
	}

private:
	std::shared_ptr<Data> data;
};

using RegisteredEnumValueList = std::deque<RegisteredEnumValue>;


} // namespace metapp


#endif
