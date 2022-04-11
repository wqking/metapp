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

#include "tutorial.h"

#include "metapp/metatypes/metatypes.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/metarepo.h"

#include <iostream>

class MyClass
{
public:
	MyClass()
		: message(), value(0)
	{
	}

	MyClass(const int value, const std::string & message)
		: message(message), value(value)
	{
	}

	// Getter and setter for private member
	int getValue() const {
		return value;
	}
	void setValue(const int n) {
		value = n;
	}

	// Member function
	std::string greeting(const std::string & extra) const {
		return message + extra;
	}

	// Member overloaded functions
	std::string makeMessage() const {
		return message;
	}
	std::string makeMessage(const std::string & a, const int b) const {
		return message + a + std::to_string(b);
	}
	std::string makeMessage(const int a, const std::string & b) const {
		return message + std::to_string(a) + b;
	}

	// static member function, it shows how to return values via reference/pointer in parameter.
	static void obtainValues(MyClass * obj, std::string & message, int * value) {
		message = obj->message;
		*value = obj->getValue();
	}

	// Public member data. You may not want to do it in your product code,
	// here is to demonstrate how to use meta field.
	std::string message;

private:
	int value;
};

namespace metapp {

template <typename Signature, typename Class>
auto selectOverload(Signature (Class::*func)) -> decltype(func)
{
	return func;
}

template <typename Signature>
auto selectOverload(Signature * func) -> decltype(func)
{
	return func;
}

}

template <>
struct metapp::DeclareMetaType <MyClass> : metapp::DeclareMetaTypeBase <MyClass>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyClass>(),
			[](metapp::MetaClass & mc) {
				mc.registerConstructor(metapp::Constructor<MyClass ()>());
				mc.registerConstructor(metapp::Constructor<MyClass (const int, const std::string &)>());

				mc.registerField("value", metapp::createAccessor(&MyClass::getValue, &MyClass::setValue));
				mc.registerField("message", &MyClass::message);

				mc.registerMethod("greeting", &MyClass::greeting);
				
				mc.registerMethod("makeMessage", metapp::selectOverload<std::string () const>(&MyClass::makeMessage));
				mc.registerMethod("makeMessage", metapp::selectOverload<std::string (const std::string &, const int) const>(&MyClass::makeMessage));
				mc.registerMethod("makeMessage", metapp::selectOverload<std::string (const int, const std::string &) const>(&MyClass::makeMessage));
				
				mc.registerMethod("obtainValues", &MyClass::obtainValues);
			}
		);
		return &metaClass;
	}

};

void tutorialMetaClass_field()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	MyClass obj;

	metapp::RegisteredField fieldValue = metaClass->getField("value");
	// Call getTarget() to get the underlying Variant
	ASSERT(metapp::accessibleGet(fieldValue.getTarget(), &obj).get<int>() == 0);
	// getTarget() can also be omitted, the RegisteredField can convert to Variant automatically
	ASSERT(metapp::accessibleGet(fieldValue, &obj).get<int>() == 0);

	// Now let's set a new value
	metapp::accessibleSet(fieldValue, &obj, 5);
	ASSERT(obj.getValue() == 5);
	ASSERT(metapp::accessibleGet(fieldValue, &obj).get<int>() == 5);

	// Now set 'message' with some new text
	metapp::RegisteredField fieldMessage = metaClass->getField("message");
	ASSERT(metapp::accessibleGet(fieldMessage, &obj).get<const std::string &>() == "");
	metapp::accessibleSet(fieldMessage, &obj, "This is a test");
	ASSERT(obj.message == "This is a test");
	ASSERT(metapp::accessibleGet(fieldMessage, &obj).get<const std::string &>() == "This is a test");
}

void tutorialMetaClass_method()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	MyClass obj;
	obj.message = "Hello";

	metapp::RegisteredMethod methodGreeting = metaClass->getMethod("greeting");
	metapp::Variant result = metapp::invokeCallable(methodGreeting, &obj, ", world");
	ASSERT(result.get<const std::string &>() == "Hello, world");
	
	ASSERT(metapp::invokeCallable(methodGreeting, &obj, ", metapp").get<const std::string &>() == "Hello, metapp");
}

void tutorialMetaClass_overloadedMethods()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	MyClass obj;
	obj.message = "Hello";

	metapp::RegisteredMethodList methodList = metaClass->getMethodList("makeMessage");
	auto itNoArgs = metapp::findCallable(methodList.begin(), methodList.end(), nullptr, 0);
	ASSERT(itNoArgs != methodList.end());
	metapp::Variant result = metapp::invokeCallable(*itNoArgs, &obj);
	ASSERT(result.get<const std::string &>() == "Hello");

	metapp::Variant arguments[] = { 38, ", world"};
	auto itWithArgs = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 2);
	ASSERT(itWithArgs!= methodList.end());
	ASSERT(itWithArgs->invoke(&obj, arguments, 2).get<const std::string &>() == "Hello38, world");
}


RUN_TUTORIAL(tutorialMetaClass_field)
RUN_TUTORIAL(tutorialMetaClass_method)
RUN_TUTORIAL(tutorialMetaClass_overloadedMethods)