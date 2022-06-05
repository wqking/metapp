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
#include "metapp/interfaces/metaclass.h"

/*desc
# Meta interface overview 

## Overview

Meta interface is an optional way to extend `MetaType` functionalities.
If a template specialization of `DeclareMetaType` implements a meta interface, for example, `MetaClass`,
then the user can retrieve the interface `MetaClass` and use all functions provided in `MetaClass`.
If the specialization doesn't implement such interface, the user can not use the interface,
and thus there is no (almost) any overhead for the interface.  
Note: the term "interface" here is an extended concept. It's not the "interface" concept in the traditional
Object Oriented Programming.  

## Get meta interface from MetaType

There are methods in `MetaType` to get the meta interface. A meta interface is a pointer to the interface.  
If the meta interface is not implemented, `nullptr` is returned.  

Here is a list of MetaType member functions to retrieve meta interfaces,  
```c++
const MetaClass * getMetaClass() const;
const MetaCallable * getMetaCallable() const;
const MetaAccessible * getMetaAccessible() const;
const MetaEnum * getMetaEnum() const;
const MetaIndexable * getMetaIndexable() const;
const MetaIterable * getMetaIterable() const;
const MetaStreamable * getMetaStreamable() const;
const MetaMappable * getMetaMappable() const;
const void * getMetaUser() const;
```

For example, assume meta type of `MyClass` implements the `MetaClass` interface, then we get the meta interface as,  
desc*/

ExampleFunc
{
	//code
	class MyClass {};
	const metapp::MetaType * metaType = metapp::getMetaType<MyClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();
	//code
	(void)metaClass;
}

/*desc
## Implement meta interface

We can implement meta interface in the specialization of DeclareMetaType for the type we want to declare.  
An "implemented" meta interface is just a static function with exactly same name and prototype as the member functions in MetaType.  
The static function defines a static object of the meta interface with proper arguments to the constructor,
then returns a pointer to the static object.  
A meta type can implement more than one meta interfaces.  
The static function can be also a static function pointer, and it's assigned some elsewhere such as in the .cpp source file.
That allows to decouple the interface implementation from the DeclareMetaType specialization.  

For example, to implement meta interface `MetaClass` for `MyClass`,  
desc*/

//code
class MyClass
{
public:
	explicit MyClass(const std::string & /*message*/) {}
	void sayHello() const {}
};

template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyClass>(),
			[](metapp::MetaClass & mc) {
				mc.registerConstructor(metapp::Constructor<MyClass (const std::string &)>());
				mc.registerCallable("sayHello", &MyClass::sayHello);
			}
		);
		return &metaClass;
	}

	// Previous function can also be function pointer, it's assigned elsewhere
	// For example, 
	//static const metapp::MetaClass * (*getMetaClass)();
	// In .cpp file
	// metapp::DeclareMetaType <MyClass>::getMetaClass = someFunction;
};
//code

/*desc
desc*/
