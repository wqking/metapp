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

#ifndef METAPP_REGISTEREDACCESSIBLE_H_969872685611
#define METAPP_REGISTEREDACCESSIBLE_H_969872685611

#include "metapp/registration/registeredannotation.h"
#include "metapp/interfaces/metaaccessible.h"

#include <string>
#include <deque>
#include <memory>

namespace metapp {

class RegisteredAccessible : public RegisteredAnnotation
{
private:
	struct Data
	{
		Data(const std::string & name, const Variant & accessible)
			: name(name), accessible(accessible)
		{
		}

		std::string name;
		Variant accessible;
	};

public:
	RegisteredAccessible()
		: data()
	{
	}

	RegisteredAccessible(const std::string & name, const Variant & accessible)
		: data(std::make_shared<Data>(name, accessible))
	{
	}

	const std::string & getName() const {
		return data->name;
	}

	const Variant & getTarget() const {
		return data->accessible;
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

using RegisteredAccessibleList = std::deque<RegisteredAccessible>;


} // namespace metapp


#endif
