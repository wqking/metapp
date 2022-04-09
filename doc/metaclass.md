# MetaClass interface

## Overview

`MetaClass` is a meta interface to provide information of a class, such as constructors, member functions, member fields, etc.  

## Get MetaClass interface

We can call `MetaType::getMetaClass()` to get the MetaClass interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
```

## Implemented built-in meta types

None

## How to implement MetaClass

Usually MetaClass interface is implemented as static variable of `MetaClass` inside static function `getMetaClass` in template specialization `DeclareMetaType`.  

For example, to implement MetaClass for `MyClass`,   
```c++
template <>
struct metapp::DeclareMetaType<MyClass> : metapp::DeclareMetaTypeBase<MyClass>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyClass>(),
			[](metapp::MetaClass & mc) {
				// here add information to mc
			}
		);
		return &metaClass;
	}
};

```

## Header

```c++
#include "metapp/interfaces/metaclass.h"
```

## MetaClass member functions for registration information

#### MetaClass constructor

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
The MetaClass instance under constructing is passed as the parameter.

#### registerConstructor

```c++
RegisteredConstructor & registerConstructor(const Variant & constructor);
```

Register a constructor. The parameter `constructor` is a Variant of `metapp::Constructor`.  

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

## MetaClass member functions for retrieving information

