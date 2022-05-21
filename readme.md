[//]: # (Auto generated file, don't modify this file.)

# metapp -- C++ library for runtime reflection and meta types
<!--begintoc-->
* [Highlight features](#a2_1)
* [Facts and features](#a2_2)
* [Basic information](#a2_3)
  * [License](#a3_1)
  * [Version 0.1 and status](#a3_2)
  * [Source code](#a3_3)
  * [Supported compilers](#a3_4)
  * [C++ standard requirements](#a3_5)
* [Quick start](#a2_4)
  * [Namespace](#a3_6)
  * [Build and install, use metapp in your project](#a3_7)
* [Example code](#a2_5)
  * [Use Variant](#a3_8)
  * [Use MetaType](#a3_9)
  * [Call function](#a3_10)
* [Documentations](#a2_6)
* [Build the test code](#a2_7)
* [Known compiler related quirks](#a2_8)
* [Motivations](#a2_9)
<!--endtoc-->

metapp is a cross platform C++ library that adds powerful reflection feature to C++.  

<a id="a2_1"></a>
## Highlight features

- **Allow to retrieve any C++ type information at runtime, such as primary types, pointer, reference, function, template,
const-volatile qualifiers, and much more.** Can you understand the type `char const *(*(* volatile * [][8])())[]` in 20 seconds?
metapp can understand it, including every CV, pointer, array, function, etc, in no time!   
- **Allow runtime generic programming.** For example, we can access elements in a container, without knowing whether
the container is `std::vector` or `std::deque` or `std::list`, and without knowing whether the value type is `int`,
or `std::string`, or another container.  
- **Very easy to reflect templates.** For example, we only need to reflect `std::vector` or `std::list` once,
then we can get meta information for `std::vector<int>`, `std::vector<std::string>`, or
even `std::vector<std::list<std::vector<std::string> > >`.

<a id="a2_2"></a>
## Facts and features

- **Powerful**
    - Support any C++ type information, such as primary, pointer, reference, function, template, const-volatile qualifiers,
and much more.
    - Support runtime generic programming.
    - True runtime reflection. Accessing fields and properties, calling methods, are truly runtime behavior,
no template parameters are needed. All parameters and return values are passed via metapp::Variant.
    - Automatically type conversion when getting/setting fields, invoking methods, etc.
    - Support multiple inheritance and hierarchy.
    - Support using in dynamic library (plugins).

- **Flexible and easy to use**
    - Building meta data doesn't require preprocessor or any external tool. Only use native C++ code, no need macros.
    - Very easy to reflect templates. We only need to reflect for the template rather than any instantiations, then we can
get any instantiation information. Indeed there are built-in reflections for STL containers such std::vector, there is
only one reflection for `std::vector`, then we can get any information for `std::vector<int>`, `std::vector<std::string>`, etc.
    - Non-intrusive. You don't need to change your code for reflection.
    - Loose coupled design. For example, constructors and member functions can be used without coupling with the class information.
    - You don't pay for what you don't use. If you don't build the meta data, no any memory overhead.
If you don't use the meta data, no any performance overhead. If you build and use the meta data,
you get trivial memory and performance overhead beside very powerful reflection system.
    - Doesn't require C++ RTTI.
    - Written in standard and portable C++, only require C++11, and support later C++ standard.
    - Cross platforms, cross compilers.

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

<a id="a2_3"></a>
## Basic information

<a id="a3_1"></a>
### License

Apache License, Version 2.0  

<a id="a3_2"></a>
### Version 0.1 and status
![CI](https://github.com/wqking/metapp/workflows/CI/badge.svg)

The project is under working in progress.  
Currently the architecture is almost stable, though minor parts may be refactored.  
Most code should work correctly.  

To put the library to first release, we need to,   
1. Add more test.
2. Complete the documentations.

<a id="a3_3"></a>
### Source code

[https://github.com/wqking/metapp](https://github.com/wqking/metapp)

<a id="a3_4"></a>
### Supported compilers

Tested with MSVC 2022, MinGW (Msys) GCC 8.3, Ubuntu GCC 5.4.
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

<a id="a3_5"></a>
### C++ standard requirements
* To Use the library  
    * The library: C++11.  
* To develop the library
    * Unit tests: C++17.
    * docsrc: C++11.

<a id="a2_4"></a>
## Quick start

<a id="a3_6"></a>
### Namespace

`metapp`

<a id="a3_7"></a>
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
if(eventpp_FOUND)
target_link_libraries(mytest metapp::metapp)
else(eventpp_FOUND)
message(FATAL_ERROR "metapp library is not found")
endif(eventpp_FOUND)
```

Note: when using the method 3 with MingW on Windows, by default CMake will install metapp in system folder which is not writable.
You should specify another folder to install.
To do so, replace `cmake ..` with `cmake .. -DCMAKE_INSTALL_PREFIX="YOUR_NEW_LIB_FOLDER"`.

<a id="a2_5"></a>
## Example code

Here is simple code pieces. There are comprehensive tutorials documentations.

<a id="a3_8"></a>
### Use Variant
Header for Variant

```c++
#include "metapp/variant.h"
```

To use all declared meta types, include this header

```c++
#include "metapp/allmetatypes.h"
```

v contains int.

```c++
metapp::Variant v(5);
```

Get the value

```c++
ASSERT(v.get<int>() == 5);
```

Now v contains std::string.

```c++
metapp::Variant v = std::string("hello");
```

Get as reference to avoid copy.

```c++
ASSERT(v.get<const std::string &>() == "hello");
```

Cast to const char *.

```c++
metapp::Variant casted = v.cast<const char *>();
const char * s = casted.get<const char *>();
ASSERT(strcmp(s, "hello") == 0);
```

<a id="a3_9"></a>
### Use MetaType
int

```c++
const metapp::MetaType * metaType = metapp::getMetaType<int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
```

constness

```c++
auto metaType = metapp::getMetaType<const int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
ASSERT(metaType->isConst());
```

<a id="a3_10"></a>
### Call function

```c++
struct MyClass {
    int value;

    int add(const int delta1, const int delta2) const {
        return value + delta1 + delta2;
    }
};

metapp::Variant v(&MyClass::add);
MyClass obj { 5 };
// The second argument is the pointer to obj, it's required when invoking member function
metapp::Variant result = metapp::callableInvoke(v, &obj, 3, 9);
ASSERT(result.get<int>() == 17);
```

Below are tutorials and documents.  
If you want to contribute to the documents, be sure to read [How to generate documentations](doc/about_document.md).  

<a id="a2_6"></a>
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

<a id="a2_7"></a>
## Build the test code

There are two parts of code to test the library,

- unittests: tests the library.
- docsrc: documentation source code, and sample code to demonstrate how to use the library. 

All parts are in the `tests` folder.

All parts require CMake to build, and there is a makefile to ease the building.  
Go to folder `tests/build`, then run `make` with different target.
- `make vc19` #generate solution files for Microsoft Visual Studio 2019, then open eventpptest.sln in folder project_vc19
- `make vc17` #generate solution files for Microsoft Visual Studio 2017, then open eventpptest.sln in folder project_vc17
- `make vc15` #generate solution files for Microsoft Visual Studio 2015, then open eventpptest.sln in folder project_vc15
- `make mingw` #build using MinGW
- `make linux` #build on Linux
- `make mingw_coverage` #build using MinGW and generate code coverage report

<a id="a2_8"></a>
## Known compiler related quirks

MSVC 2022 and 2019, can build the CMake generated test projects and the tests run correctly in Debug and RelWithDebugInfo
configurations. But some tests fails in Release mode when incremental linking is disabled.  
Adding /Debug option to linking which generates debug information makes the tests success.  
Without /Debug option, but enabling incremental linking, will cause the tests success too.  
So if metapp shows weird behavior in MSVC, try to enable incremental linking.

<a id="a2_9"></a>
## Motivations

I (wqking) developed `cpgf` library in more than 12 years ago. `cpgf` works, but it has several serious problems.
The first problem is it was written in C++03, the code is verbose and difficult to write. The second problem is that
it includes too many features in a single library -- reflection, serialization, script binding (Lua, Python, JavaScript).  
`metapp` is a modern version of `cpgf`, and it will only include reflection feature. Other features, such as serialization,
script binding, will be in separated projects, if they are developed.
