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
#include "metapp/interfaces/metaenum.h"

// Define a class to reflect
class MyClass
{
public:
	// Nested type
	enum class MyEnum {
		one = 1,
		two = 2
	};

public:
	// Default constructor
	MyClass()
		: message(), value(0)
	{
	}

	// Constructor with arguments
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
	static void obtainValues(std::string & message, int * value, MyClass * obj) {
		message = obj->message;
		*value = obj->getValue();
	}

	// Public member data. You may not want to do it in your product code,
	// here is to demonstrate how to use meta field.
	std::string message;

private:
	int value;
};

constexpr metapp::TypeKind tkMyEnum = metapp::tkUser;
constexpr metapp::TypeKind tkMyClass = metapp::tkUser + 1;

// Declare meta type for MyClass::MyEnum,
// it will be used when declare meta type for MyClass
template <>
struct metapp::DeclareMetaType <MyClass::MyEnum> : metapp::DeclareMetaTypeBase <MyClass::MyEnum>
{
	// Define the TypeKind for the type. We don't need to define the TypeKind for
	// every meta type unless we do need it.
	// If we don't define the typeKind, it will be tkObject by default.
	static constexpr metapp::TypeKind typeKind = tkMyEnum;

	// Implement the MetaEnum interface
	static const metapp::MetaEnum * getMetaEnum() {
		static const metapp::MetaEnum metaEnum([](metapp::MetaEnum & me) {
			// Register the enum name and values, then we can get the name and value later.
			me.registerValue("one", MyClass::MyEnum::one);
			me.registerValue("two", MyClass::MyEnum::two);
		});
		return &metaEnum;
	}
};

// Declare meta type for MyClass,
template <>
struct metapp::DeclareMetaType <MyClass> : metapp::DeclareMetaTypeBase <MyClass>
{
	static constexpr metapp::TypeKind typeKind = tkMyClass;

	// Implement the MetaClass interface
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyClass>(),
			[](metapp::MetaClass & mc) {
				// Register constructors
				mc.registerConstructor(metapp::Constructor<MyClass ()>());
				mc.registerConstructor(metapp::Constructor<MyClass (const int, const std::string &)>());

				// Register field with getter/setter function
				mc.registerField("value", metapp::createAccessor(&MyClass::getValue, &MyClass::setValue));
				// Register member data as field
				mc.registerField("message", &MyClass::message);

				// Register a member function
				mc.registerMethod("greeting", &MyClass::greeting);
				
				// Register overloaded member function
				mc.registerMethod("makeMessage", metapp::selectOverload<std::string () const>(&MyClass::makeMessage));
				mc.registerMethod("makeMessage", metapp::selectOverload<std::string (const std::string &, const int) const>(&MyClass::makeMessage));
				mc.registerMethod("makeMessage", metapp::selectOverload<std::string (const int, const std::string &) const>(&MyClass::makeMessage));
				
				// Register static member function
				mc.registerMethod("obtainValues", &MyClass::obtainValues);

				// Register nested type
				mc.registerType<MyClass::MyEnum>("MyEnum");
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
	ASSERT(metapp::invokeCallable(methodList, &obj).get<const std::string &>() == "Hello");

	metapp::Variant arguments[] = { 38, ", world"};
	auto itWithArgs = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 2);
	ASSERT(itWithArgs!= methodList.end());
	ASSERT(itWithArgs->invoke(&obj, arguments, 2).get<const std::string &>() == "Hello38, world");
	ASSERT(metapp::invokeCallable(methodList, &obj, 19, "Hello").get<const std::string &>() == "Hello19Hello");

	ASSERT(metapp::invokeCallable(methodList, &obj, ", this is ", 8.1).get<const std::string &>() == "Hello, this is 8");
}

void tutorialMetaClass_staticMethods()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	MyClass obj;
	obj.message = "Hello";
	obj.setValue(38);

	std::string message;
	int value = 0;
	ASSERT(message == "");
	ASSERT(value == 0);

	metapp::RegisteredMethod methodObjtainValues = metaClass->getMethod("obtainValues");
	metapp::invokeCallable(methodObjtainValues, nullptr, metapp::Variant::create<std::string &>(message), &value, &obj);
	ASSERT(message == "Hello");
	ASSERT(value == 38);
}

void tutorialMetaClass_constructor()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	metapp::RegisteredConstructorList constructorList = metaClass->getConstructorList();
	std::unique_ptr<MyClass> ptr(metapp::invokeCallable(constructorList, nullptr).get<MyClass *>());
	ASSERT(ptr->getValue() == 0);
	ASSERT(ptr->message == "");

	metapp::Variant instance = metapp::Variant::takeFrom(metapp::invokeCallable(constructorList, nullptr, 3, "good").get<MyClass *>());
	ASSERT(instance.getMetaType() == metapp::getMetaType<MyClass>());
	ASSERT(instance.get<const MyClass &>().getValue() == 3);
	ASSERT(instance.get<const MyClass &>().message == "good");
}

void tutorialMetaClass_type()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	metapp::RegisteredType enumType;

	enumType = metaClass->getType("MyEnum");
	ASSERT(enumType.getTarget() == metapp::getMetaType<MyClass::MyEnum>());

	enumType = metaClass->getType(tkMyEnum);
	ASSERT(enumType.getTarget() == metapp::getMetaType<MyClass::MyEnum>());

	enumType = metaClass->getType(metapp::getMetaType<MyClass::MyEnum>());
	ASSERT(enumType.getTarget() == metapp::getMetaType<MyClass::MyEnum>());
	ASSERT(enumType.getName() == "MyEnum");

	const metapp::MetaEnum * metaEnum = enumType.getTarget()->getMetaEnum();
	ASSERT(metaEnum->getValue("one") == 1);
	ASSERT(metaEnum->getValue("two") == 2);
}

RUN_TUTORIAL(tutorialMetaClass_field)
RUN_TUTORIAL(tutorialMetaClass_method)
RUN_TUTORIAL(tutorialMetaClass_overloadedMethods)
RUN_TUTORIAL(tutorialMetaClass_staticMethods)
RUN_TUTORIAL(tutorialMetaClass_constructor)
RUN_TUTORIAL(tutorialMetaClass_type)
