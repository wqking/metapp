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
#include "metapp/interfaces/metaiterable.h"

#include <cstring>
#include <sstream>

/*desc
# metapp -- C++ library for runtime reflection, introspection and meta types

metapp is a cross platform C++ library that adds powerful reflection feature to C++.  

## Highlight features

- **Allow to retrieve any C++ type information at runtime, such as primary types, pointer, reference, function, template,
const-volatile qualifiers, and much more.** Can you understand the type `char const *(*(* volatile * (&)[][8])())[]` easily?
metapp can understand it, including every CV, pointer, array, function, etc, in no time!   
- **Allow runtime generic programming.** For example, we can access elements in a container, without knowing whether
the container is `std::vector` or `std::deque` or `std::list`, and without knowing whether the value type is `int`,
or `std::string`, or another container.  
- **Very easy to reflect templates.** For example, we only need to reflect `std::vector` or `std::list` once,
then we can get meta information for `std::vector<int>`, `std::vector<std::string>`, or
even `std::vector<std::list<std::vector<std::string> > >`.
- **Mimic C++ reference extensively for better performance.** For example, when getting a property (accessible, in metapp term) value,
or get an element value from a container, a metapp::Variant of reference is returned when possible,
and the performance cost is kept as minimum as possible.

## Facts and features

- **Powerful**
	- Support any C++ type information, such as primary, pointer, reference, function, template, const-volatile qualifiers,
		and much more.
	- Support runtime generic programming.
	- True runtime reflection. Accessing fields and properties, calling methods, are truly runtime behavior,
		no template parameters are needed. All parameters and return values are passed via metapp::Variant.
	- Mimic C++ reference extensively for better performance.
	- Automatically type conversion when getting/setting fields, invoking methods, etc.
	- Support multiple inheritance and hierarchy.
	- Support using in dynamic library (plugins).

- **Flexible and easy to use**
	- Building meta data doesn't require preprocessor, macros, or any external tool. Only use native C++ code, no need macros.
	- Very easy to reflect templates.
	- Non-intrusive. You don't need to change your code for reflection.
	- Doesn't require C++ RTTI.
	- Use exceptions by default, but exceptions can be disabled.
	- No configuration, no policies, easy to use, while keeps powerful.
	- Loose coupled design. For example, constructors and member functions can be used without coupling with the class information.
	- You don't pay for what you don't use. If you don't build the meta data, no any memory overhead.
		If you don't use the meta data, no any performance overhead. If you build and use the meta data,
		you get trivial memory and performance overhead beside very powerful reflection system.
	- Written in standard and portable C++, only require C++11, and support later C++ standard.
	- Cross platforms, cross compilers.
	- The API is designed carefully and thoughtfully, and try to be enjoyable to use.

- **Language features that can be reflected**
	- Const volatile qualifiers, include top level CV, and CV in pointer, array and member function.
	- Pointer, reference.
	- Classes and nested inner classes.
	- Templates.
	- Accessibles (global variable, member data, property with getter/setter, etc).
	- Callables (global function, member function, constructor, std::function, etc).
	- Overloaded function.
	- Default arguments of functions.
	- Functions with variadic parameters.
	- Enumerators.
	- Constants in any data type.
	- Namespace simulation.
	- Array, multi-dimensional array.

- **Built-in reflected meta types**
	- Primary types (void, bool, int, unsigned int, etc).
	- STL strings (std::string, std::wstring).
	- STL smart pointers (std::shared_ptr, std::unique_ptr, std::weak_ptr).
	- STL containers (std::vector, std::list, std::map, etc).
	- Callables (free function, member function, std::function, constructor, overloaded function, default arguments function, variadic function, etc).
	- Many other meta types.
	- Adding new meta types is easy.

## Basic information

### License

Apache License, Version 2.0  

### Version 0.1.0 and status
![CI](https://github.com/wqking/metapp/workflows/CI/badge.svg)

The project is under working in progress.  
The first stable release will be v1.0.0. 

To put the library to first release, we need to,   
1. Add more test.
2. Finish the metapp based Lua bind project (under development).
3. Complete the documentations.

You are welcome to try the project and give feedback. Your participation will help to make the development faster and better.

### Source code

[https://github.com/wqking/metapp](https://github.com/wqking/metapp)

### Supported compilers

Tested with MSVC 2022, MinGW (Msys) GCC 8.3, Ubuntu GCC 5.4.
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

### C++ standard requirements
* To Use the library  
	* The library: C++11.  
* To develop the library
	* Unit tests: C++17.
	* docsrc: C++11.

## Quick start

### Namespace

`metapp`

### Build and install, use metapp in your project

There are various methods to use metapp

1, Include the source code in your project directly.

Clone the source code, then add the 'include' folder inside metapp to your project include directory,
then add the source files int the 'src' folder to your project, then you can use the library.  
`metapp` doesn't need any configuration.

2, Or install using CMake and use it in CMake

If you are going to use metapp in CMake managed project, you can install metapp then use it in CMake.  
In metapp root folder, run the commands,  
```
mkdir build
cd build
cmake ..
sudo make install
```

Then in the project CMakeLists.txt,   
```
# the project target is mytest, just for example
add_executable(mytest test.cpp)

find_package(metapp)
if(metapp_FOUND)
target_link_libraries(mytest metapp::metapp)
else(metapp_FOUND)
message(FATAL_ERROR "metapp library is not found")
endif(metapp_FOUND)
```

Note: when using the method 2 with MingW on Windows, by default CMake will install metapp in system folder which is not writable.
You should specify another folder to install.
To do so, replace `cmake ..` with `cmake .. -DCMAKE_INSTALL_PREFIX="YOUR_NEW_LIB_FOLDER"`.

## Example code

Here is simple code pieces. There are comprehensive tutorials documentations.
desc*/
//desc ### Use Variant

//code
//desc Header for Variant
#include "metapp/variant.h"
//desc To use all declared meta types, include this header
#include "metapp/allmetatypes.h"
//code

ExampleFunc
{
	{
		//code
		//desc v contains int.
		metapp::Variant v(5);
		//desc Get the value
		ASSERT(v.get<int>() == 5);
		//code
	}

	{
		//code
		//desc Now v contains std::string.
		metapp::Variant v = std::string("hello");
		//desc Get as reference to avoid copy.
		ASSERT(v.get<const std::string &>() == "hello");
		//desc Cast to const char *.
		metapp::Variant casted = v.cast<const char *>();
		const char * s = casted.get<const char *>();
		ASSERT(strcmp(s, "hello") == 0);
		//code
	}
}

//desc ### Use MetaType
ExampleFunc
{
	{
		//code
		//desc int
		const metapp::MetaType * metaType = metapp::getMetaType<int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		//code
	}

	{
		//code
		//desc constness
		auto metaType = metapp::getMetaType<const int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		ASSERT(metaType->isConst());
		//code
	}

	{
		//code
		//desc equivalence
		auto metaType = metapp::getMetaType<const int *>();
		ASSERT(metaType->equal(metapp::getMetaType<const int *>()));
		// Equivalence checking ignores all CV qualifiers.
		ASSERT(metaType->equal(metapp::getMetaType<int *>()));
		//code
	}
}

//desc ### Call function and accessible
ExampleFunc
{
	//code
	struct MyClass {
		int value;

		int add(const int delta1, const int delta2) const {
			return value + delta1 + delta2;
		}

		// metapp supports smart pointers very well.
		std::unique_ptr<MyClass> clone() const {
			// This is C++11, so we can't use std::make_unique
			return std::unique_ptr<MyClass>(new MyClass(*this));
		}
	};

	MyClass obj { 5 };

	metapp::Variant accessible(&MyClass::value);

	metapp::Variant funcAdd(&MyClass::add);
	//desc The second argument is the object instance, it's required when invoking member function.
	metapp::Variant resultOfAdd = metapp::callableInvoke(funcAdd, &obj, 3, 9);
	ASSERT(resultOfAdd.get<int>() == 17);

	metapp::Variant funcClone(&MyClass::clone);
	metapp::Variant cloned = metapp::callableInvoke(funcClone, &obj);
	//desc `cloned` holds std::unique_ptr<MyClass>
	ASSERT(cloned.getMetaType()->equal(metapp::getMetaType<std::unique_ptr<MyClass>>()));
	//desc We can pass the cloned Variant as the instance.
	ASSERT(metapp::callableInvoke(funcAdd, cloned, 5, 9).get<int>() == 19);

	//desc Get the value from accessible, which is MyClass::value.
	//desc The second argument is the object instance.
	//desc Since `cloned` is cloned freshly, both obj and cloned hold the same value.
	ASSERT(metapp::accessibleGet(accessible, &obj).get<int>() == 5);
	ASSERT(metapp::accessibleGet(accessible, cloned).get<int>() == 5);

	//desc Change value in obj
	obj.value = 6;
	ASSERT(metapp::accessibleGet(accessible, &obj).get<int>() == 6);
	//desc `cloned` is not affected.
	ASSERT(metapp::accessibleGet(accessible, cloned).get<int>() == 5);

	//desc Set value in `cloned`.
	metapp::accessibleSet(accessible, cloned, 38);
	//desc `obj` is not affected.
	ASSERT(metapp::accessibleGet(accessible, &obj).get<int>() == 6);
	ASSERT(metapp::accessibleGet(accessible, cloned).get<int>() == 38);
	//code
}

//desc ### Run time generic STL container
//desc Let's define a `concat` function that processes any Variant that implements meta interface MetaIterable
//code
std::string concat(const metapp::Variant & container)
{
	const metapp::Variant nonPointer = depointer(container);
	const metapp::MetaIterable * metaIterable
		= metapp::getNonReferenceMetaType(nonPointer)->getMetaIterable();
	if(metaIterable == nullptr) {
		return "";
	}
	std::stringstream stream;
	metaIterable->forEach(nonPointer, [&stream](const metapp::Variant & item) {
		stream << item;
		return true;
	});
	return stream.str();
}
//code

ExampleFunc
{
	//code
	//desc A std::vector of int.
	std::vector<int> container1 { 1, 5, 9, 6, 7 };
	//desc Construct a Variant with the vector.
	metapp::Variant v1(container1);
	//desc Concat the items in the vector.
	ASSERT(concat(v1) == "15967");

	//desc We can also use std::list. Any value can convert to Variant implicitly, so we can pass the container std::list on the fly.
	ASSERT(concat(std::list<std::string>{ "Hello", "World", "Good" }) == "HelloWorldGood");

	//desc Isn't cool we can use std::pair as a constainer?
	ASSERT(concat(std::make_pair("Number", 1)) == "Number1");
	//desc std::tuple is suppported too, and we can use heterogeneous types.
	ASSERT(concat(std::make_tuple("A", 1, "B", 2)) == "A1B2");

	//desc we can even pass a pointer to the container to `concat`.
	std::deque<int> container2 { 1, 2, 3 };
	metapp::Variant v2(&container2);
	ASSERT(concat(v2) == "123");
	//code
}

//desc ### Use reference with Variant

ExampleFunc
{
	{
		//code
		//desc Declare a value to be referred to.
		int n = 9;
		//desc rn holds a referent to n.
		//desc C++ equivalence is `int & rn = n;`
		metapp::Variant rn = metapp::Variant::reference(n);
		ASSERT(rn.get<int>() == 9);
		//desc Assign to rn with new value. 
		//desc C++ equivalence is `rn = 38;`
		rn.assign(38);
		//desc rn gets new value.
		ASSERT(rn.get<int>() == 38);
		//desc n is modified too.
		ASSERT(n == 38);
		//code
	}

	{
		//code
		//desc We can use reference to modify container elements as well.  
		//desc vs holds a `std::vector<std::string>`.
		metapp::Variant vs(std::vector<std::string> { "Hello", "world" });
		ASSERT(vs.get<const std::vector<std::string> &>()[0] == "Hello");
		//desc Get the first element. The element is returned as a reference.
		metapp::Variant item = metapp::indexableGet(vs, 0);
		//desc assign to item with new value.
		item.assign("Good");
		ASSERT(vs.get<const std::vector<std::string> &>()[0] == "Good");
		//code
	}
}

/*desc
Below are tutorials and documents.  
If you want to contribute to the documents, be sure to read [How to generate documentations](doc/about_document.md).  

## Documentations

- Tutorials

	- [Use Variant](doc/tutorial/tutorial_variant.md)
	- [Use MetaType](doc/tutorial/tutorial_metatype.md)
	- [Use MetaClass and meta data for class members](doc/tutorial/tutorial_metaclass.md)
	- [Use MetaCallable, function, member function, etc](doc/tutorial/tutorial_callable.md)
	- [Use MetaRepo to retrieve meta data at running time](doc/tutorial/tutorial_metarepo.md)
	- [Use classes inheritance](doc/tutorial/tutorial_metaclass_inheritance.md)

- Core concepts and classes
	- [Core concepts - type kind, meta type, up type, meta interface](doc/core_concepts.md)
	- [Class Variant reference](doc/variant.md)
	- [Class MetaType reference](doc/metatype.md)

- Build and use meta data
	- [Reflect meta type at compile time using DeclareMetaType](doc/declaremetatype.md)
	- [Register and use meta type at running time using MetaRepo](doc/metarepo.md)
	- [MetaItem](doc/metaitem.md)
	- [MetaItemView and BaseView](doc/views.md)

- Meta interfaces
	- [Overview and implement meta interface](doc/meta_interface_overview.md)
	- [MetaClass](doc/interfaces/metaclass.md)
	- [MetaCallable](doc/interfaces/metacallable.md)
	- [MetaAccessible](doc/interfaces/metaaccessible.md)
	- [MetaEnum](doc/interfaces/metaenum.md)
	- [MetaIndexable](doc/interfaces/metaindexable.md)
	- [MetaIterable](doc/interfaces/metaiterable.md)
	- [MetaStreaming](doc/interfaces/metastreaming.md)
	- [MetaMappable](doc/interfaces/metamappable.md)
	- [User defined meta interface](doc/interfaces/metauser.md)

- Built-in meta types
	- [Overview](doc/metatypes/overview_metatypes.md)
	- [List of all built-in meta types](doc/metatypes/list_all.md)
	- [Constructor](doc/metatypes/constructor.md)
	- [Overloaded function](doc/metatypes/overloaded_function.md)
	- [Default arguments function](doc/metatypes/default_args_function.md)
	- [Variadic function](doc/metatypes/variadic_function.md)
	- [Accessor](doc/metatypes/accessor.md)

- Utilities
	- [utility.h](doc/utilities/utility.md)

- Miscellaneous
	- [About documentations](doc/about_document.md)
	- Use metapp in dynamic library

## Build the test code

There are two parts of code to test the library,

- unittests: tests the library.
- docsrc: documentation source code, and sample code to demonstrate how to use the library. 

All parts are in the `tests` folder.

All parts require CMake to build, and there is a makefile to ease the building.  
Go to folder `tests/build`, then run `make` with different target.
- `make vc19` #generate solution files for Microsoft Visual Studio 2019, then open metapptest.sln in folder project_vc19
- `make vc17` #generate solution files for Microsoft Visual Studio 2017, then open metapptest.sln in folder project_vc17
- `make vc15` #generate solution files for Microsoft Visual Studio 2015, then open metapptest.sln in folder project_vc15
- `make mingw` #build using MinGW
- `make linux` #build on Linux
- `make mingw_coverage` #build using MinGW and generate code coverage report

## Known compiler related quirks

MSVC 2022 and 2019, can build the CMake generated test projects and the tests run correctly in Debug and RelWithDebugInfo
configurations. But some tests fails in Release mode when incremental linking is disabled.  
Adding /Debug option to linking which generates debug information makes the tests success.  
Without /Debug option, but enabling incremental linking, will cause the tests success too.  
So if metapp shows weird behavior in MSVC, try to enable incremental linking.

## Motivations

I (wqking) developed `cpgf` library in more than 12 years ago. `cpgf` works, but it has several serious problems.
The first problem is it was written in C++03, the code is verbose and difficult to write. The second problem is that
it includes too many features in a single library -- reflection, serialization, script binding (Lua, Python, JavaScript).  
`metapp` is a modern version of `cpgf`, and it will only include reflection feature. Other features, such as serialization,
script binding, will be in separated projects, if they are developed.

desc*/
