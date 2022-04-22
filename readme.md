# metapp -- C++ library for runtime reflection and meta types

metapp is a cross platform C++ library that adds powerful reflection feature to C++.  

## Highlight features

- Allow to retrieve any C++ type information at runtime, such as primary types, pointer, reference, function, template, const-volatile qualifiers, and much more. Can you understand the type `char *(*(* * [][8])())[]` in 20 seconds? metapp can understand it in no time!   
- Allow runtime generic programming. For example, we can access elements in a container, without knowing whether the container is `std::vector` or `std::deque` or `std::list`, and without knowing whether the value type is `int`, or `std::string`, or another container.  
- Meta type information can be obtained either at compile time, or at running time.  

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

### Variant

```c++
// v contains int
metapp::Variant v(5);
assert(v.get<int>() == 5);

// now v contains std::string
v = std::string("hello");
assert(v.get<std::string>() == "hello");
// get as reference to avoid copy
assert(v.get<const std::string &>() == "hello");
v.get<std::string &>() = "world";
assert(v.get<const std::string &>() == "world");

// cast to const char *
const char * s = v.cast<const char *>().get<const char *>();
assert(strcmp(s, "world") == 0);

// now v contains char *
v = "great";
assert(strcmp(v.get<const char *>(), "great") == 0);

int array[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
// now v contains reference to int[2][3]
v = metapp::Variant::create<int (&)[2][3]>(array);
assert(v.get<int (&)[2][3]>()[1][2] == 6);
array[1][2] = 10;
// since v is a reference to array, modify array will also modify v
assert(v.get<int (&)[2][3]>()[1][2] == 10);

int anotherArray[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
// Now we copy array into v
v = metapp::Variant::create<int [2][3]>(anotherArray);
assert(v.get<int (&)[2][3]>()[1][2] == 6);
anotherArray[1][2] = 10;
// since v is a copy of anotherArray, modify anotherArray will not affect v
assert(v.get<int (&)[2][3]>()[1][2] == 6);
```

### Invoke function

Let's see how to invoke free function.
func1 is the function we are going to invoke.

```c++
 std::string func1(const int n)
{
	return std::to_string(n);
}
```

Now invoke func1 via Variant

```c++
// v is pointer to func1
metapp::Variant v(&func1);

// Prepare the arguments array
metapp::Variant arguments[] { 5 };
// Invoke the callable, the nullptr is the object instance, for free function, it's nullptr
metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 1);
ASSERT(result.get<std::string>() == "5");

// Or we can use metapp::callableInvoke to pass the arguments directly
result = metapp::callableInvoke(v, nullptr, 38);
ASSERT(result.get<std::string>() == "38");
```

## Documentations

- Tutorials
    - [MetaClass](doc/tutorial_metaclass.md)

- Core components, classes, concepts
    - [Core concepts - type kind, meta type, unified type, up type, meta interface](doc/core-concepts.md)
    - [Class Variant reference](doc/variant.md)
    - [Class MetaType reference](doc/metatype.md)
    - [List of all built-in meta types](doc/built-in-meta-types.md)

- Build meta data
    - [Reflect meta type at compile time using DeclareMetaType](doc/declaremetatype.md)
    - [Register meta type at running time using MetaRepo](doc/metarepo.md)

- Meta interfaces
    - [Overview and implement meta interface](doc/meta-interface-overview.md)
    - [MetaClass](doc/metaclass.md)
    - [MetaIndexable](doc/metaindexable.md)
