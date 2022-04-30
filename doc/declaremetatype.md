[//]: # (Auto generated file, don't modify this file.)

# Use DeclareMetaType to reflect meta types at compile time
<!--begintoc-->
* [Overview](#a2_1)
* [Members in DeclareMetaType](#a2_2)
  * [Constant typeKind](#a4_1)
  * [Function cast](#a4_2)
  * [castFrom](#a4_3)
  * [Function setup](#a4_4)
* [Members in DeclareMetaType, usually don't need to be re-implemented](#a2_3)
  * [Type UpType](#a4_5)
* [Implement `cast` and `castFrom`](#a2_4)
  * [Method 1, use `CastToTypes` and `CastFromTypes`](#a3_1)
  * [Method 2, implement `cast` and `castFrom` manually](#a3_2)
  * [Test code](#a3_3)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

Even though metapp works on any C++ type that are not known to metapp, it's useful to provide metapp more information on a certain type. The template `DeclareMetaType` is used for providing such information.  
To provide more information of a type to metapp, specialize template `metapp::DeclareMetaType` with the type. The specialization must be in the global namespace. The specialization should inherit from `metapp::DeclareMetaTypeBase`.   
Only the raw type should be specialized. That's to say, the type should not contain any const-volatile qualifiers, reference, pointer, array, or any containers. metapp will handle all the other cases for you. For example, if the raw type is T, metapp will handle `const T`, `T &`, `const T *`, `std::vector<T>`, etc.  

Function `metapp::getMetaType` gets information declared by `metapp::DeclareMetaType` as non-template class `metapp::MetaType`.  

For example,  

```c++
// Assume the type is MyClass
class MyClass {};
```

```c++
template <>
struct metapp::DeclareMetaType <MyClass>
  : metapp::DeclareMetaTypeBase <MyClass>
{
};
```

The members in DeclareMetaType that will be used by metapp are static constants, type definition, or static functions. All members are optional. If any member is missed, metapp will use default implementation.  

<a id="a2_2"></a>
## Members in DeclareMetaType

<a id="a4_1"></a>
#### Constant typeKind

```c++
template <>
struct metapp::DeclareMetaType <MyClass>
  : metapp::DeclareMetaTypeBase <MyClass>
{
  static constexpr metapp::TypeKind typeKind = metapp::tkUser + 1;
};
```

The default value of `typeKind` is `metapp::tkObject`.  

Each `MetaType` always has one `TypeKind`. TypeKind is a 16 bit integer that represents the meta type. For any user defined TypeKind, the value must be equal or greater than `metapp::tkUser`.  

<a id="a4_2"></a>
#### Function cast

```c++
static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType);
```

Function `cast` return true if the `value` can be type casted to `toMetaType`, otherwise returns false.  
If `result` is not nullptr and if the `value` can be casted, set `result` with the casted value.  
If the implementation can't cast the value, it should return the value of default implementation, `metapp::commonCast`.  

```c++
template <>
struct metapp::DeclareMetaType <MyClass>
  : metapp::DeclareMetaTypeBase <MyClass>
{
  static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType)
  {
    if(can cast MyClass to toMetaType) {
      *result = casted value;
      return true;
    }
    return metapp::commonCast(result, value, getMetaType<MyClass>(), toMetaType);
  }
};
```

<a id="a4_3"></a>
#### castFrom

```c++
static bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType);
```

Function `castFrom` return true if the `value` of `fromMetaType` can be casted to the type of which is under DeclareMetaType, otherwise returns false.  
If `result` is not nullptr and if the `value` can be casted, set `result` with the casted value.  

<a id="a4_4"></a>
#### Function setup

```c++
static void setup();
```

Function `setup` is invoked on the first time when the MetaType is initialized. It will be called only once for one MetaType.  
`setup` is useful when the `DeclareMetaType` needs to do initialize work. One use case is to register inheritance relationship.  

**Example** 

```c++
struct A {};
struct B {};
struct C : A, B {};

template <>
struct metapp::DeclareMetaType <C>
  : metapp::DeclareMetaTypeBase <C>
{
  static void setup() {
    metapp::getMetaRepo()->registerBase<C, A, B>();
  }
};
```

<a id="a2_3"></a>
## Members in DeclareMetaType, usually don't need to be re-implemented

<a id="a4_5"></a>
#### Type UpType

```c++
template <>
struct metapp::DeclareMetaType <MyClass>
  : metapp::DeclareMetaTypeBase <MyClass>
{
  using UpType = TheUpTypeOfMyClass;
};
```


<a id="a2_4"></a>
## Implement `cast` and `castFrom`

There are various ways to implement `cast` and `castFrom`. Assume we have the classes,  

```c++
struct Text
{
  std::string s;
};

struct Integer
{
  int i;

  explicit Integer(const int n) : i(n) {}

  Integer(const Text & text)
    : i(std::stoi(text.s))
  {
  }
  
  operator Text () const {
    return Text{std::to_string(i)};
  }
};
```

So `Integer` can "cast" to `Text`, and `Integer` can also "castFrom" `Text`.  
Now let's implement the `cast` and `castFrom` in DeclareMetaType.  

<a id="a3_1"></a>
### Method 1, use `CastToTypes` and `CastFromTypes`

```c++
// Include cast.h to use CastToTypes and CastFromTypes
#include "metapp/cast.h"

template <>
struct metapp::DeclareMetaType<Integer>
  : metapp::CastToTypes<Integer, metapp::TypeList<Text, double> >, // double is an example, see below
    metapp::CastFromTypes<Integer, metapp::TypeList<Text> >
{
};
```

That's all. `CastToTypes` and `CastFromTypes` will implement `cast` and `castFrom`.  
The first template parameter in `CastToTypes` and `CastFromTypes` is the type that is under DeclareMetaType,
so it's the same parameter as metapp::DeclareMetaType.  
The second template parameter is a `metapp::TypeList`. The template parameters in `metapp::TypeList` are a list of types.
`metapp::CastToTypes` will check if the first parameter (here is `Text`) can cast to any type in the `TypeList`,
and implement `cast` for the cast-able types. If `Text` can't cast to any type in the list, such as the `double` in the example code,
it ignores the type and it doesn't cause compile error.  
`metapp::CastFromTypes` is similar. It checks if the first parameter can cast from any type in the `TypeList`,
and ignores any types that is not castFrom-able.  
If `Text` can cast to and cast from the same types, we can use `CastFromToTypes` to make the code shorter, for exmaple,  

```c++
template <>
struct metapp::DeclareMetaType<Text>
  : metapp::CastFromToTypes<Text, metapp::TypeList<Integer> >
{
};
```

<a id="a3_2"></a>
### Method 2, implement `cast` and `castFrom` manually

`CastToTypes`, `CastFromTypes` and `CastFromToTypes` is enough in most time,
but some time it makes sense to implement `cast` and `castFrom` manually.  
Here is example code,  

```c++
// Include cast.h to use CastToTypes and CastFromTypes
#include "metapp/cast.h"

template <>
struct metapp::DeclareMetaType<Integer>
{
  static bool cast(metapp::Variant * result, const metapp::Variant & value, const metapp::MetaType * toMetaType) {
    // Check if toMetaType is Text
    if(toMetaType->getUnifiedType() == metapp::getMetaType<Text>()->getUnifiedType()) {
      if(result != nullptr) {
        *result = static_cast<Text>(value.get<const Integer &>());
      }
      return true;
    }
    // Note it's getMetaType<Integer>(), it's the meta type under DeclareMetaType
    return commonCast(result, value, getMetaType<Integer>(), toMetaType);
  }

  static bool castFrom(metapp::Variant * result, const metapp::Variant & value, const metapp::MetaType * fromMetaType)
  {
    if(fromMetaType->getUnifiedType() == metapp::getMetaType<Text>()->getUnifiedType()) {
      if(result != nullptr) {
        *result = static_cast<Integer>(value.get<const Text &>());
      }
      return true;
    }
    return false;
  }
};
```

<a id="a3_3"></a>
### Test code

No matter which method we use, we can test as,

Cast Integer to Text.

```c++
metapp::Variant v(Integer{ 3 });
metapp::Variant casted = v.cast<Text>();
ASSERT(casted.get<const Text &>().s == "3");
```

Cast Integer from Text.

```c++
metapp::Variant v(Text{ "5" });
metapp::Variant casted = v.cast<Integer>();
ASSERT(casted.get<const Integer &>().i == 5);
```

We can also cast Integer to const Text &. Even though we only implement `cast` for `Text`,
metapp will handle the reference properly.

```c++
metapp::Variant v(Integer{ 3 });
metapp::Variant casted = v.cast<const Text &>();
ASSERT(casted.get<const Text &>().s == "3");
```