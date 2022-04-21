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

#ifndef METAPP_REGISTEREDTYPE_H_969872685611
#define METAPP_REGISTEREDTYPE_H_969872685611

#include "metapp/registration/registeredannotation.h"

#include <string>
#include <deque>
#include <memory>

namespace metapp {

class MetaType;

class RegisteredType : public RegisteredAnnotation
{
private:
	struct Data
	{
		Data(const std::string & name, const MetaType * metaType)
			: name(name), metaType(metaType)
		{
		}

		std::string name;
		const MetaType * metaType;
	};

public:
	RegisteredType()
		: data()
	{
	}

	RegisteredType(const std::string & name, const MetaType * metaType)
		: data(std::make_shared<Data>(name, metaType))
	{
	}

	const std::string & getName() const {
		return data ? data->name : internal_::emptyString;
	}

	const MetaType * getTarget() const {
		return data ? data->metaType : nullptr;
	}

	bool isEmpty() const {
		return ! data;
	}

	operator const MetaType * () const {
		return getTarget();
	}

private:
	std::shared_ptr<Data> data;
};

using RegisteredTypeList = std::deque<RegisteredType>;


} // namespace metapp


#endif
