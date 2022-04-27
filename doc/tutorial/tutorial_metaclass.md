# Tutorial for using MetaClass

## Include headers
Header for MetaClass

```c++
#include "metapp/interfaces/metaclass.h"
```

To use the built-in meta types, we must include "metapp/allmetatypes.h"

```c++
#include "metapp/allmetatypes.h"
```

Header for MetaEnum

```c++
#include "metapp/interfaces/metaenum.h"
```

## Define the C++ class to reflect for

Here is the class we are going to reflect for.

```c++
class MyClass
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

  // static member function, it shows how to return values
  // via reference/pointer in parameter.
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
```

## Declare meta type for MyClass

```c++
constexpr metapp::TypeKind tkMyEnum = metapp::tkUser;
constexpr metapp::TypeKind tkMyClass = metapp::tkUser + 1;
```

Now declare meta type for MyClass.  
The declaration must be specialization of metapp::DeclareMetaType, and inherit from metapp::DeclareMetaTypeBase.

```c++
template <>
struct metapp::DeclareMetaType <MyClass> : metapp::DeclareMetaTypeBase <MyClass>
{
```

Define the TypeKind for the type. We don't need to define the TypeKind for
every meta type unless we do need it.  
If we don't define the typeKind, it will be tkObject by default.  

```c++
  static constexpr metapp::TypeKind typeKind = tkMyClass;
```

Implement the MetaClass interface.  
Not every classes need to implement MetaClass. Without MetaClass, we can still construct object of the class.  
But with MetaClass we can get more information such as member data, member functions, etc.  
Note the code is inside the specialization `struct metapp::DeclareMetaType <MyClass>`.  

```c++
  static const metapp::MetaClass * getMetaClass() {
    static const metapp::MetaClass metaClass(
      metapp::getMetaType<MyClass>(),
      [](metapp::MetaClass & mc) {
        // Register constructors
        mc.registerConstructor(metapp::Constructor<MyClass ()>());
        mc.registerConstructor(metapp::Constructor<
          MyClass (const int, const std::string &)
        >());

        // Register field with getter/setter function
        mc.registerAccessible("value",
          metapp::createAccessor(&MyClass::getValue, &MyClass::setValue));
        // Register member data as field
        auto & item = mc.registerAccessible("message", &MyClass::message);
        // Add some annotations to the accessible
        item.addAnnotation("description", "This is a description");
        item.addAnnotation("notes", std::vector<std::string> { "first", "second" });

        // Register a member function
        mc.registerCallable("greeting", &MyClass::greeting);
        
        // Register overloaded member functions
        mc.registerCallable("makeMessage",
          metapp::selectOverload<std::string () const>(&MyClass::makeMessage));
        mc.registerCallable("makeMessage",
          metapp::selectOverload<std::string (const std::string &, const int) const>(
            &MyClass::makeMessage));
        mc.registerCallable("makeMessage",
          metapp::selectOverload<std::string (const int, const std::string &) const>(
            &MyClass::makeMessage));
        
        // Register static member function
        mc.registerCallable("obtainValues", &MyClass::obtainValues);

        // Register nested type.
        // The declaration of meta type for MyClass::MyInner is not required,
        mc.registerType<MyClass::MyInner>("MyInner");

        // We declare the meta type for MyClass::MyEnum later.
        // The declaration of meta type for MyClass::MyEnum is not required,
        // if it's not declared, we can still registerType<MyClass::MyEnum>,
        // but we can't get any name or value from the enum.
        mc.registerType<MyClass::MyEnum>("MyEnum");
      }
    );
    return &metaClass;
  }

};
```

Declare meta type for MyClass::MyEnum,
it is used when declaring meta type for MyClass.

```c++
template <>
struct metapp::DeclareMetaType <MyClass::MyEnum> : metapp::DeclareMetaTypeBase <MyClass::MyEnum>
{
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
```

## Use member data

Now let's see how to use field meta data.

```c++
// First let's get the MetaType of MyClass
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();

// If we have a Variant that holds a MyClass, we can get the MetaType from the Variant as well
//metapp::Variant v = MyClass();
//const metapp::MetaType * metaType = v.getMetaType();

// Get the MetaClass from the MetaType.
// If the MetaType doesn't implement MetaClass, the return value is nullptr.
const metapp::MetaClass * metaClass = metaType->getMetaClass();

// Declare an instace of MyClass, we will use the field meta data to access it's member data
MyClass obj;

// Get the meta data of field "value"
metapp::RegisteredAccessible fieldValue = metaClass->getAccessible("value");

// Call metapp::accessibleGet to get the value of the field. The first parameter is the Variant.
// Call getTarget() to get the underlying Variant.
ASSERT(metapp::accessibleGet(fieldValue.getTarget(), &obj).get<int>() == 0);
// getTarget() can also be omitted, the RegisteredAccessible can convert to Variant automatically
ASSERT(metapp::accessibleGet(fieldValue, &obj).get<int>() == 0);

// Now let's set a new value
metapp::accessibleSet(fieldValue, &obj, 5);
ASSERT(obj.getValue() == 5);
ASSERT(metapp::accessibleGet(fieldValue, &obj).get<int>() == 5);

// Now set 'message' with some new text
metapp::RegisteredAccessible fieldMessage = metaClass->getAccessible("message");
ASSERT(metapp::accessibleGet(fieldMessage, &obj).get<const std::string &>() == "");
metapp::accessibleSet(fieldMessage, &obj, "This is a test");
ASSERT(obj.message == "This is a test");
ASSERT(metapp::accessibleGet(fieldMessage, &obj).get<const std::string &>() == "This is a test");
```

## Use member function

Now let's call the member method.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();

MyClass obj;
obj.message = "Hello";

// Get the meta data of method "greeting".
metapp::RegisteredCallable methodGreeting = metaClass->getCallable("greeting");

// Call metapp::callableInvoke to invoke the method, and pass the arguments.
// The return value is a metapp::Variant.
metapp::Variant result = metapp::callableInvoke(methodGreeting, &obj, ", world");
ASSERT(result.get<const std::string &>() == "Hello, world");

// Call the method again with different arguments.
ASSERT(metapp::callableInvoke(methodGreeting, &obj, ", metapp").get<const std::string &>() == "Hello, metapp");
```

## Use overloaded member functions

Now let's call the overloaded member method.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();

MyClass obj;
obj.message = "Hello";

// Get all the overloaded methods of "makeMessage" by calling metaClass->getCallableList
// If we use metaClass->getCallable("makeMessage"), the only first method is returned.
metapp::RegisteredCallableList callableList = metaClass->getCallableList("makeMessage");

// There are various ways to call the overload methods

// Approach 1
// Use metapp::findCallable to find the callable method.
// The last two arguments are the arguments and argument count, here they are nullptr and 0, indicate no arguments
// The return value is an iterator
auto itNoArgs = metapp::findCallable(callableList.begin(), callableList.end(), nullptr, 0);
// Found the method "std::string makeMessage() const"
ASSERT(itNoArgs != callableList.end());
// Invoke the found method
metapp::Variant result = itNoArgs->getTarget().getMetaType()->getMetaCallable()->invoke(
  itNoArgs->getTarget(), &obj, nullptr, 0);
ASSERT(result.get<const std::string &>() == "Hello");

// Find another method, which should be "std::string makeMessage(const int a, const std::string & b) const"
metapp::Variant arguments[] = { 38, ", world"};
auto itWithArgs = metapp::findCallable(callableList.begin(), callableList.end(), arguments, 2);
ASSERT(itWithArgs!= callableList.end());
ASSERT(itWithArgs->getTarget().getMetaType()->getMetaCallable()->invoke(
  itWithArgs->getTarget(), &obj, arguments, 2).get<const std::string &>() == "Hello38, world");

// Approach 2
// We can use metapp::callableInvoke to "invoke" the method list directly.
// metapp::callableInvoke will try to find the proper method, then call it.
ASSERT(metapp::callableInvoke(callableList, &obj).get<const std::string &>() == "Hello");
ASSERT(metapp::callableInvoke(callableList, &obj, 19, "Hello").get<const std::string &>() == "Hello19Hello");
ASSERT(metapp::callableInvoke(callableList, &obj, ", this is ", 8.1).get<const std::string &>() == "Hello, this is 8");
```

## Use static member function

Now let's see how to use static method.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();

MyClass obj;
obj.message = "Hello";
obj.setValue(38);

std::string message;
int value = 0;
ASSERT(message == "");
ASSERT(value == 0);

metapp::RegisteredCallable methodObjtainValues = metaClass->getCallable("obtainValues");
// There is no much difference between member methods and static methods, the only difference
// is that the instance can be nullptr when invoking the method.
metapp::callableInvoke(methodObjtainValues, nullptr, metapp::Variant::create<std::string &>(message), &value, &obj);
ASSERT(message == "Hello");
ASSERT(value == 38);
```

## Use constructor

Now let's play with contructors.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();

// Get all constructors from the meta class
metapp::RegisteredConstructorList constructorList = metaClass->getConstructorList();

// Invoke the default constructor which doesn't have any arguments.
// Invoking constructors is same as invoking overloaded methods.
// In metapp::callableInvoke, the second argument "nullptr" is the instance pointer, that's not
// used when invoking constructors.
// The constructor returns a Variant, which is a pointer to the constructed object.
// The caller must free the pointer. The returned Variant doesn't free it.
std::unique_ptr<MyClass> ptr(metapp::callableInvoke(constructorList, nullptr).get<MyClass *>());
ASSERT(ptr->getValue() == 0);
ASSERT(ptr->message == "");

// If we want to convert the returned pointer to a Variant which manage the object lifetime,
// we can use metapp::Variant::takeFrom to create a new Variant that owns the object.
metapp::Variant instance = metapp::Variant::takeFrom(metapp::callableInvoke(constructorList, nullptr, 3, "good").get<MyClass *>());
ASSERT(instance.getMetaType() == metapp::getMetaType<MyClass>());
ASSERT(instance.get<const MyClass &>().getValue() == 3);
ASSERT(instance.get<const MyClass &>().message == "good");
// instance will free the object when instance is freed
```

## Use nested types

Now let's play with types.

```c++
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

metapp::RegisteredType innerType = metaClass->getType("MyInner");
ASSERT(innerType.getTarget() == metapp::getMetaType<MyClass::MyInner>());
std::unique_ptr<MyClass::MyInner> inner(static_cast<MyClass::MyInner *>(innerType.getTarget()->construct()));
ASSERT(inner->n == 1999);
```

## Use annotations

Now let's play with annotations

```c++
// First let's get the MetaType of MyClass
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();

// Get the MetaClass from the MetaType.
// If the MetaType doesn't implement MetaClass, the return value is nullptr.
const metapp::MetaClass * metaClass = metaType->getMetaClass();

metapp::RegisteredAccessible fieldMessage = metaClass->getAccessible("message");
const metapp::Variant & description = fieldMessage.getAnnotation("description");
ASSERT(description.cast<std::string>().get<const std::string &>() == "This is a description");
const metapp::Variant & notes = fieldMessage.getAnnotation("notes");
ASSERT(notes.get<const std::vector<std::string> &>()[0] == "first");
ASSERT(notes.get<const std::vector<std::string> &>()[1] == "second");
const metapp::Variant & notExist = fieldMessage.getAnnotation("notExist");
ASSERT(notExist.isEmpty());
```
