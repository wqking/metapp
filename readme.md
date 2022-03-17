# metapp -- C++ library for runtime reflection and meta types

metapp is a cross platform C++ library that adds powerful reflection feature to C++.

## Facts and features

- **Powerful**
    - Supports any C++ types, such as primary, pointer, reference, function, template, const-volatile qualifiers, and much more.
    - True runtime reflection. Accessing fields and properties, calling methods, are truly runtime behavior, no template parameters are needed. All parameters and return values are passed via metapp::Variant.
    - Automatically type conversion when getting/setting fields, invoking methods, etc.
    - Support multiple inheritance and hierarchy.

- **Flexible and easy to use**
    - Building meta data doesn't require preprocessor or any external tool. Only use native C++ code, no need macros.
    - Non-intrusive. You don't need to change your code for reflection.
    - Loose coupled design. For example, constructors and member functions can be used without the class information.
    - You don't pay for what you don't use. If you don't build the meta data, no any memory overhead. If you don't use the meta data, no any performance overhead. If you build and use the meta data, you get trivial memory and performance overhead beside very powerful reflection system.
    - Doesn't require C++ RTTI.
    - Written in standard and portable C++, only require C++11, and support later C++ standard.

- **Language features that can be reflected**
    - Member and static fields with any data type.
    - Global fields (global variable) with any data type.
    - Properties (fields with getter and setter). Can be either member or global.
    - Member and static functions with any parameters and return type.
    - Global functions (free functions).
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

## Version 0.1 RC
![CI](https://github.com/wqking/metapp/workflows/CI/badge.svg)

This is the first release candidate version.   
metapp needs more tests.  

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

## Documentations

Meta type system
