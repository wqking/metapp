[//]: # (Auto generated file, don't modify this file.)

# metapp -- C++ library for runtime reflection, introspection and meta types
<!--begintoc-->
- [Highlight and unique features](#mdtoc_24681dca)
- [Facts and features](#mdtoc_21927030)
- [Basic information](#mdtoc_abc52c05)
  - [License](#mdtoc_5768f419)
  - [Version 0.1.0 and status](#mdtoc_54c2c4cb)
  - [Source code](#mdtoc_6b8a2c23)
  - [Supported compilers](#mdtoc_215a5bea)
- [Quick start](#mdtoc_ea7b0a9)
  - [Namespace](#mdtoc_33e16b56)
  - [Build and install, use metapp in your project](#mdtoc_7785483e)
- [Example code](#mdtoc_3bb166c4)
  - [Use Variant](#mdtoc_cee017b6)
  - [Use MetaType](#mdtoc_e463fa1a)
  - [Call function and accessible](#mdtoc_eb620ae3)
  - [Runtime generic STL container](#mdtoc_5243b8d0)
  - [Use reference with Variant](#mdtoc_b8048b76)
  - [Reflect a class (declare meta type)](#mdtoc_19cc9779)
- [Documentations](#mdtoc_aa76f386)
- [Build the test code](#mdtoc_460948ff)
- [Known compiler related quirks in MSVC](#mdtoc_2f938cf5)
- [Motivations](#mdtoc_c55531bd)
<!--endtoc-->

metapp is a cross platform C++ library that adds reflection feature to C++.  
metapp is light weight, powerful, and unique.

<a id="mdtoc_24681dca"></a>
## Highlight and unique features

- **Allow to retrieve any C++ type information at runtime, such as primary types, pointer, reference, function, template,
const-volatile qualifiers, and much more.** Can you understand the type `char const *(*(* volatile * (&)[][8])())[]` easily?
metapp can understand it, including every CV, pointer, array, function, etc, in no time!   
- **Allow runtime generic programming.** For example, we can access elements in a container without knowing whether
the container is `std::vector` or `std::deque` or `std::list`, and without knowing whether the value type is `int`,
or `std::string`, or another container.  
- **Very easy to reflect templates.** For example, we only need to reflect `std::vector` or `std::list` once,
then we can get meta information for `std::vector<int>`, `std::vector<std::string>`, or
even `std::vector<std::list<std::vector<std::string> > >`.
- **Mimic C++ reference extensively for better performance.** For example, when getting a property value,
or get an element value from a container, a `metapp::Variant` of reference to the element is returned when possible, the element
value is referenced instead of copied, so the memory and performance cost is kept as minimum as possible.

<a id="mdtoc_21927030"></a>
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

<a id="mdtoc_abc52c05"></a>
## Basic information

<a id="mdtoc_5768f419"></a>
### License

Apache License, Version 2.0  

<a id="mdtoc_54c2c4cb"></a>
### Version 0.1.0 and status
![CI](https://github.com/wqking/metapp/workflows/CI/badge.svg)

The project is under working in progress.  
The first stable release will be v1.0.0. 

To put the library to first release, we need to,   
1. Add more test.
2. Finish the metapp based Lua bind project (under development).
3. Complete the documentations.

You are welcome to try the project and give feedback. Your participation will help to make the development faster and better.

<a id="mdtoc_6b8a2c23"></a>
### Source code

[https://github.com/wqking/metapp](https://github.com/wqking/metapp)

<a id="mdtoc_215a5bea"></a>
### Supported compilers

metapp requires C++ compiler that supports C++11 standard.  
The library is tested with MSVC 2022, 2019, MinGW (Msys) GCC 8.3 and 11.3.0, Clang (carried by MSVC).  
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

<a id="mdtoc_ea7b0a9"></a>
## Quick start

<a id="mdtoc_33e16b56"></a>
### Namespace

`metapp`

<a id="mdtoc_7785483e"></a>
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

<a id="mdtoc_3bb166c4"></a>
## Example code

Here is simple code pieces. There are comprehensive tutorials documentations.

<a id="mdtoc_cee017b6"></a>
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
metapp::Variant v { 5 };
```

Get the value

```c++
ASSERT(v.get<int>() == 5);
metapp::Variant casted = v.cast<double>();
ASSERT(casted.get<double>() == 5.0);
```

Now v contains pointer to char.

```c++
v = "hello";
ASSERT(strcmp(v.get<char *>(), "hello") == 0);
```

Cast to std::string.

```c++
casted = v.cast<std::string>();
```

Get as reference to avoid copy.

```c++
ASSERT(casted.get<const std::string &>() == "hello");
```

<a id="mdtoc_e463fa1a"></a>
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

equivalence

```c++
auto metaType = metapp::getMetaType<const int *>();
ASSERT(metaType->equal(metapp::getMetaType<const int *>()));
// Equivalence checking ignores all CV qualifiers.
ASSERT(metaType->equal(metapp::getMetaType<int *>()));
```

<a id="mdtoc_eb620ae3"></a>
### Call function and accessible
Define a class to be used later.

```c++
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
```

Define a class instance.

```c++
MyClass obj { 5 };
```

Define a callable.

```c++
metapp::Variant funcAdd(&MyClass::add);
```

Invoke the callable, the second argument is the object instance, it's required when invoking member function.

```c++
metapp::Variant resultOfAdd = metapp::callableInvoke(funcAdd, &obj, 3, 9);
ASSERT(resultOfAdd.get<int>() == 17);
```

Another callable, note the instance here is the object reference.
metapp supports instance of pointer, reference, value, or even smart pointers.

```c++
metapp::Variant funcClone(&MyClass::clone);
metapp::Variant cloned = metapp::callableInvoke(funcClone, metapp::Variant::reference(obj));
```

`cloned` holds `std::unique_ptr<MyClass>`.

```c++
ASSERT(cloned.getMetaType()->equal(metapp::getMetaType<std::unique_ptr<MyClass>>()));
```

We can pass the cloned Variant as the instance. This is similar to that
we can call member function using `std::unique_ptr`.

```c++
ASSERT(metapp::callableInvoke(funcAdd, cloned, 5, 9).get<int>() == 19);
```

Define an accessible.

```c++
metapp::Variant accessible(&MyClass::value);
```

Get the value from accessible, which is MyClass::value.
The second argument is the object instance.
Since `cloned` is cloned freshly, both obj and cloned hold the same value.

```c++
ASSERT(metapp::accessibleGet(accessible, &obj).get<int>() == 5);
ASSERT(metapp::accessibleGet(accessible, cloned).get<int>() == 5);
```

Change value in obj

```c++
obj.value = 6;
ASSERT(metapp::accessibleGet(accessible, &obj).get<int>() == 6);
```

`cloned` is not affected.

```c++
ASSERT(metapp::accessibleGet(accessible, cloned).get<int>() == 5);
```

Set value in `cloned`.

```c++
metapp::accessibleSet(accessible, cloned, 38);
```

`obj` is not affected.

```c++
ASSERT(metapp::accessibleGet(accessible, &obj).get<int>() == 6);
ASSERT(metapp::accessibleGet(accessible, cloned).get<int>() == 38);
```

<a id="mdtoc_5243b8d0"></a>
### Runtime generic STL container
Let's define a `concat` function that processes any Variant that implements meta interface MetaIterable

```c++
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
```

A std::vector of int.

```c++
std::vector<int> container1 { 1, 5, 9, 6, 7 };
```

Construct a Variant with the vector. To avoid container1 being coped, we use reference.

```c++
metapp::Variant v1 = metapp::Variant::reference(container1);
```

Concat the items in the vector.

```c++
ASSERT(concat(v1) == "15967");
```

We can also use std::list. Any value can convert to Variant implicitly, so we can pass the container std::list on the fly.

```c++
ASSERT(concat(std::list<std::string>{ "Hello", "World", "Good" }) == "HelloWorldGood");
```

std::tuple is supported too, and we can use heterogeneous types.

```c++
ASSERT(concat(std::make_tuple("A", 1, "B", 2)) == "A1B2");
```

Isn't cool we can use std::pair as a container?

```c++
ASSERT(concat(std::make_pair("Number", 1)) == "Number1");
```

We can even pass a pointer to the container to `concat`.

```c++
std::deque<int> container2 { 1, 2, 3 };
metapp::Variant v2(&container2);
ASSERT(concat(v2) == "123");
```

<a id="mdtoc_b8048b76"></a>
### Use reference with Variant
Declare a value to be referred to.

```c++
int n = 9;
```

rn holds a reference to n.
C++ equivalence is `int & rn = n;`

```c++
metapp::Variant rn = metapp::Variant::reference(n);
ASSERT(rn.get<int>() == 9);
```

Assign to rn with new value. C++ equivalence is `rn = 38;` where rn is `int &`.
We can't write `rn = 38;` where rn is `Variant`, that's different meaning that assign rn with
a Variant of value 38. See Variant document for details.

```c++
rn.assign(38); // different with rn = 38
```

rn gets new value.

```c++
ASSERT(rn.get<int>() == 38);
```

n is modified too.

```c++
ASSERT(n == 38);
```

We can use reference to modify container elements as well.  
vs holds a `std::vector<std::string>`.

```c++
metapp::Variant vs(std::vector<std::string> { "Hello", "world" });
ASSERT(vs.get<const std::vector<std::string> &>()[0] == "Hello");
```

Get the first element. The element is returned as a reference.

```c++
metapp::Variant item = metapp::indexableGet(vs, 0);
```

assign to item with new value.

```c++
item.assign("Good");
ASSERT(vs.get<const std::vector<std::string> &>()[0] == "Good");
```

<a id="mdtoc_19cc9779"></a>
### Reflect a class (declare meta type)
Here is the class we are going to reflect for.

```c++
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
```

Now let's `DeclareMetaType` for MyPet. We `DeclareMetaType` for all kinds of types,
not only classes, but also enumerators, templates, etc.

```c++
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
```

Now let's use the reflected meta class.
Obtain the meta type for MyPet, then get the meta class.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyPet>();
const metapp::MetaClass * metaClass = metaType->getMetaClass();
```

`getConstructor`, then invoke the constructor as if it's a normal callable, with property arguments.
Then obtain the MyPet instance pointer from the returned Variant and store it in a `std::shared_ptr`.

```c++
std::shared_ptr<MyPet> myPet(metapp::callableInvoke(metaClass->getConstructor(), nullptr,
  "Lovely", 3).get<MyPet *>());
// Verify the object is constructed properly.
ASSERT(myPet->name == "Lovely");
ASSERT(myPet->getAge() == 3);
```

Get field by name then get the value.

```c++
const auto & propertyName = metaClass->getAccessible("name");
ASSERT(metapp::accessibleGet(propertyName, myPet).get<const std::string &>() == "Lovely");
const auto & propertyAge = metaClass->getAccessible("age");
ASSERT(metapp::accessibleGet(propertyAge, myPet).get<int>() == 3);
```

Set field `name` with new value.

```c++
metapp::accessibleSet(propertyName, myPet, "Cute");
ASSERT(metapp::accessibleGet(propertyName, myPet).get<const std::string &>() == "Cute");
```

Get member function then invoke it.

```c++
const auto & methodBark = metaClass->getCallable("bark");
ASSERT(metapp::callableInvoke(methodBark, myPet).get<const std::string &>() == "Bow-wow, Cute");

const auto & methodCalculate = metaClass->getCallable("calculate");
// Pass arguments 2 and 3 to `calculate`, the result is 2+3=5.
ASSERT(metapp::callableInvoke(methodCalculate, myPet, 2, 3).get<int>() == 5);
```

<a id="mdtoc_aa76f386"></a>
## Documentations
Below are tutorials and documents. Don't feel upset if you find issues or missing stuff in the documents, I'm not
native English speaker and it's not that exciting to write document. Any way, the code quality is always much better
than the document, for ever.  
If you want to contribute to the documents, be sure to read [How to generate documentations](doc/about_document.md).  

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
  - [MetaStreamable](doc/interfaces/metastreamable.md)
  - [MetaMappable](doc/interfaces/metamappable.md)
  - [MetaPointerWrapper](doc/interfaces/metapointerwrapper.md)
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
  - [TypeList reference](doc/utilities/typelist.md)

- Miscellaneous
  - [About documentations](doc/about_document.md)
  - [Use metapp in dynamic library](doc/dynamic_library.md)
  - [Infrequently Asked Questions](doc/faq.md)


<a id="mdtoc_460948ff"></a>
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

<a id="mdtoc_2f938cf5"></a>
## Known compiler related quirks in MSVC

MSVC 2022 and 2019, can build the CMake generated test projects and the tests run correctly in Debug and RelWithDebugInfo
configurations. But some tests fail in Release mode when incremental linking is disabled.  
Those failed tests should not fail, because they work correct in MSVC debug mode and in GCC/Clang.
Adding /Debug option to linking which generates debug information makes the tests success.  
Without /Debug option, but enabling incremental linking, will cause the tests success too.  
So if metapp shows weird behavior in MSVC, try to enable incremental linking.

<a id="mdtoc_c55531bd"></a>
## Motivations

I (wqking) developed `cpgf` library since more than 12 years ago. `cpgf` works, but it has several serious problems.
The first problem is it was written in C++03, the code is verbose and difficult to write. The second problem is that
it includes too many features in a single library -- reflection, serialization, script binding (Lua, Python, JavaScript).
`cpgf` became unmanageable and can't be developed in an elegant way.  
`metapp` is a modern version of `cpgf`, and it will only include reflection feature. Other features, such as serialization,
script binding, will be in separated projects, if they are developed.

