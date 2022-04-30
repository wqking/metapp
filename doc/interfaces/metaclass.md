[//]: # (Auto generated file, don't modify this file.)

# MetaClass interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaClass interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaClass constructor](#a2_5)
* [MetaClass member functions for registering meta data](#a2_6)
  * [registerConstructor](#a4_1)
  * [registerAccessible](#a4_2)
  * [registerCallable](#a4_3)
  * [registerType](#a4_4)
* [MetaClass member functions for retrieving meta data](#a2_7)
  * [getConstructorList](#a4_5)
  * [getAccessible](#a4_6)
  * [getAccessibleList](#a4_7)
  * [getCallable](#a4_8)
  * [getCallableList by name](#a4_9)
  * [getCallableList](#a4_10)
  * [getType by name](#a4_11)
  * [getType by type kind](#a4_12)
  * [getType by MetaType](#a4_13)
  * [getTypeList](#a4_14)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaClass` is a meta interface to provide meta information of a class, such as constructors, member functions, member fields, etc.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metaclass.h"
```

<a id="a2_3"></a>
## Get MetaClass interface

We can call `MetaType::getMetaClass()` to get the MetaClass interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<SomeClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
```

<a id="a2_4"></a>
## Implemented built-in meta types

None

<a id="a2_5"></a>
## MetaClass constructor

```c++
template <typename FT>
MetaClass(const MetaType * classMetaType, FT callback);
```

`classMetaType` is the MetaType of the class being registered. It's used when traversing in the inheritance hierarchy.  
`callback` is a callback function. MetaClass invokes the callback in the constructor. Since the interface is usually implemented as static variable inside static function, the "callback in constructor" mechanism can guarantee thread safety.  

`callback` prototype,  
```c++
void callback(metapp::MetaClass & mc);
```
The MetaClass instance under constructing is passed as the parameter. The callback should register all meta data to `mc`.

<a id="a2_6"></a>
## MetaClass member functions for registering meta data

<a id="a4_1"></a>
#### registerConstructor

```c++
RegisteredConstructor & registerConstructor(const Variant & constructor);
```

Register a constructor. The parameter `constructor` is a Variant of `metapp::Constructor`.  
The returned `RegisteredConstructor` can be used add annotations to the meta data.

**Example**  

```c++
class CtorClass
{
public:
  CtorClass() : s(), n() {}
  CtorClass(const std::string & s, const int n) : s(s), n(n) {}

  std::string s;
  int n;
};

template <>
struct metapp::DeclareMetaType<CtorClass> : metapp::DeclareMetaTypeBase<CtorClass>
{
  static const metapp::MetaClass * getMetaClass() {
    static const metapp::MetaClass metaClass(
      metapp::getMetaType<CtorClass>(),
      [](metapp::MetaClass & mc) {
        mc.registerConstructor(metapp::Constructor<CtorClass()>());
        mc.registerConstructor(metapp::Constructor<CtorClass(const std::string &, const int)>());
      }
    );
    return &metaClass;
  }
};
```

<a id="a4_2"></a>
#### registerAccessible

```c++
RegisteredAccessible & registerAccessible(const std::string & name, const Variant & field);
```

Register a field (member or static member data).
The parameter `name` is the field name. The field can be got from the MetaClass by the name later. If a field with the same name has already registered, `registerAccessible` doesn't register the new field and returns the previous registered field.  
The parameter `field` is a Variant of MetaType that implements meta interface `MetaAccessible`. It can be pointer to member data, accessorpp::Accessor, or pointer to global data to simulate static member.  
The returned `RegisteredAccessible` can be used to add annotations to the meta data.  

**Example**  

```c++
class AccClass
{
public:
  AccClass() : text(), value() {}

  std::string text;

  int getValue() const {
    return value;
  }

  void setValue(const int n) {
    value = n;
  }

private:
  int value;
};

template <>
struct metapp::DeclareMetaType<AccClass> : metapp::DeclareMetaTypeBase<AccClass>
{
  static const metapp::MetaClass * getMetaClass() {
    static const metapp::MetaClass metaClass(
      metapp::getMetaType<AccClass>(),
      [](metapp::MetaClass & mc) {
        mc.registerAccessible("text", &AccClass::text);
        mc.registerAccessible("value", metapp::createAccessor(&AccClass::getValue, &AccClass::setValue));
      }
    );
    return &metaClass;
  }
};
```

<a id="a4_3"></a>
#### registerCallable

```c++
RegisteredCallable & registerCallable(const std::string & name, const Variant & callable);
```
Register a method (member or static member method).  
The parameter `name` is the method name. metapp allows multiple methods be registered under the same name,, they are treated as overloaded methods.  
The parameter `callable` is a Variant of MetaType that implements meta interface `MetaCallable`. It can be a pointer to member method, a pointer to non-member free method to simulate static member, or even `std::function`.  
The returned `RegisteredCallable` can be used to add annotations to the meta data.  

**Example**  

```c++
class CaClass
{
public:
  std::string greeting(const std::string & extra) const {
    return "Hello, " + extra;
  }

  // overloaded functions
  int add(int a, int b) {
    return a + b;
  }
  int add(int a, int b, int c) {
    return a + b + c;
  }
};

template <>
struct metapp::DeclareMetaType<CaClass> : metapp::DeclareMetaTypeBase<CaClass>
{
  static const metapp::MetaClass * getMetaClass() {
    static const metapp::MetaClass metaClass(
      metapp::getMetaType<CaClass>(),
      [](metapp::MetaClass & mc) {
        mc.registerCallable("greeting", &CaClass::greeting);
        mc.registerCallable("add", metapp::selectOverload<int(int, int)>(&CaClass::add));
        mc.registerCallable("add", metapp::selectOverload<int(int, int, int)>(&CaClass::add));
      }
    );
    return &metaClass;
  }
};
```

<a id="a4_4"></a>
#### registerType

```c++
template <typename T>
RegisteredType & registerType(const std::string & name = ""); // #1
  return registerType(name, getMetaType<T>());
}
RegisteredType & registerType(std::string name, const MetaType * metaType); // #2
```

Register a MetaType.  
The #1 form is equivalent to `registerType(name, getMetaType<T>())`;  
If the parameter `name` is empty, the function tries to get the name from built-in types. If the name is not found, then the name is not used and the MetaType can't be got by name.  
The returned `RegisteredType` can be used to add annotations to the meta data.  
This function can be used to register nested classes, or enum in the class.  

**Example**  

```c++
class TyClass
{
public:
  class NestedClass {};
  enum class NestedEnum {};
};

template <>
struct metapp::DeclareMetaType<TyClass> : metapp::DeclareMetaTypeBase<TyClass>
{
  static const metapp::MetaClass * getMetaClass() {
    static const metapp::MetaClass metaClass(
      metapp::getMetaType<TyClass>(),
      [](metapp::MetaClass & mc) {
        mc.registerType<TyClass::NestedClass>("NestedClass");
        mc.registerType("NestedEnum", metapp::getMetaType<TyClass::NestedEnum>());
      }
    );
    return &metaClass;
  }
};
```

<a id="a2_7"></a>
## MetaClass member functions for retrieving meta data

Most functions to retrieve meta data has a parameter `const MetaClass::Flags flags` with default value of `MetaClass::flagIncludeBase`. For functions that find a certain meta data, that means if the function can't find the meta data in current meta class, it will look up all base class recursively for the meta data. For functions that get all meta data, that means the function will return all meta data in current meta class, and all meta data in base classes, recursively. If you want only current meta class be checked, pass `MetaClass::flagNone`.  

<a id="a4_5"></a>
#### getConstructorList

```c++
const RegisteredConstructorList & getConstructorList() const;

using RegisteredConstructorList = std::deque<RegisteredConstructor>;
```

Get a list of RegisteredConstructor.  

**Example**  

```c++
const metapp::MetaType * metaType = metapp::getMetaType<CtorClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
// constructorList[0] is CtorClass()
// constructorList[1] is CtorClass(const std::string & s, const int n)
const metapp::RegisteredConstructorList & constructorList = metaClass->getConstructorList();
// Call metapp::callableInvoke with a std::deque of callables can invoke the proper function
// that matches the arguments
metapp::Variant instance = metapp::callableInvoke(constructorList, nullptr, "abc", 5);
CtorClass * ptr = instance.get<CtorClass *>();
ASSERT(ptr->s == "abc");
ASSERT(ptr->n == 5);
// Don't forget to delete the instance.  
// A better approach is to use Variant::takeFrom() to convert the instance to a Variant managed object,
// then we don't need to delete the instance.
delete ptr;
```

<a id="a4_6"></a>
#### getAccessible

```c++
const RegisteredAccessible & getAccessible(const std::string & name, const MetaClass::Flags flags = MetaClass::flagIncludeBase) const;
```

Get a field of `name`. If the field is not registered, an empty RegisteredAccessible is returned (RegisteredAccessible::isEmpty() is true).  

<a id="a4_7"></a>
#### getAccessibleList

```c++
RegisteredAccessibleList getAccessibleList(const Flags flags = flagIncludeBase) const;

using RegisteredAccessibleList = std::deque<RegisteredAccessible>;
```

Get a list of all registered field.  

**Example**  

```c++
const metapp::MetaType * metaType = metapp::getMetaType<AccClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
AccClass object;
object.text = "hello";
const metapp::RegisteredAccessible & text = metaClass->getAccessible("text");
ASSERT(metapp::accessibleGet(text, &object).get<const std::string &>() == "hello");

const metapp::RegisteredAccessibleList & accessibleList = metaClass->getAccessibleList();
ASSERT(accessibleList[0].getName() == "text");
ASSERT(accessibleList[1].getName() == "value");
```

<a id="a4_8"></a>
#### getCallable

```c++
const RegisteredCallable & getCallable(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a method of `name`. If the method is not registered, an empty RegisteredCallable is returned (RegisteredCallable::isEmpty() is true).  

<a id="a4_9"></a>
#### getCallableList by name

```c++
RegisteredCallableList getCallableList(const std::string & name, const Flags flags = flagIncludeBase) const;

using RegisteredCallableList = std::deque<RegisteredCallable>;
```

Get a list of all registered methods which has `name`. This is useful to get the overload methods.  

<a id="a4_10"></a>
#### getCallableList

```c++
RegisteredCallableList getCallableList(const Flags flags = flagIncludeBase) const;

using RegisteredCallableList = std::deque<RegisteredCallable>;
```

Get a list of all registered methods.  

**Example**  

```c++
const metapp::MetaType * metaType = metapp::getMetaType<CaClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
CaClass object;

const metapp::RegisteredCallable & greeting = metaClass->getCallable("greeting");
ASSERT(metapp::callableInvoke(greeting, &object, "world").get<const std::string &>() == "Hello, world");

const metapp::RegisteredCallableList add = metaClass->getCallableList("add");
ASSERT(metapp::callableInvoke(add, &object, 1, 5).get<int>() == 6);

const metapp::RegisteredCallableList callableList = metaClass->getCallableList();
ASSERT(callableList[0].getName() == "greeting");
ASSERT(callableList[1].getName() == "add");
ASSERT(callableList[2].getName() == "add");
```

<a id="a4_11"></a>
#### getType by name

```c++
const RegisteredType & getType(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a RegisteredType of `name`. If the type name is not registered, an empty RegisteredType is returned (RegisteredType::isEmpty() is true).  

<a id="a4_12"></a>
#### getType by type kind

```c++
const RegisteredType & getType(const TypeKind kind, const Flags flags = flagIncludeBase) const;
```

Get a RegisteredType of `kind`. If the type kind is not registered, an empty RegisteredType is returned (RegisteredType::isEmpty() is true).  

<a id="a4_13"></a>
#### getType by MetaType

```c++
const RegisteredType & getType(const MetaType * metaType, const Flags flags = flagIncludeBase) const;
```

Get a RegisteredType of `metaType`. If the meta type is not registered, an empty RegisteredType is returned (RegisteredType::isEmpty() is true).   
This function is useful to get the name of a registered meta type.

<a id="a4_14"></a>
#### getTypeList

```c++
RegisteredTypeList getTypeList(const Flags flags = flagIncludeBase) const;

using RegisteredTypeList = std::deque<RegisteredType>;
```

Get a list of all registered types.  
