[//]: # (Auto generated file, don't modify this file.)

# MetaCallable interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaCallable interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaCallable constructor](#a2_5)
* [MetaCallable member functions](#a2_6)
  * [getParameterCount](#a4_1)
  * [getReturnType](#a4_2)
  * [getParameterType](#a4_3)
  * [rankInvoke](#a4_4)
  * [canInvoke](#a4_5)
  * [invoke](#a4_6)
* [Non-member utility functions](#a2_7)
  * [callableGetParameterCount](#a4_7)
  * [callableGetReturnType](#a4_8)
  * [callableGetParameterType](#a4_9)
  * [callableRankInvoke](#a4_10)
  * [callableCanInvoke](#a4_11)
  * [callableInvoke](#a4_12)
  * [callableInvoke on container](#a4_13)
  * [findCallable](#a4_14)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaCallable` is a meta interface to invoke meta callable (function, member function, constructor, std::function, etc).  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metacallable.h"
```

<a id="a2_3"></a>
## Get MetaCallable interface

We can call `MetaType::getMetaCallable()` to get the `MetaCallable` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaCallable * metaCallable = metaType->getMetaCallable();
```

<a id="a2_4"></a>
## Implemented built-in meta types

Constructor (tkConstructor)  
Function (tkFunction)  
Member function pointer, T (C::*)(arguments...) (tkMemberPointer)  
`std::function` (tkStdFunction)  
Default arguments function (tkDefaultArgsFunction)  
Variadic function (tkVariadicFunction)  

<a id="a2_5"></a>
## MetaCallable constructor

```c++
MetaCallable(
  size_t (*getParameterCount)(const Variant & var),
  const MetaType * (*getReturnType)(const Variant & var),
  const MetaType * (*getParameterType)(const Variant & var, const size_t index),
  unsigned int (*rankInvoke)(const Variant & var, const Variant * arguments, const size_t argumentCount),
  bool (*canInvoke)(const Variant & var, const Variant * arguments, const size_t argumentCount),
  Variant (*invoke)(const Variant & var, void * instance, const Variant * arguments, const size_t argumentCount)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

<a id="a2_6"></a>
## MetaCallable member functions

The first parameter in all of the member functions is `const Variant & var`. It's the Variant which meta type implements `MetaCallable`, and hold the proper data such as function pointer. The member functions operate on the data.  
We can treat `var` as the C++ object instance which class implements an interface called `MetaCallable`.  
`var` can be a value, a reference, or a pointer.  

<a id="a4_1"></a>
#### getParameterCount

```c++
size_t getParameterCount(const Variant & var);
```

Returns the parameter count.  
For variadic function (tkVariadicFunction), returns 0.  

<a id="a4_2"></a>
#### getReturnType

```c++
const MetaType * getReturnType(const Variant & var);
```

Returns meta type of the callable return type.  
For constructor (tkConstructor), the return type is pointer to the class.  

<a id="a4_3"></a>
#### getParameterType

```c++
const MetaType * getParameterType(const Variant & var, const size_t index);
```

Returns the meta type of parameter at `index`.  
For variadic function (tkVariadicFunction), the function always returns nullptr.  

<a id="a4_4"></a>
#### rankInvoke

```c++
unsigned int rankInvoke(const Variant & var, const Variant * arguments, const size_t argumentCount);
```

Returns the rank value of whether each argument in array `arguments` matches the parameter in the callable.  
If the return value is 0, then the arguments can't be used to invoke the callable. Otherwise, the larger the return value, the more matching.  

Parameter `arguments` is a pointer to an array of Variant, each element is an argument. The first element is the left-most argument in the callable.  
Parameter `argumentCount` is the number of argument in `arguments`.  

<a id="a4_5"></a>
#### canInvoke

```c++
bool canInvoke(const Variant & var, const Variant * arguments, const size_t argumentCount);
```

Returns true if the `arguments` can be used to invoke the callable, false if not.  
This is similar to check if the result of `rankInvoke` is larger than 0, but it's more efficient than `rankInvoke`.

<a id="a4_6"></a>
#### invoke

```c++
Variant invoke(const Variant & var, void * instance, const Variant * arguments, const size_t argumentCount);
```

Invokes the callable, returns the result of the callable. If the callable doesn't return any value (the result type is void), then empty Variant is returned (Variant::isEmpty() is true).  

<a id="a2_7"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaCallable`.  
Usually you should prefer the utility functions to calling `MetaCallable` member function directly. However, if you need to call functions on a single `MetaCallable` more than one times in a high performance application, you may store `var.getMetaType()->getMetaCallable()` to a local variable, then use the variable to call the member functions. This is because `getMetaCallable()` has slightly performance overhead (the overhead is neglect most time).

<a id="a4_7"></a>
#### callableGetParameterCount

```c++
inline size_t callableGetParameterCount(const Variant & var)
{
  return var.getMetaType()->getMetaCallable()->getParameterCount(var);
}
```

Shortcut for `MetaCallable::getParameterCount()`.

<a id="a4_8"></a>
#### callableGetReturnType

```c++
inline const MetaType * callableGetReturnType(const Variant & var)
{
  return var.getMetaType()->getMetaCallable()->getReturnType(var);
}
```

Shortcut for `MetaCallable::getReturnType()`.

<a id="a4_9"></a>
#### callableGetParameterType

```c++
inline const MetaType * callableGetParameterType(const Variant & var, const size_t index)
{
  return var.getMetaType()->getMetaCallable()->getParameterType(var, index);
}
```

Shortcut for `MetaCallable::getParameterType()`.

<a id="a4_10"></a>
#### callableRankInvoke

```c++
template <typename ...Args>
unsigned int callableRankInvoke(const Variant & var, Args ...args);
```

Converts `args` to Variant array then calls `MetaCallable::rankInvoke()` and returns the result.

<a id="a4_11"></a>
#### callableCanInvoke

```c++
template <typename ...Args>
bool callableCanInvoke(const Variant & var, Args ...args);
```

Converts `args` to Variant array then calls `MetaCallable::canInvoke()` and returns the result.

<a id="a4_12"></a>
#### callableInvoke

```c++
template <typename ...Args>
Variant callableInvoke(const Variant & var, void * instance, Args ...args);
```

Converts `args` to Variant array then calls `MetaCallable::invoke()` and returns the result.

<a id="a4_13"></a>
#### callableInvoke on container

```c++
template <template <typename, typename> class Container, typename T, typename Allocator, typename ...Args>
Variant callableInvoke(const Container<T, Allocator> & callableList, void * instance, Args ...args);
```

Finds a best matched callable in `callableList`, then invokes the matched callable and returns the result.  
If no matched callable, exception `metapp::IllegalArgumentException` is thrown.  
`callableList` can be STL iterable containers, such as `std::vector`, `std::deque`, or `std::list`. The value type T can be `metapp::Variant` that holds a callable.  
`callableList` can also be `MetaItemList` or `MetaItemList` obtained from MetaClass or MetaRepo.

<a id="a4_14"></a>
#### findCallable

```c++
template <typename Iterator>
Iterator findCallable(
  Iterator first,
  Iterator last,
  const Variant * arguments,
  const size_t argumentCount
);
```

Returns an iterator to the element that's best matched to `arguments` in the range [first, last).  
If no matched callable, `last` is returned.  
`Iterator` must be the iterator to `Variant`, `MetaItem`, or `MetaItem`.
