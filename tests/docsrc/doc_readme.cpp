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

#include "tutorial.h"

#define FN_PREFIX docReadme_

#include "metapp/allmetatypes.h"

/*desc
# metapp -- C++ library for runtime reflection and meta types

metapp is a cross platform C++ library that adds powerful reflection feature to C++.  

## Highlight features

- Allow to retrieve any C++ type information at runtime, such as primary types, pointer, reference, function, template, const-volatile qualifiers, and much more. Can you understand the type `char *(*(* * [][8])())[]` in 20 seconds? metapp can understand it in no time!   
- Allow runtime generic programming. For example, we can access elements in a container, without knowing whether the container is `std::vector` or `std::deque` or `std::list`, and without knowing whether the value type is `int`, or `std::string`, or another container.  

## Facts and features

- **Powerful**
    - Support any C++ type information, such as primary, pointer, reference, function, template, const-volatile qualifiers, and much more.
    - Support runtime generic programming.
    - True runtime reflection. Accessing fields and properties, calling methods, are truly runtime behavior, no template parameters are needed. All parameters and return values are passed via metapp::Variant.
    - Automatically type conversion when getting/setting fields, invoking methods, etc.
    - Support multiple inheritance and hierarchy.

- **Flexible and easy to use**
    - Building meta data doesn't require preprocessor or any external tool. Only use native C++ code, no need macros.
    - Non-intrusive. You don't need to change your code for reflection.
    - Easy to reflect templates. Indeed there are built-in reflections for STL containers such std::vector.
    - Loose coupled design. For example, constructors and member functions can be used without the class information.
    - You don't pay for what you don't use. If you don't build the meta data, no any memory overhead. If you don't use the meta data, no any performance overhead. If you build and use the meta data, you get trivial memory and performance overhead beside very powerful reflection system.
    - Doesn't require C++ RTTI.
    - Written in standard and portable C++, only require C++11, and support later C++ standard.
    - Cross platforms, cross compilers.

- **Language features that can be reflected**
    - Member and static fields with any data type.
    - Global fields (global variable) with any data type.
    - Properties (fields with getter and setter). Can be either member or global.
    - Member and static functions with any parameters and return type.
    - Non-member functions (free functions).
    - Overloaded function.
    - Default arguments of functions.
    - Functions with variadic parameters.
    - Enumerators.
    - Constants in any data type.
    - Classes and nested inner classes.
    - Namespace simulation.
    - Class constructors.
    - Array, multi-dimensional array.

## License

Apache License, Version 2.0  

## Version 0.1 WIP
![CI](https://github.com/wqking/metapp/workflows/CI/badge.svg)

The project is under working in progress.  
Currently the architecture is almost stable, though minor parts may be refactored.  
Most code should work correctly.  

To put the library to first release, we need to,   
1. Add more test.
2. Complete the documentations.

## Source code

[https://github.com/wqking/metapp](https://github.com/wqking/metapp)

## Supported compilers

Tested with MSVC 2022, MinGW (Msys) GCC 8.3, Ubuntu GCC 5.4.
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

## C++ standard requirements
* To Use the library  
    * The library: C++11.  
* To develop the library
    * Unit tests: C++17.
    * Tutorials: C++11.

## Quick start

### Namespace

`metapp`

### Use metapp in your project

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

UFN(FN_PREFIX)
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
UFN(FN_PREFIX)
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
}

//desc ### Call function
UFN(FN_PREFIX)
{
    //code
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
    //code
}

/*desc
Below are tutorials and documents.  
If you want to contribute, be sure to read [How to generate documentations](doc/generate_document.md).  

## Tutorials

- [Use Variant](doc/tutorial/tutorial_variant.md)
- [Use MetaType](doc/tutorial/tutorial_metatype.md)
- [Use MetaClass and meta data for class members](doc/tutorial/tutorial_metaclass.md)
- [Use MetaCallable, function, member function, etc](doc/tutorial/tutorial_callable.md)
- [Use MetaRepo to retrieve meta data at running time](doc/tutorial/tutorial_metarepo.md)
- [Use classes inheritance](doc/tutorial/tutorial_metaclass_inheritance.md)

## Documentations

- Core components, classes, concepts
    - [Core concepts - type kind, meta type, unified type, up type, meta interface](doc/core_concepts.md)
    - [Class Variant reference](doc/variant.md)
    - [Class MetaType reference](doc/metatype.md)

- Build meta data
    - [Reflect meta type at compile time using DeclareMetaType](doc/declaremetatype.md)
    - [Register and use meta type at running time using MetaRepo](doc/metarepo.md)
    - [Registered items (RegisteredCallable, etc)](doc/registered_items.md)

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
    - [MetaMember](doc/interfaces/metamember.md)
    - [User defined meta interface](doc/interfaces/metauser.md)

- Built-in meta types
    - [Overview](doc/metatypes/overview_metatypes.md)
    - [List of all built-in meta types](doc/metatypes/list_all.md)
    - [Constructor](doc/metatypes/constructor.md)
    - [Default arguments function](doc/metatypes/default_args_function.md)
    - [Variadic function](doc/metatypes/variadic_function.md)
    - [Accessor](doc/metatypes/accessor.md)

- Utilities
    - [utility.h](doc/utilities/utility.md)

desc*/
