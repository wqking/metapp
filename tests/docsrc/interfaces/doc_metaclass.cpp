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

#include "metapp/allmetatypes.h"

/*desc
# MetaClass interface

## Overview

`MetaClass` is a meta interface to provide meta information of a class, such as constructors, member functions, member fields, etc.  

## Header
desc*/

//code
#include "metapp/interfaces/metaclass.h"
//code

/*desc
## Get MetaClass interface

We can call `MetaType::getMetaClass()` to get the MetaClass interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<SomeClass>();
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

## MetaClass member functions for registering meta data

#### registerConstructor

```c++
MetaItem & registerConstructor(const Variant & constructor);
```

Register a constructor. The parameter `constructor` is a Variant of `metapp::Constructor`.  
The returned `MetaItem` can be used to add annotations to the meta data.

**Example**  
desc*/

//code
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
//code

/*desc
#### registerAccessible

```c++
MetaItem & registerAccessible(const std::string & name, const Variant & field);
```

Register a field (member or static member data).
The parameter `name` is the field name. The field can be got from the MetaClass by the name later. If a field with the same name has already registered, `registerAccessible` doesn't register the new field and returns the previous registered field.  
The parameter `field` is a Variant of MetaType that implements meta interface `MetaAccessible`. It can be pointer to member data, accessorpp::Accessor, or pointer to global data to simulate static member.  
The returned `MetaItem` can be used to add annotations to the meta data.  

**Example**  
desc*/

//code
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
//code

/*desc
#### registerCallable

```c++
MetaItem & registerCallable(const std::string & name, const Variant & callable);
```
Register a method (member or static member method).  
The parameter `name` is the method name. metapp allows multiple methods be registered under the same name,, they are treated as overloaded methods.  
The parameter `callable` is a Variant of MetaType that implements meta interface `MetaCallable`. It can be a pointer to member method, a pointer to non-member free method to simulate static member, or even `std::function`.  
The returned `MetaItem` can be used to add annotations to the meta data.  

**Example**  
desc*/

//code
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
//code

/*desc
#### registerVariable

```c++
MetaItem & registerVariable(const std::string & name, const Variant & variable);
```

Register a variable.  
The parameter `name` is the variable name.  
The parameter `variable` is a Variant of any value.  
The returned `MetaItem` can be used to add annotations to the meta data.  

The difference between `accessible` and `variable` is, an `accessible` must implement meta interface `MetaAccessible`, while
a `variable` can be any value. How to use a `variable` is up to the user.  
The best practice to decide when to use `accessible` or `variable` is, when a Variant will be got/set value via an accessible
such as a pointer to a variable, register it as `accessible`. If a Variant's value is not going to change, such as a constant,
register it as `variable`.  
Note: I'm not satisfied with the term `variable`. I've thought about constant, object, value, item, element, but none is satisfying.

**Example**  
desc*/

//code
class VarClass
{
public:
	static const int one = 1;

	int index;
};

template <>
struct metapp::DeclareMetaType<VarClass> : metapp::DeclareMetaTypeBase<VarClass>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<VarClass>(),
			[](metapp::MetaClass & mc) {
				mc.registerVariable("one", VarClass::one);
				// Any value can be registered as variable, not limited to members in the class
				mc.registerVariable("name", std::string("metapp"));
				// We want to set/get the `index` field, so we register it as accessible.
				mc.registerAccessible("index", &VarClass::index);
			}
		);
		return &metaClass;
	}
};
//code

/*desc
#### registerType

```c++
template <typename T>
MetaItem & registerType(const std::string & name = ""); // #1
	return registerType(name, getMetaType<T>());
}
MetaItem & registerType(std::string name, const MetaType * metaType); // #2
```

Register a MetaType.  
The #1 form is equivalent to `registerType(name, getMetaType<T>())`;  
If the parameter `name` is empty, the function tries to get the name from built-in types. If the name is not found, then the name is not used and the MetaType can't be got by name.  
The returned `MetaItem` can be used to add annotations to the meta data.  
This function can be used to register nested classes, or enum in the class.  

**Example**  
desc*/

//code
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
//code

/*desc
## MetaClass member functions for retrieving meta data

Most functions to retrieve meta data has a parameter `const MetaClass::Flags flags` with default value of `MetaClass::flagIncludeBase`. For functions that find a certain meta data, that means if the function can't find the meta data in current meta class, it will look up all base class recursively for the meta data. For functions that get all meta data, that means the function will return all meta data in current meta class, and all meta data in base classes, recursively. If you want only current meta class be checked, pass `MetaClass::flagNone`.  

#### getConstructor

```c++
const MetaItem & getConstructor() const;
```

Returns MetaItem of a callable of the constructors.  
If there are more than one constructors, the callable Variant is tkOverloadedFunction.  

**Example**  
desc*/

ExampleFunc
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<CtorClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();
	const metapp::MetaItem & constructor = metaClass->getConstructor();
	// Calling metapp::callableInvoke can invoke the proper function that matches the arguments
	metapp::Variant instance = metapp::callableInvoke(constructor, nullptr, "abc", 5);
	CtorClass * ptr = instance.get<CtorClass *>();
	ASSERT(ptr->s == "abc");
	ASSERT(ptr->n == 5);
	// Don't forget to delete the instance.  
	// A better approach is to use Variant::takeFrom() to convert the instance to a Variant managed object,
	// then we don't need to delete the instance.
	delete ptr;
	//code
}

/*desc
#### getAccessible

```c++
const MetaItem & getAccessible(const std::string & name, const MetaClass::Flags flags = MetaClass::flagIncludeBase) const;
```

Get a field of `name`. If the field is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getAccessibleView

```c++
MetaItemView getAccessibleView(const Flags flags = flagIncludeBase) const;
```

Returns a MetaItemView for all registered accessibles.  

**Example**  
desc*/

ExampleFunc
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<AccClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();
	AccClass object;
	object.text = "hello";
	const metapp::MetaItem & text = metaClass->getAccessible("text");
	ASSERT(metapp::accessibleGet(text, &object).get<const std::string &>() == "hello");

	const metapp::MetaItemView accessibleView = metaClass->getAccessibleView();
	ASSERT(accessibleView[0].getName() == "text");
	ASSERT(accessibleView[1].getName() == "value");
	//code
}

/*desc
#### getCallable

```c++
const MetaItem & getCallable(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a method of `name`. If the method is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getCallableView

```c++
MetaItemView getCallableView(const Flags flags = flagIncludeBase) const;
```

Returns a MetaItemView for all registered callables.  

**Example**  
desc*/

ExampleFunc
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<CaClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();
	CaClass object;

	const metapp::MetaItem & greeting = metaClass->getCallable("greeting");
	ASSERT(metapp::callableInvoke(greeting, &object, "world").get<const std::string &>() == "Hello, world");

	const metapp::MetaItem & add = metaClass->getCallable("add");
	ASSERT(metapp::callableInvoke(add, &object, 1, 5).get<int>() == 6);

	const metapp::MetaItemView callableView = metaClass->getCallableView();
	ASSERT(callableView.size() == 2);
	ASSERT(callableView[0].getName() == "greeting");
	ASSERT(callableView[1].getName() == "add");
	//code
}

/*desc
#### getVariable

```c++
const MetaItem & getVariable(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a variable of `name`. If the variable is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getVariableView

```c++
MetaItemView getVariableView(const Flags flags = flagIncludeBase) const;
```

Returns a MetaItemView for all registered variables.  

**Example**  
desc*/

ExampleFunc
{
	//code
	const metapp::MetaType * metaType = metapp::getMetaType<VarClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	const metapp::MetaItem & one = metaClass->getVariable("one");
	ASSERT(one.asVariable().get<int>() == 1);

	const metapp::MetaItem & name = metaClass->getVariable("name");
	ASSERT(name.asVariable().get<const std::string &>() == "metapp");
	//code
}

/*desc
#### getType by name

```c++
const MetaItem & getType(const std::string & name, const Flags flags = flagIncludeBase) const;
```

Get a MetaItem of `name`. If the type name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getType by type kind

```c++
const MetaItem & getType(const TypeKind kind, const Flags flags = flagIncludeBase) const;
```

Get a MetaItem of `kind`. If the type kind is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getType by MetaType

```c++
const MetaItem & getType(const MetaType * metaType, const Flags flags = flagIncludeBase) const;
```

Get a MetaItem of `metaType`. If the meta type is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).   
This function is useful to get the name of a registered meta type.

#### getTypeView

```c++
MetaItemView getTypeView(const Flags flags = flagIncludeBase) const;
```

Returns a MetaItemView for all registered types.  

desc*/
