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
#include "metapp/interfaces/metaclass.h"

#include <cstring>
#include <sstream>

/*desc
# metapp -- C++ library for runtime reflection, introspection and meta types

metapp is a cross platform C++ runtime reflection library.  
metapp is light weight, powerful, fast, unique, non-intrusive, no macros, and easy to use.  
Even if you don't need reflection, you may use `metapp::Variant` with any C++ types, and you can enjoy the large amount of
built-in meta types.

## Highlight and unique features

- **Allow to retrieve any C++ type information at runtime, such as primary types, pointer, reference, function, template,
const-volatile qualifiers, and much more.** Can you understand the type `char const *(*(* volatile * (&)[][8])())[]` easily?
metapp can understand it, including every CV, pointer, array, function, etc, in no time!
- **Allow runtime generic programming.** For example, we can access elements in a container without knowing whether
the container is `std::vector` or `std::deque` or `std::list`, and without knowing whether the value type is `int`,
`std::string`, `MyFancyClass`, or another container.
- **Very easy to reflect templates.** For example, we only need to reflect `std::vector` or `std::list` once,
then we can get meta information for `std::vector<int>`, `std::vector<std::string>`, or
even `std::vector<std::list<std::vector<std::string> > >`.
- **Imitate C++ reference extensively for better performance.** For example, when getting a property value,
or get an element value from a container, a `metapp::Variant` of reference to the element is returned when possible, the element
value is referenced instead of copied, so the memory and performance cost is kept as minimum as possible.
- **Good performance.** The performance is roughly similar to or better than Qt meta system. The execution speed can no way be
close to native C++, but it's fast enough for reflection library.

## Facts and features

- **Powerful**
	- Support any C++ type information, such as primary, pointer, reference, function, template, const-volatile qualifiers,
		and much more.
	- Support runtime generic programming.
	- Unreflected types can be inspected. Reflection only provides more information, but not a must.
	- True runtime reflection. Accessing fields and properties, calling methods, are truly runtime behavior,
		no template parameters are needed. All parameters and return values are passed via metapp::Variant.
	- Imitate C++ reference extensively for better performance.
	- Good performance. The performance is roughly similar to or better than Qt meta system. Performance, binary size and compile time
		are continuously optimized.
	- Automatically type conversion when getting/setting fields, invoking methods, etc.
	- Support multiple inheritance and hierarchy.
	- Support using in dynamic library (plugins).
	- Minimum usage on global stateful static data, be loose couple with global state as much as possible.
	- Extendable. The core library is very slim with very few functions, most functions are implemented by `meta interface`
		which is very powerful and extendable.

- **Flexible and easy to use**
	- Building meta data doesn't require preprocessor, macros, or any external tool. Only use native C++ code, no need macros.
	- Very easy to reflect templates.
	- Non-intrusive. You don't need to modify your code for reflection.
	- Doesn't require C++ RTTI.
	- Use exceptions by default, but exceptions can be disabled.
	- No configuration, no policies, easy to use, while keeps powerful.
	- Loose coupled design. For example, constructors and member functions can be used without coupling with the class information.
	- You don't pay for what you don't use. If you don't build the meta data, no any memory overhead.
		If you don't use the meta data, no any performance overhead. If you build and use the meta data,
		you get trivial memory and performance overhead beside very powerful reflection system.
	- Written in standard and portable C++, only require C++11, and support later C++ standard
		(such as std::any and std::variant are supported if the compiler supports C++17).
	- Cross platforms, cross compilers.
	- No external dependency.
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

### Version 0.1.0
![CI](https://github.com/wqking/metapp/workflows/CI/badge.svg)

The project is under working in progress.  
The first stable release will be v1.0.0. 

To put the library to first release, we need to,   
1. Add more tests.
2. Finish the metapp based Lua bind project (under development).
3. Complete the documentations.

Any significant features such as supporting operators reflection will be developed after v1.0.0 is released.

You are welcome to try the project and give feedback. Your participation will help to make the development faster and better.

### Source code

[https://github.com/wqking/metapp](https://github.com/wqking/metapp)

### Supported compilers

metapp requires C++ compiler that supports C++11 standard.  
The library is tested with MSVC 2022, 2019, MinGW (Msys) GCC 8.3 and 11.3.0, Clang (carried by MSVC).  
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

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

Note: when using the method 2 with MinGW on Windows, by default CMake will install metapp in system folder which is not writable.
You should specify another folder to install.
To do so, replace `cmake ..` with `cmake .. -DCMAKE_INSTALL_PREFIX="YOUR_NEW_LIB_FOLDER"`.

## Example code

Here are some simple code pieces. There are comprehensive tutorials in the documentations.
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
	//code
	//desc v contains int.
	metapp::Variant v { 5 };
	// Get the value
	ASSERT(v.get<int>() == 5);
	//desc cast v to double
	metapp::Variant casted = v.cast<double>();
	ASSERT(casted.get<double>() == 5.0);

	//desc Now v contains char array.
	v = "hello";
	ASSERT(strcmp(v.get<char []>(), "hello") == 0);
	//desc Cast to std::string.
	casted = v.cast<std::string>();
	// Get as reference to avoid copy.
	ASSERT(casted.get<const std::string &>() == "hello");
	//code
}

//desc ### Inspect MetaType
ExampleFunc
{
	//code
	//desc Let's inspect the type `const std::map<const int, std::string> * volatile *`
	const metapp::MetaType * metaType = metapp::getMetaType<
		const std::map<const int, std::string> * volatile *>();
	ASSERT(metaType->isPointer()); // The type is pointer
	
	// Second level pointer
	const metapp::MetaType * secondLevelPointer = metaType->getUpType();
	ASSERT(secondLevelPointer->isPointer());
	ASSERT(secondLevelPointer->isVolatile()); //second level pointer is volatile
	
	// The pointed type (const std::map<const int, std::string>).
	const metapp::MetaType * pointed = secondLevelPointer->getUpType();
	ASSERT(pointed->isConst());
	ASSERT(pointed->getTypeKind() == metapp::tkStdMap);
	// Key type
	ASSERT(pointed->getUpType(0)->getTypeKind() == metapp::tkInt);
	ASSERT(pointed->getUpType(0)->isConst()); //key is const
	// Mapped type.
	ASSERT(pointed->getUpType(1)->getTypeKind() == metapp::tkStdString);
	//code
}

//desc ### Reflect a class (declare meta type)
//code
//desc Here is the class we are going to reflect for.
class MyPet
{
public:
	MyPet() : name(), age() {}
	MyPet(const std::string & name, const int age) : name(name), age(age) {}

	int getAge() const { return age; }
	void setAge(const int newAge) { age = newAge; }

	std::string bark() const { return "Bow-wow, " + name; }
	int calculate(const int a, const int b) const { return a + b; }

	std::string name; // I don't like public field in non-POD, here is only for demo
private:
	int age;
};

// We can use factory function as constructor.
MyPet * createMyPet(const std::string & name, const int birthYear, const int nowYear)
{
	return new MyPet(name, nowYear - birthYear);
}

//desc Now let's `DeclareMetaType` for MyPet. We `DeclareMetaType` for all kinds of types,
//desc not only classes, but also enumerators, templates, etc.
template <>
struct metapp::DeclareMetaType<MyPet> : metapp::DeclareMetaTypeBase<MyPet>
{
	// Reflect the class information via MetaClass.
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyPet>(),
			[](metapp::MetaClass & mc) {
				// Register constructors
				mc.registerConstructor(metapp::Constructor<MyPet ()>());
				mc.registerConstructor(metapp::Constructor<MyPet (const std::string &, int)>());
				// Factory function as constructor
				mc.registerConstructor(&createMyPet);

				// Register field with getter/setter function
				mc.registerAccessible("age",
					metapp::createAccessor(&MyPet::getAge, &MyPet::setAge));
				// Register another field
				mc.registerAccessible("name", &MyPet::name);

				// Register member functions
				mc.registerCallable("bark", &MyPet::bark);
				mc.registerCallable("calculate", &MyPet::calculate);
			}
		);
		return &metaClass;
	}
};

//code
//desc Now let's use the reflected meta class.  
ExampleFunc
{
	//code
	//desc Obtain the meta type for MyPet, then get the meta class. If we've registered the meta type of MyPet
	//desc to MetaRepo, we can get it at runtime instead of depending on the compile time `getMetaType`.
	const metapp::MetaType * metaType = metapp::getMetaType<MyPet>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();

	//desc `getConstructor`, then invoke the constructor as if it's a normal callable, with proper arguments.
	//desc Then obtain the MyPet instance pointer from the returned Variant and store it in a `std::shared_ptr`.  
	//desc The constructor is an overloaded callable since there are three constructors registered,
	//desc `metapp::callableInvoke` will choose the proper callable to invoke.
	std::shared_ptr<MyPet> myPet(metapp::callableInvoke(metaClass->getConstructor(), nullptr,
		"Lovely", 3).get<MyPet *>());
	// Verify the object is constructed properly.
	ASSERT(myPet->name == "Lovely");
	ASSERT(myPet->getAge() == 3);
	// Call the factory function, the result is same as myPet with name == "Lovely" and age == 3.
	std::shared_ptr<MyPet> myPetFromFactory(metapp::callableInvoke(metaClass->getConstructor(), nullptr,
		"Lovely", 2019, 2022).get<MyPet *>());
	ASSERT(myPetFromFactory->name == "Lovely");
	ASSERT(myPetFromFactory->getAge() == 3);

	//desc Get field by name then get the value.
	const auto & propertyName = metaClass->getAccessible("name");
	ASSERT(metapp::accessibleGet(propertyName, myPet).get<const std::string &>() == "Lovely");
	const auto & propertyAge = metaClass->getAccessible("age");
	ASSERT(metapp::accessibleGet(propertyAge, myPet).get<int>() == 3);

	//desc Set field `name` with new value.
	metapp::accessibleSet(propertyName, myPet, "Cute");
	ASSERT(metapp::accessibleGet(propertyName, myPet).get<const std::string &>() == "Cute");

	//desc Get member function then invoke it.
	const auto & methodBark = metaClass->getCallable("bark");
	ASSERT(metapp::callableInvoke(methodBark, myPet).get<const std::string &>() == "Bow-wow, Cute");

	const auto & methodCalculate = metaClass->getCallable("calculate");
	// Pass arguments 2 and 3 to `calculate`, the result is 2+3=5.
	ASSERT(metapp::callableInvoke(methodCalculate, myPet, 2, 3).get<int>() == 5);
	//code
}

//desc ### Runtime generic algorithm on STL container
//desc Let's define a `concat` function that processes any Variant that implements meta interface MetaIterable
//code
std::string concat(const metapp::Variant & container)
{
	// `container` may contains a pointer such as T *. We use `metapp::depointer` to convert it to equivalent
	// non-pointer such as T &, that eases the algorithm because we don't care pointer any more.
	const metapp::Variant nonPointer = metapp::depointer(container);
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
	//desc Construct a Variant with the vector. To avoid container1 being copied, we move the container1 into Variant.
	metapp::Variant v1 = std::move(container1);
	ASSERT(container1.empty()); // container1 was moved
	//desc Concat the items in the vector.
	ASSERT(concat(v1) == "15967");

	//desc We can also use std::list. Any value can convert to Variant implicitly, so we can pass the container std::list on the fly.
	ASSERT(concat(std::list<std::string>{ "Hello", "World", "Good" }) == "HelloWorldGood");

	//desc std::tuple is supported too, and we can use heterogeneous types.
	ASSERT(concat(std::make_tuple("A", 1, "B", 2)) == "A1B2");
	//desc Isn't cool we can use std::pair as a container?
	ASSERT(concat(std::make_pair("Number", 1)) == "Number1");

	//desc We can even pass a pointer to container to `concat`.
	std::deque<int> container2 { 1, 2, 3 };
	ASSERT(concat(&container2) == "123");
	//code
}

//desc ### Use reference with Variant

ExampleFunc
{
	{
		//code
		//desc Declare a value to be referred to.
		int n = 9;
		//desc rn holds a reference to n.
		//desc C++ equivalence is `int & rn = n;`
		metapp::Variant rn = metapp::Variant::reference(n);
		ASSERT(rn.get<int>() == 9);
		//desc Assign to rn with new value. C++ equivalence is `rn = (int)38.1;` where rn is `int &`.
		//desc Here we can't use `rn = 38.1;` where rn is `Variant`, that's different meaning.
		//desc See Variant document for details.
		rn.assign(38.1); // different with rn = 38.1, `rn = 38.1` won't modify n
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

//desc ### Work with unreflected types
//code
//desc Assume we have two types that we don't reflect for.
struct UnreflectedFoo { int f; };
enum class UnreflectedBar { one, two };
//desc Surely we can't get any member function or property information for the classes since they are not reflected.
//desc But at least we can,  
//code
ExampleFunc
{
	//code
	//desc 1, Construct the object using default constructor.
	std::unique_ptr<UnreflectedFoo> foo(static_cast<UnreflectedFoo *>(
		metapp::getMetaType<UnreflectedFoo>()->construct()));
	ASSERT(foo->f == 0);

	//desc 2, Copy construct the object.
	foo->f = 38; // Change member value to prove the value is copied.
	std::unique_ptr<UnreflectedFoo> copiedFoo(static_cast<UnreflectedFoo *>(
		metapp::getMetaType<UnreflectedFoo>()->copyConstruct(foo.get())));
	ASSERT(copiedFoo->f == 38);

	//desc 3, Construct the object inplace.
	UnreflectedFoo foo2 { 9 };
	ASSERT(foo2.f == 9);
	metapp::getMetaType<UnreflectedFoo>()->placementConstruct(&foo2);
	ASSERT(foo2.f == 0);

	//desc 4, Copy construct the object inplace.
	foo2.f = 38;
	UnreflectedFoo foo3 { 19 };
	ASSERT(foo3.f == 19);
	metapp::getMetaType<UnreflectedFoo>()->placementCopyConstruct(&foo3, &foo2);
	ASSERT(foo3.f == 38);

	//desc 5, Destroy the object, if we dont' use RAII
	void * foo5 = metapp::getMetaType<UnreflectedFoo>()->construct();
	// `destroy` knows the type of foo5 and can destroy the `void *` properly.
	metapp::getMetaType<UnreflectedFoo>()->destroy(foo5);

	//desc 6, Identify the meta type
	const UnreflectedFoo * fooPtr = &foo3;
	metapp::Variant varFoo = fooPtr;
	metapp::Variant varBar = UnreflectedBar();
	ASSERT(varBar.getMetaType()->equal(metapp::getMetaType<UnreflectedBar>()));
	ASSERT(varFoo.getMetaType()->isPointer());
	ASSERT(varFoo.getMetaType()->getUpType()->isConst());
	ASSERT(varFoo.getMetaType()->equal(metapp::getMetaType<const UnreflectedFoo *>()));
	ASSERT(varFoo.getMetaType()->getUpType()->equal(metapp::getMetaType<const UnreflectedFoo>()));
	//code
}

/*desc
## Documentations
Below are tutorials and documents. Don't feel upset if you find issues or missing stuff in the documents, I'm not
native English speaker and it's not that exciting to write document. Any way, the code quality is always much better
than the document, for ever.  
If you want to contribute to the documents, be sure to read [How to generate documentations](doc/about_document.md).  

//@include inc/inc_doc_index.md

## Build the test code

There are several parts of code to test the library,

- unittests: tests the library.
- docsrc: documentation source code, and sample code to demonstrate how to use the library. 
- benchmark: measure the performance.

All parts are in the `tests` folder.

All parts require CMake to build, and there is a makefile to ease the building.  
Go to folder `tests/build`, then run `make` with different target.
- `make vc19` #generate solution files for Microsoft Visual Studio 2019, then open metapptest.sln in folder project_vc19
- `make vc17` #generate solution files for Microsoft Visual Studio 2017, then open metapptest.sln in folder project_vc17
- `make vc15` #generate solution files for Microsoft Visual Studio 2015, then open metapptest.sln in folder project_vc15
- `make mingw` #build using MinGW
- `make linux` #build on Linux
- `make mingw_coverage` #build using MinGW and generate code coverage report

## Known compiler related quirks in MSVC

MSVC 2022 and 2019, can build the CMake generated test projects and the tests run correctly in Debug and RelWithDebugInfo
configurations. But some tests fail in Release mode when incremental linking is disabled.  
Those failed tests should not fail, because they work correct in MSVC debug mode and in GCC/Clang.
Adding /Debug option to linking which generates debug information makes the tests success.  
Without /Debug option, but enabling incremental linking, will cause the tests success too.  
So if metapp shows weird behavior in MSVC, try to enable incremental linking.

## Motivations

I (wqking) developed `cpgf` library since more than 12 years ago. `cpgf` works, but it has several serious problems.
The first problem is it was written in C++03, the code is verbose and difficult to write. The second problem is that
it includes too many features in a single library -- reflection, serialization, script binding (Lua, Python, JavaScript).
`cpgf` became unmanageable and can't be developed in an elegant way.  
`metapp` is a fresh library that only focuses on reflection. Other features, such as serialization,
script binding, will be in separated projects, if they are developed.

desc*/
