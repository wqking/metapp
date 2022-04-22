# MetaClass interface

## Overview

`MetaClass` is a meta interface to provide meta information of a class, such as constructors, member functions, member fields, etc.  

## Header

```c++
#include "metapp/interfaces/metaclass.h"
```

## Get MetaClass interface

We can call `MetaType::getMetaClass()` to get the MetaClass interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
```

## Implemented built-in meta types

None

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

## MetaClass member functions for registration meta data

#### registerConstructor

```c++
RegisteredConstructor & registerConstructor(const Variant & constructor);
```

Register a constructor. The parameter `constructor` is a Variant of `metapp::Constructor`.  
The returned `RegisteredConstructor` can be used add annotations to the meta data.

**Example**  
```c++
class MyClass
{
public:
	MyClass();
	MyClass(const std::string & a, const int b);
};

template <>
struct metapp::DeclareMetaType<MyClass> : metapp::DeclareMetaTypeBase<MyClass>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyClass>(),
			[](metapp::MetaClass & mc) {
				mc.registerConstructor(metapp::Constructor<MyClass()>());
				mc.registerConstructor(metapp::Constructor<MyClass(const std::string &, const int)>());
			}
		);
		return &metaClass;
	}
};

```

#### registerAccessible

```c++
RegisteredAccessible & registerAccessible(const std::string & name, const Variant & field);
```

Register a field (member or static member data).
The parameter `name` is the field name. The field can be got from the MetaClass by the name later. If a field with the same name has already registered, `registerAccessible` doesn't register the new field and returns the previous registered field.  
The parameter `field` is a Variant of MetaType that implements meta interface `MetaAccessible`. It can be pointer to member data, accessorpp::Accessor, or pointer to global data to simulate static member.  
The returned `RegisteredAccessible` can be used to add annotations to the meta data.  

#### registerCallable

```c++
RegisteredCallable & registerCallable(const std::string & name, const Variant & callable);
```
Register a method (member or static member method).  
The parameter `name` is the method name. metapp allows multiple methods be registered under the same name,, they are treated as overloaded methods.  
The parameter `callable` is a Variant of MetaType that implements meta interface `MetaCallable`. It can be a pointer to member method, a pointer to global free method to simulate static member, or even `std::function`.  
The returned `RegisteredCallable` can be used to add annotations to the meta data.  

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


## MetaClass member functions for retrieving meta data

Most functions to retrieve meta data has a parameter `const MetaClass::Flags flags` with default value of `MetaClass::flagIncludeBase`. For functions that find a certain meta data, that means if the function can't find the meta data in current meta class, it will look up all base class recursively for the meta data. For functions that get all meta data, that means the function will return all meta data in current meta class, and all meta data in base classes, recursively. If you want only current meta class be checked, pass `MetaClass::flagNone`.  

#### getConstructorList

```c++
const RegisteredConstructorList & getConstructorList() const;

using RegisteredConstructorList = std::deque<RegisteredConstructor>;
```

Get a list of RegisteredConstructor.  

#### getAccessible

```c++
const RegisteredAccessible & getAccessible(const std::string & name, const MetaClass::Flags flags = MetaClass::flagIncludeBase) const;
```

Get a field of `name`. If the field is not registered, an empty RegisteredAccessible is returned (RegisteredAccessible::isEmpty() is true).  

#### getAccessibleList

```c++
RegisteredAccessibleList getAccessibleList(const Flags flags = flagIncludeBase) const;

using RegisteredAccessibleList = std::deque<RegisteredAccessible>;
```

Get a list of all registered field.  

#### getCallable

```c++
const RegisteredCallable & getCallable(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a method of `name`. If the method is not registered, an empty RegisteredCallable is returned (RegisteredCallable::isEmpty() is true).  

#### getCallableList by name

```c++
RegisteredCallableList getCallableList(const std::string & name, const Flags flags = flagIncludeBase) const;

using RegisteredCallableList = std::deque<RegisteredCallable>;
```

Get a list of all registered methods which has `name`. This is useful to get the overload methods.  

#### getCallableList

```c++
RegisteredCallableList getCallableList(const Flags flags = flagIncludeBase) const;

using RegisteredCallableList = std::deque<RegisteredCallable>;
```

Get a list of all registered methods.  

#### getType by name

```c++
const RegisteredType & getType(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a RegisteredType of `name`. If the type name is not registered, an empty RegisteredType is returned (RegisteredType::isEmpty() is true).  

#### getType by type kind

```c++
const RegisteredType & getType(const TypeKind kind, const Flags flags = flagIncludeBase) const;
```

Get a RegisteredType of `kind`. If the type kind is not registered, an empty RegisteredType is returned (RegisteredType::isEmpty() is true).  

#### getType by MetaType

```c++
const RegisteredType & getType(const MetaType * metaType, const Flags flags = flagIncludeBase) const;
```

Get a RegisteredType of `metaType`. If the meta type is not registered, an empty RegisteredType is returned (RegisteredType::isEmpty() is true).   
This function is useful to get the name of a registered meta type.

#### getTypeList

```c++
RegisteredTypeList getTypeList(const Flags flags = flagIncludeBase) const;

using RegisteredTypeList = std::deque<RegisteredType>;
```

Get a list of all registered types.  

