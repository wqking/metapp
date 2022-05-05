[//]: # (Auto generated file, don't modify this file.)

# Registered items
<!--begintoc-->
* [Overview](#a2_1)
  * [getName](#a4_1)
  * [getTarget](#a4_2)
  * [Implicitly type convert](#a4_3)
  * [isEmpty](#a4_4)
* [Class MetaItem](#a2_2)
  * [Header](#a4_5)
  * [API](#a4_6)
* [Class MetaItem](#a2_3)
  * [Header](#a4_7)
  * [API](#a4_8)
* [Class MetaItem](#a2_4)
  * [Header](#a4_9)
  * [API](#a4_10)
* [Class MetaItem](#a2_5)
  * [Header](#a4_11)
  * [API](#a4_12)
* [Class MetaItem](#a2_6)
  * [Header](#a4_13)
  * [API](#a4_14)
* [Class MetaItem](#a2_7)
  * [Header](#a4_15)
  * [API](#a4_16)
* [Base class RegisteredAnnotation](#a2_8)
  * [API](#a4_17)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

When getting meta information from MetaRepo such as calling `MetaRep::getCallable()`, the returned value is a registered item, such as `MetaItem`, or a `std::deque` of the registered items, such as `MetaItemList`.  

The registered items allow to,  
1. Get the meta data name.
2. Get the underlying target (for `MetaItem`, the target is the callable Variant).
3. Register and retrieve annotations.

All registered items inherit from `RegisteredAnnotation`. `RegisteredAnnotation` allow to register and retrieve annotations.  

All registered items have common member functions,  

<a id="a4_1"></a>
#### getName

```c++
const std::string & getName() const
```

Returns the name. If the item doesn't have name, returns empty string.  
`MetaItem` doesn't have the `getName` function.  

<a id="a4_2"></a>
#### getTarget

```c++
TargetType getTarget() const;
```

Returns the underlying data. `TargetType` is a placeholder here, it's different in different classes. For example, `TargetType` is `const Variant &` in `MetaItem`, which is the `callable`.  

<a id="a4_3"></a>
#### Implicitly type convert

```c++
operator TargetType () const
```

Implicitly casts the item to `TargetType`.  

<a id="a4_4"></a>
#### isEmpty

```c++
bool isEmpty() const;
```

Returns true if the item is empty, that means the item doesn't contain any valid data.  
`MetaRepo` may return empty item if the meta data is not found in the repo.  

<a id="a2_2"></a>
## Class MetaItem

<a id="a4_5"></a>
#### Header

```c++
#include "metapp/registration/registeredaccessible.h"
```

<a id="a4_6"></a>
#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
  const std::string & getName() const;
  const Variant & getTarget() const;
  bool isEmpty() const;
  operator const Variant & () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the accessible Variant.

<a id="a2_3"></a>
## Class MetaItem

<a id="a4_7"></a>
#### Header

```c++
#include "metapp/registration/registeredcallable.h"
```

<a id="a4_8"></a>
#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
  const std::string & getName() const;
  const Variant & getTarget() const;
  bool isEmpty() const;
  operator const Variant & () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the callable Variant.

<a id="a2_4"></a>
## Class MetaItem

<a id="a4_9"></a>
#### Header

```c++
#include "metapp/registration/registeredconstructor.h"
```

<a id="a4_10"></a>
#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
  const Variant & getTarget() const;
  bool isEmpty() const;
  operator const Variant & () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the callable (tkConstructor) Variant.  
Note: `MetaItem` doesn't have `getName()` function.  

<a id="a2_5"></a>
## Class MetaItem

<a id="a4_11"></a>
#### Header

```c++
#include "metapp/registration/registeredrepo.h"
```

<a id="a4_12"></a>
#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
  const std::string & getName() const;
  MetaRepo * getTarget() const;
  bool isEmpty() const;
  operator MetaRepo * () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the pointer to registered `MetaRepo`.  

<a id="a2_6"></a>
## Class MetaItem

<a id="a4_13"></a>
#### Header

```c++
#include "metapp/registration/registeredtype.h"
```

<a id="a4_14"></a>
#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
  const std::string & getName() const;
  const MetaType * getTarget() const;
  bool isEmpty() const;
  operator const MetaType * () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the pointer to registered `MetaType`.  

<a id="a2_7"></a>
## Class MetaItem

<a id="a4_15"></a>
#### Header

```c++
#include "metapp/registration/registeredenumvalue.h"
```

<a id="a4_16"></a>
#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
  using ValueType = long long;

  const std::string & getName() const;
  ValueType getTarget() const;
  bool isEmpty() const;
  operator ValueType () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the pointer to registered enum value as `ValueType (long long)`.  
`MetaItem` is not returned by `MetaRepo`, it's returned by `MetaEnum`.  

<a id="a2_8"></a>
## Base class RegisteredAnnotation

`RegisteredAnnotation` is the base class of all registered items. Thant means when you get a registered item, you get a `RegisteredAnnotation`.  
`RegisteredAnnotation` allows to register and retrieve annotations. Annotation is the extra information bound to a meta data. An annotation is a name/value pair. Name is `std::string`, value is `metapp::Variant`. That means the value can be any data.  

<a id="a4_17"></a>
#### API

```c++
class RegisteredAnnotation
{
public:
  void registerAnnotation(const std::string & name, const Variant & value);
  const Variant & getAnnotation(const std::string & name) const;
  const std::map<std::string, Variant> & getAllAnnotations() const;
};
```

For `getAnnotation`, if the annotation of `name` is not found, an empty Variant is returned (`Variant::isEmpty() is true`).
