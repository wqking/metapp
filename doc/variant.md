# Variant

## Overview

`metapp::Variant` allows to store data of any type and convert between types.  
Variant holds a single value of any type at the same time. They type can be any C++ type, such as int, class, std::string, std::vector, function pointer, etc, any type.  

For example,  

```c++
metapp::Variant v1 = 5;
metapp::Variant v2 = std::vector<std::string>();
metapp::Variant v3 = MyClass();
```

In above code, the type held by v1 is metapp::tkInt, v2 is metapp::tkVector, v3 is metapp::tkObject unless MyClass is regitered with another type kind.

## Header

```c++
#include "metapp/variant.h"
```

## Construct a Variant

#### Default constructor

```c++
Variant() noexcept;
```

Construct an empty Variant of type `tkVoid`.

#### Construct from a value

```c++
template <typename T>
Variant(T value);
```

Construct a Variant of type T and copy value into Variant.  
Since there is no way to specify the template parameter T explicitly when calling a constructor, we can't construct reference (tkReference) or C array (tkArray) using this constructor, because the type T is either removed reference, or decayed for array. To specify T explicitly, use `Variant::create`.  

#### Construct from a type

```c++
explicit Variant(const MetaType * metaType);
```

Construct a Variant of type `metaType`, and initialize the default value using `metaType`.

#### Construct from a type and a value

```c++
Variant(const MetaType * metaType, const void * copyFrom);
```

Construct a Variant of type `metaType`, and initialize with the object pointed by `copyFrom`.

#### Copy and move constructors

```c++
Variant(const Variant & other) noexcept;
Variant(Variant && other) noexcept;
```

Copy and move constructors.

#### create
```c++
template <typename T>
static Variant create(T value);
```
Construct a Variant of type T and copy value into Variant, then return the Variant.  
This is similar with the construct `template <typename T> Variant(T value);`, but the `create` function allows to specify T explicitly, which is useful to construct reference or array.  
If T is metapp::Variant or reference to metapp::Variant, value is returned directly.  

Example code,

```c++
int n = 5;
// The type held by v is tkReference
metapp::Variant v = metapp::Variant::create<int &>(n);
```

#### retype
```c++
static Variant retype(const MetaType * metaType, const Variant & var);
```

Return a Variant which data is the data in `var`, type is `metaType`.  
This is only useful if you are 100% sure the data in `var` can be in type `metaType`, otherwise, you should cast the Variant instead of retyping it.  

#### takeFrom
```c++
static Variant takeFrom(const MetaType * metaType, void * instance);
```

Return a Variant which data is the `instance`, type is `metaType`.  
`instance` is a pointer that points to an object allocated on the heap, the constructed Variant will take and manage the ownership of `instance`, so `instance` should not be freed any elsewhere.  
Example code,  

```c++
MyClass * instance = new MyClass();
metapp::Variant v = metapp::Variant::takeFrom(metapp::getMetaType<MyClass>(), instance);
// Now v will free instance when v is destoryed
```

#### takeFrom another Variant
```c++
static Variant takeFrom(const Variant & var);
```

Return a Variant which data is the pointer in `var`, type is the type in `var`.  
`var` must hold a pointer. It must not hold the object direct.  
This function is not useful in most case. The only useful case is to use it on the return value when invoking a meta constructor.

Example code,  

```c++
// This is wrong. In this case, var hold the ownership of MyClass
// takeFrom can't take the ownership from var
//metapp::Variant var(MyClass());

// This works, var hold the pointer, it doesn't hold the ownership of MyClass
// Note: if we don't call takeFrom later, var will leak the object because it doesn't hold the ownership.
metapp::Variant var(new MyClass());
metapp::Variant v(metapp::Variant::takeFrom(var));
```

## Member functions

#### Assign from value
```c++
template <typename T>
Variant & operator = (T value) noexcept;
```

Assign to the Variant with `value`.  

#### Copy and move assignment
```c++
Variant & operator = (const Variant & other) noexcept;
Variant & operator = (Variant && other) noexcept;
```
Copy and move assignment.

#### clone
```c++
Variant clone() const;
```
Clone the underlying object and return a Variant that holds the cloned object.  
To understand how `clone` works, please see the section "Memory management in Variant".  

#### canGet
```c++
template <typename T>
bool canGet() const;
```
Return true if `myVariant.get<T>()` can be called to get the underlying value.  
The rules to determine `canGet`, assume the underlying value has type V,  
- If both T and V are reference, `canGet` returns true.  
- If both T and V are pointer, `canGet` returns true.  
- If either T or V is reference, the other one is not reference, `canGet` returns true only if the referred type is same as the non-reference type.  
- If neither T and V are reference or pointer, `canGet` returns true only if T is same as V.  

#### get
```c++
template <typename T>
T get() const;
```
If `canGet<T>()` returns true, `get` returns the underlying value as T.  
If `canGet<T>()` returns false, it throws exception `metapp::BadCastException`.  


## Memory management in Variant

