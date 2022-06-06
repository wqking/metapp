[//]: # (Auto generated file, don't modify this file.)

# MetaItem
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [MetaItem member functions](#mdtoc_1a3b32fe)
  - [getType](#mdtoc_2a8cfdc6)
  - [isEmpty](#mdtoc_a01163fe)
  - [getName](#mdtoc_f871d4e9)
  - [asXxx functions](#mdtoc_e1cbbfa0)
  - [getTarget](#mdtoc_a98a2c7b)
  - [Implicitly type convert](#mdtoc_698b050a)
  - [registerAnnotation](#mdtoc_eb2a8a58)
  - [getAnnotation](#mdtoc_4f30f669)
  - [getAllAnnotations](#mdtoc_de77b03c)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaItem` represents the meta data registered in `MetaRepo` and `MetaClass`.  
When registering any meta data in `MetaRepo` and `MetaClass`, the register function returns a `MetaItem`.  
When retrieving any meta data from `MetaRepo` and `MetaClass`, the returned data is either `MetaItem` or `MetaItemView`.  

<a id="mdtoc_6e72a8c1"></a>
## Header


```c++
#include "metapp/metaitem.h"
```

<a id="mdtoc_1a3b32fe"></a>
## MetaItem member functions

<a id="mdtoc_2a8cfdc6"></a>
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
  variable,
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
The item is a callable that implements MetaCallable meta interface.  

**MetaItem::Type::variable**  
The item is a variable.  

**MetaItem::Type::constructor**  
The item is a constructor.

**MetaItem::Type::metaRepo**  
The item is a MetaRepo.  

**MetaItem::Type::metaType**  
The item is a MetaType.  

**MetaItem::Type::enumValue**  
The item is a value in `MetaEnum`.  

<a id="mdtoc_a01163fe"></a>
#### isEmpty

```c++
bool isEmpty() const;
```

Returns true if the item doesn't hold any valid data.  

<a id="mdtoc_f871d4e9"></a>
#### getName

```c++
const std::string & getName() const
```

Returns the name. If the item doesn't have name, returns empty string.  
`MetaItem::Type::constructor` always has empty name.  

<a id="mdtoc_e1cbbfa0"></a>
#### asXxx functions

```c++
const Variant & asAccessible() const;
const Variant & asCallable() const;
const Variant & asVariable() const;
const Variant & asConstructor() const;
const MetaType * asMetaType() const;
const MetaRepo * asMetaRepo() const;
const Variant & asEnumValue() const;
```

Returns the proper data.  
If the type doesn't match the function, exception `metapp::WrongMetaTypeException` is raised.  

<a id="mdtoc_a98a2c7b"></a>
#### getTarget

```c++
const Variant & getTarget() const;
```

Return the underlying Variant of the meta item. It doesn't check if the type matches, and it doesn't convert the data
to appropriate type such as `const MetaRepo *`.  

<a id="mdtoc_698b050a"></a>
#### Implicitly type convert

```c++
operator const Variant & () const;
```

Convert the item to Variant implicitly. This is useful to pass the meta item where a Variant is required. For example,  

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

<a id="mdtoc_eb2a8a58"></a>
#### registerAnnotation

```c++
void registerAnnotation(const std::string & name, const Variant & value);
```

Register an annotation `value` under `name`.  
An annotation is any data. `metapp` doesn't use the data.  
The user can use the annotation for any purpose.
For example, if `metapp` is used in a property editor, an annotation may provide description,
or indicate a property should be hidden from the editor.

<a id="mdtoc_4f30f669"></a>
#### getAnnotation

```c++
const Variant & getAnnotation(const std::string & name) const;
```

Returns the annotation for `name`. If `name` is not registered, an empty Variant is returned (Variant::isEmpty() is true).

<a id="mdtoc_de77b03c"></a>
#### getAllAnnotations

```c++
const std::map<std::string, Variant> & getAllAnnotations() const;
```

Returns all registered annotations in a `std::map`.

