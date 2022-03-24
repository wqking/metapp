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

#ifndef METAPP_REGISTEREDFIELD_H_969872685611
#define METAPP_REGISTEREDFIELD_H_969872685611

#include "metapp/registration/annotation.h"
#include "metapp/interfaces/metaaccessible.h"

#include <string>
#include <vector>
#include <memory>

namespace metapp {

class RegisteredField : public Annotation
{
private:
	struct Data
	{
		Data(const std::string & name, const Variant & field)
			: name(name), field(field)
		{
		}

		std::string name;
		Variant field;
	};

public:
	static const RegisteredField & getEmpty()
	{
		static RegisteredField emptyRegisteredField = RegisteredField();
		return emptyRegisteredField;
	}

public:
	RegisteredField()
		: data()
	{
	}

	RegisteredField(const std::string & name, const Variant & field)
		: data(std::make_shared<Data>(name, field))
	{
	}

	const std::string & getName() const {
		return data->name;
	}

	const Variant & getField() const {
		return data->field;
	}

	bool isEmpty() const {
		return ! data;
	}

	const MetaType * getValueType() const {
		return data->field.getMetaType()->getMetaAccessible()->getValueType();
	}

	Variant get(const void * instance) const {
		return data->field.getMetaType()->getMetaAccessible()->get(data->field, instance);
	} 

	void set(void * instance, const Variant & value) const {
		data->field.getMetaType()->getMetaAccessible()->set(data->field, instance, value);
	} 

private:
	std::shared_ptr<Data> data;
};

using RegisteredFieldList = std::vector<RegisteredField>;


} // namespace metapp


#endif
