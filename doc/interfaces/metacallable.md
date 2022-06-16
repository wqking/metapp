[//]: # (Auto generated file, don't modify this file.)

# MetaCallable interface
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Get MetaCallable interface](#mdtoc_8959fb44)
- [Implemented built-in meta types](#mdtoc_ed7f0e2e)
- [MetaCallable constructor](#mdtoc_8b5bcfbf)
- [MetaCallable member functions](#mdtoc_84aa785a)
  - [getClassType](#mdtoc_a80b7ef1)
  - [getParameterCountInfo](#mdtoc_9a56d2a7)
  - [getReturnType](#mdtoc_3d8ebc96)
  - [getParameterType](#mdtoc_60a0b869)
  - [rankInvoke](#mdtoc_a909c1fa)
  - [canInvoke](#mdtoc_fbd8fd0a)
  - [invoke](#mdtoc_e3562fb)
  - [isStatic](#mdtoc_460427c9)
- [ArgumentSpan](#mdtoc_b0381b3f)
- [Non-member utility functions](#mdtoc_e4e47ded)
  - [callableGetClassType](#mdtoc_35629588)
  - [callableGetParameterCountInfo](#mdtoc_a73a92b5)
  - [callableGetReturnType](#mdtoc_14ca1ce5)
  - [callableGetParameterType](#mdtoc_e9a15729)
  - [callableRankInvoke](#mdtoc_3e98474f)
  - [callableCanInvoke](#mdtoc_b887c001)
  - [callableInvoke](#mdtoc_2879261a)
  - [findCallable](#mdtoc_358ac861)
  - [callableIsStatic](#mdtoc_70d046a1)
- [MetaCallable can cast to std::function](#mdtoc_4f2a2173)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaCallable` is a meta interface to invoke meta callable (function, member function, constructor, std::function, etc).  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/interfaces/metacallable.h"
```

<a id="mdtoc_8959fb44"></a>
## Get MetaCallable interface

We can call `MetaType::getMetaCallable()` to get the `MetaCallable` interface.
If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaCallable * metaCallable = metaType->getMetaCallable();
```

<a id="mdtoc_ed7f0e2e"></a>
## Implemented built-in meta types

Constructor (tkConstructor)  
Function (tkFunction)  
Member function pointer, T (C::*)(arguments...) (tkMemberPointer)  
`std::function` (tkStdFunction)  
Default arguments function (tkDefaultArgsFunction)  
Variadic function (tkVariadicFunction)  

<a id="mdtoc_8b5bcfbf"></a>
## MetaCallable constructor

```c++
MetaCallable(
  const MetaType * (*getClassType)(const Variant & callable),
  int (*getParameterCountInfo)(const Variant & callable),
  const MetaType * (*getReturnType)(const Variant & callable),
  const MetaType * (*getParameterType)(const Variant & callable, const int index),
  int (*rankInvoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments),
  bool (*canInvoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments),
  Variant (*invoke)(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

<a id="mdtoc_84aa785a"></a>
## MetaCallable member functions

The first parameter in all of the member functions is `const Variant & callable`.
It's the Variant which meta type implements `MetaCallable`, and hold the proper data such as function pointer.
The member functions operate on the data.  
We can treat `callable` as the C++ object instance which class implements an interface called `MetaCallable`.  
Variant `callable` can be value that implements `MetaCallable`, or reference that refers to value that implements `MetaCallable`.  

<a id="mdtoc_a80b7ef1"></a>
#### getClassType

```c++
const MetaType * getClassType(const Variant & callable);
```

Returns the meta type of the class that the callable belongs to, or to say, the class declares the callable. 
If the function returns meta type of `void` (MetaType::isVoid() is true), the callable doesn't belong to any class,
or the callable is a static member function. When invoking the callable, the `instance` can be nullptr.  
If the function returns non-void meta type, the callable belongs to the class of the meta type.
When invoking the callable, the `instance` must be pointer to a valid object.  

<a id="mdtoc_9a56d2a7"></a>
#### getParameterCountInfo

```c++
MetaCallable::ParameterCountInfo getParameterCountInfo(const Variant & callable);
```

Returns the parameter count.  

ParameterCountInfo class,

```c++
class ParameterCountInfo
{
public:
  ParameterCountInfo();
  ParameterCountInfo(const int resultCount, const int parameterCount);
  ParameterCountInfo(const int resultCount, const int minParameterCount, const int maxParameterCount);
  int getResultCount() const;
  int getMinParameterCount() const;
  int getMaxParameterCount() const;
};
```

`resultCount` is 0 or 1. 0 means there is no return value. 1 means there is one return value.  
`minParameterCount` and `maxParameterCount` define the number of arguments can be used to call the callable.
The calling arguments count must be,  
`minParameterCount <= argument count <= maxParameterCount`.  
For most callables, `minParameterCount` equals to `maxParameterCount`.  
For overloaded function (tkOverloadedFunction), `resultCount` is the maximum result count of the overloaded functions.
'minParameterCount' is the minimum argument count of the overloaded functions.
'maxParameterCount' is the maximum argument count of the overloaded functions.  
For default args function (tkDefaultArgsFunction), 'minParameterCount' is the number of non-default arguments,
`maxParameterCount` is the number of all arguments, including both non-default and default arguments.  
For variadic function (tkVariadicFunction), 'minParameterCount' is 0, `maxParameterCount` is std::numeric_limits<int>::max().  

<a id="mdtoc_3d8ebc96"></a>
#### getReturnType

```c++
const MetaType * getReturnType(const Variant & callable);
```

Returns meta type of the callable return type.  
For constructor (tkConstructor), the return type is pointer to the class.  

<a id="mdtoc_60a0b869"></a>
#### getParameterType

```c++
const MetaType * getParameterType(const Variant & callable, const int index);
```

Returns the meta type of parameter at `index`.  
For variadic function (tkVariadicFunction), the function always returns nullptr.  

<a id="mdtoc_a909c1fa"></a>
#### rankInvoke

```c++
int rankInvoke(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments);
```

Returns the rank value of whether each argument in array `arguments` matches the parameter in the callable.  
The return value is positive number or zero.  
If the return value is 0, then the arguments can't be used to invoke the callable.
Otherwise, the larger the return value, the more matching.  

Parameter `instance` is the object instance if `callable` is a class member function.
`instance` can be value, pointer, or smart pointer.
If `callable` is a member function, and the constness of `instance` can't access `callable`
(for example, `instance` is a pointer to const, while `callable` is a non-const member function),
the rank is 0, value 0 is returned, and the `arguments` are not checked.  
If `callable` is not a member function, `instance` is ignored.  
`instance` can be value, reference, pointer, `std::shared_ptr`, `std::unique_ptr`, etc.  

Parameter `arguments` is an `ArgumentSpan` object. It's used to pass the arguments. For details, please see the section
for `ArgumentSpan` in this document.  

For implementor: we may get the actual pointer from `instance` by using `metapp::getPointer`, get the pointed type
by using `metapp::getPointedType`. See the document for `utility.h` for more details.

<a id="mdtoc_fbd8fd0a"></a>
#### canInvoke

```c++
bool canInvoke(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments);
```

Returns true if the `arguments` can be used to invoke the callable, false if not.  
This is similar to check if the result of `rankInvoke` is larger than 0, but it's more efficient than `rankInvoke`.

<a id="mdtoc_e3562fb"></a>
#### invoke

```c++
Variant invoke(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments);
```

Invokes the callable, returns the result of the callable. If the callable doesn't return any value (the result type is void),
then empty Variant is returned (Variant::isEmpty() is true).  
Parameter `instance` can be value, reference, pointer, `std::shared_ptr`, `std::unique_ptr`, etc.  

<a id="mdtoc_460427c9"></a>
#### isStatic

```c++
bool isStatic(const Variant & callable) const;
```

Returns true if the callable is static or non-member, false if the callable is class member.  
The function is equivalent to `return getClassType(callable)->isVoid();`.  

<a id="mdtoc_b0381b3f"></a>
## ArgumentSpan

`ArgumentSpan` is used to pass arguments to MetaCallable.  
The definition of `ArgumentSpan` is `metapp::span<const Variant>`.  
`metapp::span` is alias of `std::span` which is a feature in C++20.
If the compiler doesn't support C++20, a C++11 equivalent third party library is used (github.com/tcbrindle/span).  
That's to say, `ArgumentSpan` doesn't require C++20, it can be used in C++11.  
In this document, the term `std::span` is exchangeable with `metapp::span`.  

In case you are not familiar with `std::span`, here is a quick summary how to use `ArgumentSpan`.  
`ArgumentSpan` refers to a contiguous sequence of `Variant`, which is the arguments passed to the callable.
The first argument in `ArgumentSpan` is passed as the left-most argument. Below is some sample code,  

Define a callable Variant.

```c++
metapp::Variant myCallable(std::function<std::string (const std::string &, int)>(
  [](const std::string & a, int b) {
    return a + std::to_string(b);
  }
));
```

Get the MetaCallable.

```c++
const metapp::MetaCallable * metaCallable = myCallable.getMetaType()->getMetaCallable();
```

Now let's prepare the ArgumentSpan.  
We can pass std::vector directly.

```c++
std::vector<metapp::Variant> argumentsA { "a", 5 };
ASSERT(metaCallable->invoke(myCallable, nullptr, argumentsA).get<const std::string &>() == "a5");
```

We can use std::array.

```c++
std::array<metapp::Variant, 2> argumentsB { "b", 6 };
ASSERT(metaCallable->invoke(myCallable, nullptr, argumentsB).get<const std::string &>() == "b6");
```

We can use C++ array, argumentsC has known size 2.

```c++
metapp::Variant argumentsC[] { "c", 7 };
ASSERT(metaCallable->invoke(myCallable, nullptr, argumentsC).get<const std::string &>() == "c7");
```

We can use C++ array that doesn't have known size, then we need to pass the size explicitly.

```c++
metapp::Variant temp[] { "d", 8 };
// We use a lambda to enable the unknown size array.
auto simulateUnknowSizeArray = [&myCallable, &metaCallable](metapp::Variant argumentsD[]) {
  // Won't compile, argumentsD has unknown size.
  //ASSERT(metaCallable->invoke(myCallable, nullptr, argumentsD).get<const std::string &>() == "d8");
  // Compiles, we pass the size explicitly.
  ASSERT(metaCallable->invoke(myCallable, nullptr, { argumentsD, 2 }).get<const std::string &>() == "d8");
};
simulateUnknowSizeArray(temp);
```

Here is how to use metapp::ArgumentSpan if we want to implement MetaCallable or variadic function.  
Assume we implement below function.

```c++
void demoArgumentSpan(const metapp::ArgumentSpan & arguments)
{
  // for..loop
  for(const metapp::Variant & argument : arguments) {
    // use argument here
  }

  // Get size and use operator[] to access the elements.
  const std::size_t count = arguments.size();
  for(std::size_t i = 0; i < count; ++i) {
    const metapp::Variant & argument = arguments[i];
    // use argument here
  }
}
```

<a id="mdtoc_e4e47ded"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaCallable`.  
Usually you should prefer the utility functions to calling `MetaCallable` member function directly.
However, if you need to call functions on a single `MetaCallable` more than one times in a high performance
application, you may store `callable.getMetaType()->getMetaCallable()` to a local variable, then use the variable
to call the member functions.
This is because `getMetaCallable()` has slightly performance overhead (the overhead is neglect most time).

<a id="mdtoc_35629588"></a>
#### callableGetClassType

```c++
inline const MetaType * callableGetClassType(const Variant & callable)
{
  return callable.getMetaType()->getMetaCallable()->getClassType(callable);
}
```

Shortcut for `MetaCallable::getClassType()`.

<a id="mdtoc_a73a92b5"></a>
#### callableGetParameterCountInfo

```c++
inline int callableGetParameterCountInfo(const Variant & callable)
{
  return callable.getMetaType()->getMetaCallable()->getParameterCountInfo(callable);
}
```

Shortcut for `MetaCallable::getParameterCountInfo()`.

<a id="mdtoc_14ca1ce5"></a>
#### callableGetReturnType

```c++
inline const MetaType * callableGetReturnType(const Variant & callable)
{
  return callable.getMetaType()->getMetaCallable()->getReturnType(callable);
}
```

Shortcut for `MetaCallable::getReturnType()`.

<a id="mdtoc_e9a15729"></a>
#### callableGetParameterType

```c++
inline const MetaType * callableGetParameterType(const Variant & callable, const int index)
{
  return callable.getMetaType()->getMetaCallable()->getParameterType(callable, index);
}
```

Shortcut for `MetaCallable::getParameterType()`.

<a id="mdtoc_3e98474f"></a>
#### callableRankInvoke

```c++
template <typename ...Args>
int callableRankInvoke(const Variant & callable, const Variant & instance, Args && ...args);
```

Converts `args` to Variant array then calls `MetaCallable::rankInvoke()` and returns the result.

<a id="mdtoc_b887c001"></a>
#### callableCanInvoke

```c++
template <typename ...Args>
bool callableCanInvoke(const Variant & callable, const Variant & instance, Args && ...args);
```

Converts `args` to Variant array then calls `MetaCallable::canInvoke()` and returns the result.

<a id="mdtoc_2879261a"></a>
#### callableInvoke

```c++
template <typename ...Args>
Variant callableInvoke(const Variant & callable, const Variant & instance, Args && ...args);
```

Converts `args` to Variant array then calls `MetaCallable::invoke()` and returns the result.  

Note: the converted Variant holds reference to each `args`. When invoking the underlying C++ function held by `callable`,
the Variant will be converted to the type of target argument type. If the argument type is `Variant`, the Variant
will passed to the argument directly. The argument will be a Variant that holds a reference to `args`, which may be on
the stack. If the callable stores the Variant for later use, the reference will become dangling address and cause crash.  
In such case, you should construct the Variant arguments explicitly instead of relying on the template deduction.  
Here is the example code for the potential issue,

```c++
metapp::Variant storedVar;
metapp::Variant callable(std::function<void (const metapp::Variant &)>([&storedVar](const metapp::Variant & var) {
  ASSERT(var.getMetaType()->isReference());
  ASSERT(var.get<const std::string &>() == "good");
  // Wrong! storedVar will container invalid address
  storedVar = var;
  ASSERT(storedVar.get<const std::string &>() == "good");
  // After the function returns, storedVar may refer to invalid address
}));
metapp::callableInvoke(callable, nullptr, std::string("good"));
// Below line won't work, it either assert fail or crash.
// ASSERT(storedVar.get<const std::string &>() == "good");
```

<a id="mdtoc_358ac861"></a>
#### findCallable

```c++
template <typename Iterator>
Iterator findCallable(
  Iterator first,
  Iterator last,
  const Variant & instance,
  const ArgumentSpan & arguments,
  int * resultMaxRank = nullptr
);
```

Returns an iterator to the element that's best matched to `arguments` in the range [first, last).  
If no matched callable, `last` is returned.  
`Iterator` must be the iterator to `Variant`, `MetaItem`, or `MetaItem`.

<a id="mdtoc_70d046a1"></a>
#### callableIsStatic

```c++
inline bool callableIsStatic(const Variant & callable)
{
  return callable.getMetaType()->getMetaCallable()->isStatic(callable);
}
```

Shortcut for `MetaCallable::isStatic()`.

<a id="mdtoc_4f2a2173"></a>
## MetaCallable can cast to std::function

Any `MetaCallable` can cast to `std::function` as long as the number of parameter is appropriate. The casting only checks
parameter count, it doesn't check argument type or the result type. The user needs to ensure the argument type is cast-able.   
For example,  

```c++
std::string f1(const int a, const std::string & b)
{
  return std::to_string(a) + b;
}
```

```c++
metapp::Variant v(&f1);
// The parameters type are not exactly, but they are convertible, double can convert to int
using FT = std::function<std::string (double, std::string)>;
FT func = v.cast<FT>().get<FT &>();
ASSERT(func(5.3, "Hello") == "5Hello");
```
