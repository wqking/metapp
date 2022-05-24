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

#include "testutil.h"

/* desc
# Tutorial for using MetaClass

`MetaClass` is a meta interface to provide meta information of a class,
such as constructors, member functions, member fields, etc.  
desc */

//desc ## Include headers

//code
//desc Header for MetaClass
#include "metapp/interfaces/metaclass.h"
//desc To use the built-in meta types, we must include "metapp/allmetatypes.h"
#include "metapp/allmetatypes.h"
//desc Header for MetaEnum
#include "metapp/interfaces/metaenum.h"
//code

/* desc
## Define the C++ class to reflect for

Here is the class we are going to reflect for.
desc */

//code
class TmClass
{
public:
	// Nested enum type, its meta type will be declared
	enum class MyEnum {
		one = 1,
		two = 2
	};

	// Nested class type, its meta type will not be declared,
	// but we can still registerType for it
	struct MyInner {
		MyInner() : n(1999) {}
		int n;
	};

public:
	// Default constructor
	TmClass()
		: message(), value(0)
	{
	}

	// Constructor with arguments
	TmClass(const int value, const std::string & message)
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

	// static member function, it shows how to return values
	// via reference/pointer in parameter.
	static void obtainValues(std::string & message, int * value, TmClass * obj) {
		message = obj->message;
		*value = obj->getValue();
	}

	// Public member data. You may not want to do it in your product code,
	// here is to demonstrate how to use meta field.
	std::string message;

private:
	int value;
};
//code

/* desc
## Declare meta type for TmClass
desc */

//code
constexpr metapp::TypeKind tkMyEnum = metapp::tkUser;
constexpr metapp::TypeKind tkMyClass = metapp::tkUser + 1;

/* desc
Now declare meta type for TmClass.  
The declaration must be specialization of metapp::DeclareMetaType, and inherit from metapp::DeclareMetaTypeBase.  
desc */
template <>
struct metapp::DeclareMetaType <TmClass> : metapp::DeclareMetaTypeBase <TmClass>
{
	// Define the TypeKind for the type. We don't need to define the TypeKind for
	// every meta type unless we do need it.  
	// If we don't define the typeKind, it will be tkObject by default.  
	static constexpr metapp::TypeKind typeKind = tkMyClass;

	// Implement the MetaClass interface.  
	// Not every classes need to implement MetaClass.
	// Without MetaClass, we can still construct object of the class.  
	// But with MetaClass we can get more information such as member data, member functions, etc.  
	// Note the code is inside the specialization `struct metapp::DeclareMetaType <TmClass>`.  
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<TmClass>(),
			[](metapp::MetaClass & mc) {
				// Register constructors
				mc.registerConstructor(metapp::Constructor<TmClass ()>());
				mc.registerConstructor(metapp::Constructor<
					TmClass (const int, const std::string &)
				>());

				// Register field with getter/setter function
				mc.registerAccessible("value",
					metapp::createAccessor(&TmClass::getValue, &TmClass::setValue));
				// Register member data as field
				auto & item = mc.registerAccessible("message", &TmClass::message);
				// Add some annotations to the accessible
				item.registerAnnotation("description", "This is a description");
				item.registerAnnotation("notes", std::vector<std::string> { "first", "second" });

				// Register a member function
				mc.registerCallable("greeting", &TmClass::greeting);
				
				// Register overloaded member functions
				mc.registerCallable("makeMessage",
					metapp::selectOverload<std::string () const>(&TmClass::makeMessage));
				mc.registerCallable("makeMessage",
					metapp::selectOverload<std::string (const std::string &, const int) const>(
						&TmClass::makeMessage));
				mc.registerCallable("makeMessage",
					metapp::selectOverload<std::string (const int, const std::string &) const>(
						&TmClass::makeMessage));
				
				// Register static member function
				mc.registerCallable("obtainValues", &TmClass::obtainValues);

				// Register nested type.
				// The declaration of meta type for TmClass::MyInner is not required,
				mc.registerType<TmClass::MyInner>("MyInner");

				// We declare the meta type for TmClass::MyEnum later.
				// The declaration of meta type for TmClass::MyEnum is not required,
				// if it's not declared, we can still registerType<TmClass::MyEnum>,
				// but we can't get any name or value from the enum.
				mc.registerType<TmClass::MyEnum>("MyEnum");
			}
		);
		return &metaClass;
	}

};
//code

/* desc
Declare meta type for TmClass::MyEnum,
it is used when declaring meta type for TmClass.
desc */

//code
template <>
struct metapp::DeclareMetaType <TmClass::MyEnum> : metapp::DeclareMetaTypeBase <TmClass::MyEnum>
{
	static constexpr metapp::TypeKind typeKind = tkMyEnum;

	// Implement the MetaEnum interface
	static const metapp::MetaEnum * getMetaEnum() {
		static const metapp::MetaEnum metaEnum([](metapp::MetaEnum & me) {
			// Register the enum name and values, then we can get the name and value later.
			me.registerValue("one", TmClass::MyEnum::one);
			me.registerValue("two", TmClass::MyEnum::two);
		});
		return &metaEnum;
	}
};
//code

/* desc
## Use member data

Now let's see how to use field meta data.
desc */
TEST_CASE("tutorialMetaClass_accessible")
{
	//code
	//desc First let's get the MetaType of TmClass.
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();
	
	//desc If we have a Variant that holds a TmClass, we can get the MetaType from the Variant as well.
	#if 0 //nocode
	metapp::Variant v = TmClass();
	const metapp::MetaType * metaType = v.getMetaType();
	#endif //nocode

	//desc Now get the MetaClass from the MetaType.  
	//desc If the MetaType doesn't implement MetaClass, the return value is nullptr.
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	//desc Declare an instace of TmClass, we will use the field meta data to access its member data.
	TmClass obj;

	//desc Get the meta data of field "value".
	const metapp::MetaItem & fieldValue = metaClass->getAccessible("value");

	//desc Call metapp::accessibleGet to get the value of the field. The first parameter is the Variant.  
	//desc The second argument is the object instance.  
	//desc Call asAccessible() to get the underlying accessible Variant.
	ASSERT(metapp::accessibleGet(fieldValue.asAccessible(), &obj).get<int>() == 0);
	//desc getTarget() can also be omitted, the MetaItem can convert to Variant automatically
	ASSERT(metapp::accessibleGet(fieldValue, &obj).get<int>() == 0);

	//desc Now let's set a new value.
	metapp::accessibleSet(fieldValue, &obj, 5);
	ASSERT(obj.getValue() == 5);
	ASSERT(metapp::accessibleGet(fieldValue, &obj).get<int>() == 5);

	//desc Now set 'message' with some new text.
	const metapp::MetaItem & fieldMessage = metaClass->getAccessible("message");
	ASSERT(metapp::accessibleGet(fieldMessage, &obj).get<const std::string &>() == "");
	metapp::accessibleSet(fieldMessage, &obj, "This is a test");
	ASSERT(obj.message == "This is a test");
	ASSERT(metapp::accessibleGet(fieldMessage, &obj).get<const std::string &>() == "This is a test");
	//code
}

/* desc
## Use member function

Now let's call the member method.
desc */
TEST_CASE("tutorialMetaClass_method")
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	TmClass obj;
	obj.message = "Hello";

	//desc Get the meta data of method "greeting".
	const metapp::MetaItem & methodGreeting = metaClass->getCallable("greeting");
	
	//desc Use metapp::callableInvoke to invoke the method, and pass the arguments.  
	//desc The return value is a metapp::Variant.
	metapp::Variant result = metapp::callableInvoke(methodGreeting, &obj, ", world");
	ASSERT(result.get<const std::string &>() == "Hello, world");
	
	//desc Call the method again with different arguments.
	ASSERT(metapp::callableInvoke(methodGreeting, &obj, ", metapp").get<const std::string &>() == "Hello, metapp");
	//code
}

/* desc
## Use overloaded member functions

Now let's call the overloaded member method.
desc */
TEST_CASE("tutorialMetaClass_overloadedMethods")
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	TmClass obj;
	obj.message = "Hello";

	//desc Get all the overloaded methods of "makeMessage" by calling metaClass->getCallable().  
	//desc All overloaded methods are combined to a single Variant of type kind `tkOverloadedFunction`.  
	//desc We can obtain the overloaded methods as if it's a single method.
	const metapp::MetaItem & callable = metaClass->getCallable("makeMessage");

	//desc metapp::callableInvoke will try to find the proper method, then call it.  
	//desc If no method can be invoked with the arguments, exception `metapp::IllegalArgumentException` is raised.  
	ASSERT(metapp::callableInvoke(callable, &obj).get<const std::string &>() == "Hello");
	ASSERT(metapp::callableInvoke(callable, &obj, 19, "Hello").get<const std::string &>() == "Hello19Hello");
	ASSERT(metapp::callableInvoke(callable, &obj, ", this is ", 8.1).get<const std::string &>() == "Hello, this is 8");
	//code
}

/* desc
## Use static member function

Now let's see how to use static method.
desc */
TEST_CASE("tutorialMetaClass_staticMethods")
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	TmClass obj;
	obj.message = "Hello";
	obj.setValue(38);

	std::string message;
	int value = 0;
	ASSERT(message == "");
	ASSERT(value == 0);

	const metapp::MetaItem & obtainValues = metaClass->getCallable("obtainValues");
	//desc There is no much difference between member methods and static methods, the only difference
	//desc is that the instance can be `nullptr` when invoking the method.
	metapp::callableInvoke(obtainValues, nullptr, metapp::Variant::create<std::string &>(message), &value, &obj);
	ASSERT(message == "Hello");
	ASSERT(value == 38);
	//code
}

/* desc
## Use constructor

Now let's play with contructors.
desc */
TEST_CASE("tutorialMetaClass_constructor")
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	//desc The constructors. All overloaded constructors are combined to
	//desc a single Variant of type kind `tkOverloadedFunction`.  
	const metapp::MetaItem & constructor = metaClass->getConstructor();

	//desc Invoke the default constructor which doesn't have any arguments.  
	//desc Invoking constructors is same as invoking overloaded methods.  
	//desc In metapp::callableInvoke, the second argument "nullptr" is the instance pointer, that's not
	//desc used when invoking constructors.  
	//desc The constructor returns a Variant, which is a pointer to the constructed object.  
	//desc The caller must free the pointer. The returned Variant doesn't free it.
	std::unique_ptr<TmClass> ptr(metapp::callableInvoke(constructor, nullptr).get<TmClass *>());
	ASSERT(ptr->getValue() == 0);
	ASSERT(ptr->message == "");

	//desc If we want to convert the returned pointer to a Variant which manages the object lifetime,
	//desc we can use metapp::Variant::takeFrom to create a new Variant that owns the object.
	metapp::Variant instance = metapp::Variant::takeFrom(metapp::callableInvoke(constructor, nullptr, 3, "good").get<TmClass *>());
	ASSERT(instance.getMetaType() == metapp::getMetaType<TmClass>());
	ASSERT(instance.get<const TmClass &>().getValue() == 3);
	ASSERT(instance.get<const TmClass &>().message == "good");
	//desc instance will free the object when instance is freed.
	//code
}

/* desc
## Use nested types

Now let's play with types.
desc */
TEST_CASE("tutorialMetaClass_type")
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	metapp::MetaItem enumType;

	enumType = metaClass->getType("MyEnum");
	ASSERT(enumType.asMetaType() == metapp::getMetaType<TmClass::MyEnum>());

	enumType = metaClass->getType(tkMyEnum);
	ASSERT(enumType.asMetaType() == metapp::getMetaType<TmClass::MyEnum>());

	enumType = metaClass->getType(metapp::getMetaType<TmClass::MyEnum>());
	ASSERT(enumType.asMetaType() == metapp::getMetaType<TmClass::MyEnum>());
	ASSERT(enumType.getName() == "MyEnum");

	const metapp::MetaEnum * metaEnum = enumType.asMetaType()->getMetaEnum();
	ASSERT(metaEnum->getValue("one").asEnumValue().get<TmClass::MyEnum>() == TmClass::MyEnum::one);
	ASSERT(metaEnum->getValue("two").asEnumValue().get<TmClass::MyEnum>() == TmClass::MyEnum::two);

	const metapp::MetaItem & innerType = metaClass->getType("MyInner");
	ASSERT(innerType.asMetaType() == metapp::getMetaType<TmClass::MyInner>());
	std::unique_ptr<TmClass::MyInner> inner(static_cast<TmClass::MyInner *>(innerType.asMetaType()->construct()));
	ASSERT(inner->n == 1999);
	//code
}

/* desc
## Use annotations

Now let's play with annotations
desc */
TEST_CASE("tutorialMetaClass_annotation")
{
	//code
	//desc First let's get the MetaType of TmClass.
	const metapp::MetaType * metaType = metapp::getMetaType<TmClass>();

	//desc Get the MetaClass from the MetaType.  
	//desc If the MetaType doesn't implement MetaClass, the return value is nullptr.
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	const metapp::MetaItem & fieldMessage = metaClass->getAccessible("message");
	const metapp::Variant & description = fieldMessage.getAnnotation("description");
	ASSERT(description.cast<std::string>().get<const std::string &>() == "This is a description");
	const metapp::Variant & notes = fieldMessage.getAnnotation("notes");
	ASSERT(notes.get<const std::vector<std::string> &>()[0] == "first");
	ASSERT(notes.get<const std::vector<std::string> &>()[1] == "second");
	const metapp::Variant & notExist = fieldMessage.getAnnotation("notExist");
	ASSERT(notExist.isEmpty());
	//code
}

