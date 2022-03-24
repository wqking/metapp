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

namespace metapp {

class RegisteredField : public Annotation
{
public:
	static const RegisteredField & getEmpty()
	{
		static RegisteredField emptyRegisteredField = RegisteredField(std::string(), Variant());
		return emptyRegisteredField;
	}

public:
	RegisteredField(const std::string & name, const Variant & field)
		: name(name), field(field)
	{
	}

	const std::string & getName() const {
		return name;
	}

	const Variant & getField() const {
		return field;
	}

	bool isEmpty() const {
		return field.isEmpty();
	}

	const MetaType * getValueType() const {
		return field.getMetaType()->getMetaAccessible()->getValueType();
	}

	Variant get(const void * instance) const {
		return field.getMetaType()->getMetaAccessible()->get(field, instance);
	} 

	void set(void * instance, const Variant & value) const {
		field.getMetaType()->getMetaAccessible()->set(field, instance, value);
	} 

private:
	std::string name;
	Variant field;
};

using RegisteredFieldList = std::vector<std::reference_wrapper<const RegisteredField> >;


} // namespace metapp


#endif
