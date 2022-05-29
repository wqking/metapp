[//]: # (Auto generated file, don't modify this file.)

# MetaItem
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [MetaItem member functions](#a2_3)
  * [getType](#a4_1)
  * [isEmpty](#a4_2)
  * [getName](#a4_3)
  * [asXxx functions](#a4_4)
  * [Implicitly type convert](#a4_5)
  * [registerAnnotation](#a4_6)
  * [getAnnotation](#a4_7)
  * [getAllAnnotations](#a4_8)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaItem` represents the meta data registered in `MetaRepo` and `MetaClass`.  
When registering any meta data in `MetaRepo` and `MetaClass`, the register function returns a `MetaItem`.  
When retrieving any meta data from `MetaRepo` and `MetaClass`, the returned data is either `MetaItem` or `MetaItemView`.  

<a id="a2_2"></a>
## Header


```c++
#include "metapp/metaitem.h"
```

<a id="a2_3"></a>
## MetaItem member functions

<a id="a4_1"></a>
#### getType

```c++
MetaItem::Type getType() const;
```

Returns the type of the `MetaItem`. Each `MetaItem` has one type.  
`MetaItem::Type` is a `enum class`.

```c++
enum class MetaItem::Type {
  none,
  accessible,
  callable,
  constant,
  constructor,
  metaRepo,
  metaType,
  enumValue
};
```

The values,  

**MetaItem::Type::none**  
The item doesn't have any valid type. `isEmpty()` will return true.  
Don't check this type, use `isEmpty()` instead.

**MetaItem::Type::accessible**  
The item is an accessible that implements MetaAccessible meta interface.  

**MetaItem::Type::callable**  
The item is an callable that implements MetaCallable meta interface.  

**MetaItem::Type::constant**  
The item is an constant.  

**MetaItem::Type::constructor**  
The item is a constructor.

**MetaItem::Type::metaRepo**  
The item is a MetaRepo.  

**MetaItem::Type::metaType**  
The item is a MetaType.  

**MetaItem::Type::enumValue**  
The item is a value in `MetaEnum`.  

<a id="a4_2"></a>
#### isEmpty

```c++
bool isEmpty() const;
```

Returns true if the item doesn't hold any valid data.  

<a id="a4_3"></a>
#### getName

```c++
const std::string & getName() const
```

Returns the name. If the item doesn't have name, returns empty string.  
`MetaItem::Type::constructor` always has empty name.  

<a id="a4_4"></a>
#### asXxx functions

```c++
const Variant & asAccessible() const;
const Variant & asCallable() const;
const Variant & asConstant() const;
const Variant & asConstructor() const;
const MetaType * asMetaType() const;
const MetaRepo * asMetaRepo() const;
const Variant & asEnumValue() const;
```

Returns the proper data.  
If the type doesn't match the function, exception `metapp::IllegalArgumentException` is raised.  

<a id="a4_5"></a>
#### Implicitly type convert

```c++
operator const Variant & () const;
```

Convert the item to Variant. This is useful to pass the meta item where a Variant is required. For example,  

```c++
metapp::MetaRepo metaRepo;
metaRepo.registerCallable("funcForMetaItem", std::function<int()>([]() {
  return 5;
}));
const metapp::MetaItem & callable = metaRepo.getCallable("funcForMetaItem");
```

The first argument in `metapp::callableInvoke` is `metapp::Variant`,
we can pass `metapp::MetaItem` to it the item will convert to `metapp::Variant`.

```c++
const metapp::Variant result = metapp::callableInvoke(callable, nullptr);
ASSERT(result.get<int>() == 5);
```

We can also get the Variant from the item explicitly

```c++
const metapp::Variant result2 = metapp::callableInvoke(callable.asCallable(), nullptr);
ASSERT(result2.get<int>() == 5);
```

<a id="a4_6"></a>
#### registerAnnotation

```c++
void registerAnnotation(const std::string & name, const Variant & value);
```

Register an annotation `value` under `name`.  
An annotation is any data. `metapp` doesn't use the data.  
The user can use the annotation for any purpose.
For example, if `metapp` is used in a property editor, an annotation may provide description,
or indicate a property should be hidden from the editor.

<a id="a4_7"></a>
#### getAnnotation

```c++
const Variant & getAnnotation(const std::string & name) const;
```

Returns the annotaion for `name`. If `name` is not registered, an empty Variant is returned (Variant::isEmpty() is true).

<a id="a4_8"></a>
#### getAllAnnotations

```c++
const std::map<std::string, Variant> & getAllAnnotations() const;
```

Returns all registered annotations in a `std::map`.
