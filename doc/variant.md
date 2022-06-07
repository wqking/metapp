[//]: # (Auto generated file, don't modify this file.)

# Variant reference
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Construct a Variant](#mdtoc_bdc0c616)
  - [Default constructor](#mdtoc_56b1be22)
  - [Construct from a value](#mdtoc_f3ffa08)
  - [Construct from a type and a value](#mdtoc_173682ea)
  - [Copy and move constructors](#mdtoc_d8985d88)
  - [create](#mdtoc_8fd6e0fb)
  - [reference](#mdtoc_aea34913)
  - [retype](#mdtoc_b607d61)
  - [takeFrom](#mdtoc_cb467e9a)
  - [takeFrom another Variant](#mdtoc_5ceda52a)
- [Member functions](#mdtoc_9ab1cb86)
  - [Assign from value](#mdtoc_6dd11bbb)
  - [Copy and move assignment](#mdtoc_4dd087b4)
  - [getMetaType](#mdtoc_83f2208d)
  - [canGet](#mdtoc_4273f5f8)
  - [get](#mdtoc_fd3b2e70)
  - [getAddress](#mdtoc_7e7d9c01)
  - [canCast](#mdtoc_f164fa3f)
  - [cast](#mdtoc_12b8b9f6)
  - [castSilently](#mdtoc_7ff798fb)
  - [isEmpty](#mdtoc_a01163fe)
  - [clone](#mdtoc_ec6dedd8)
  - [assign](#mdtoc_7222a9a1)
  - [swap](#mdtoc_25938561)
- [Free functions](#mdtoc_dafb9086)
  - [getTypeKind](#mdtoc_9973f311)
  - [Streaming operators](#mdtoc_c136ecac)
  - [swap](#mdtoc_25938562)
- [Memory management in Variant](#mdtoc_246aa466)
  - [The data storage in Variant is similar to native C++](#mdtoc_a4e1bd2)
  - [Copying variants is different from native C++](#mdtoc_3a417e4a)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`metapp::Variant` is a light weight class that allows to store data of any type and convert between types.  
Variant holds a single value of any type at the same time.
The type can be any C++ type, such as int, class, std::string, std::vector, function pointer, etc, any type.  

For example,  

```c++
metapp::Variant v1 = 5;
metapp::Variant v2 = std::vector<std::string>();
metapp::Variant v3 = MyClass();
```

In above code, the type held by v1 is metapp::tkInt, v2 is metapp::tkVector,
v3 is metapp::tkObject unless MyClass is registered with another type kind.

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/variant.h"
```

<a id="mdtoc_bdc0c616"></a>
## Construct a Variant

<a id="mdtoc_56b1be22"></a>
#### Default constructor

```c++
Variant() noexcept;
```

Construct an empty Variant of type `tkVoid`.

<a id="mdtoc_f3ffa08"></a>
#### Construct from a value

```c++
template <typename T>
Variant(T value);
```

Construct a Variant of type T and copy value into Variant.  
Since there is no way to specify the template parameter T explicitly when calling a constructor,
we can't construct reference (tkReference) or C array (tkArray) using this constructor,
because the type T is either removed reference, or decayed for pointer. To specify T explicitly, use `Variant::create`.  
If `value` is not copyable, it will be moved into Variant.  
If `value` is not copyable nor movable, exception `metapp::NotConstructibleException` is raised.  

<a id="mdtoc_173682ea"></a>
#### Construct from a type and a value

```c++
Variant(const MetaType * metaType, const void * copyFrom);
```

Construct a Variant of type `metaType`, and initialize with the object pointed by `copyFrom`.  
If `copyFrom` is nullptr, the object in the Variant is default constructed.  
If `copyFrom` is not nullptr, the object in the Variant is copied from the object pointed by `copyFrom`. In such case,
`copyFrom` must point to an object of the exact same type of `metaType`. The constructor doesn't and can't validate `copyFrom`.

If `metaType` is not copyable, `copyFrom` will be moved into Variant.  
If `metaType` is not copyable nor movable, exception `metapp::NotConstructibleException` is raised.  

Note: `Variant(metaType)` will create a Variant that holds a pointer to a MetaType (tkMetaType), it's different with
`Variant(metaType, nullptr)`.

<a id="mdtoc_d8985d88"></a>
#### Copy and move constructors

```c++
Variant(const Variant & other) noexcept;
Variant(Variant && other) noexcept;
```

Copy and move constructors.

<a id="mdtoc_8fd6e0fb"></a>
#### create
```c++
template <typename T>
static Variant create(T value);
```
Construct a Variant of type T and copy value into Variant, then return the Variant.  
This is similar with the constructor `template <typename T> Variant(T value)`,
the `create` function allows to specify T explicitly, which is useful to construct reference or array,
or object with top level CV qualifiers.  
If T is metapp::Variant or reference to metapp::Variant, value is returned directly.  

If `value` is not copyable, it will be moved into Variant.  
If `value` is not copyable nor movable, exception `metapp::NotConstructibleException` is raised.  

**Example**

```c++
int n = 5;

// The type held by v1 is tkReference
metapp::Variant v1 = metapp::Variant::create<int &>(n);
ASSERT(v1.getMetaType()->isReference());

// The type held by v2 is const int
metapp::Variant v2 = metapp::Variant::create<const int>(n);
ASSERT(v2.getMetaType()->isConst());

// On contrary, if we don't use create, the meta type is different.
// The type held by v3 is tkInt, not tkReference
metapp::Variant v3(n);
ASSERT(! v3.getMetaType()->isReference());

// The type held by v4 is int, not const
metapp::Variant v4= n;
ASSERT(! v4.getMetaType()->isConst());
```

<a id="mdtoc_aea34913"></a>
#### reference
```c++
template <typename T>
static Variant reference(T && value);
```

Create a Variant of reference to `value`. `value` can be either lvalue or rvalue reference.  
This is equivalent to `Variant::create<T &>(value);`, but in `reference` the template argument can be deduced.

**Example**

```c++
int n = 5;
// The type held by v is tkReference
metapp::Variant v = metapp::Variant::reference(n);
```

<a id="mdtoc_b607d61"></a>
#### retype
```c++
static Variant retype(const MetaType * metaType, const Variant & var);
```

Return a Variant which data is the data in `var`, type is `metaType`.  
This is only useful if you are 100% sure the data in `var` can be in type `metaType`,
otherwise, you should cast the Variant instead of retyping it.  

<a id="mdtoc_cb467e9a"></a>
#### takeFrom
```c++
static Variant takeFrom(const MetaType * metaType, void * instance);
```

Return a Variant which data is the object pointed by `instance`, type is `metaType`.  
`instance` is a pointer that points to an object allocated on the heap,
the constructed Variant will take and manage the ownership of `instance`, so `instance` should not be freed any elsewhere.  
`metaType` should be the type that `instance` points to, not the pointer type.
The returned `Variant` is a value object, not a pointer.

**Example**  

```c++
std::string * instance = new std::string("Hello");
metapp::Variant v = metapp::Variant::takeFrom(metapp::getMetaType<std::string>(), instance);
// v is a value, so we should get as value or reference, but not pointer
ASSERT(v.get<const std::string &>() == "Hello");
// Now v will free instance when v is destroyed
```

<a id="mdtoc_5ceda52a"></a>
#### takeFrom another Variant
```c++
static Variant takeFrom(const Variant & var);
```

Return a Variant which data is the pointer in `var`, type is the type in `var`.  
`var` must hold a pointer. It must not hold the object direct.  
This function is not useful in most cases. The only useful case is to use it on the return value when invoking a meta constructor.

**Example**  

```c++
// This is wrong. In this case, var hold the ownership of MyClass
// takeFrom can't take the ownership from var
//metapp::Variant var(std::string());

// This works, var hold the pointer, it doesn't hold the ownership of MyClass
// Note: if we don't call takeFrom later, var will leak the object
// because it doesn't hold the ownership.
metapp::Variant var(new std::string());
metapp::Variant v(metapp::Variant::takeFrom(var));
```

<a id="mdtoc_9ab1cb86"></a>
## Member functions

<a id="mdtoc_6dd11bbb"></a>
#### Assign from value
```c++
template <typename T>
Variant & operator = (T value) noexcept;
```

Assign to the Variant with `value`.  
The previous value held by the variant is destroyed after assigned with the new value.  

<a id="mdtoc_4dd087b4"></a>
#### Copy and move assignment
```c++
Variant & operator = (const Variant & other) noexcept;
Variant & operator = (Variant && other) noexcept;
```
Copy and move assignment.  
The previous value held by the variant is destroyed after assigned with the new variant.  
Example code,  

```c++
metapp::Variant t(5);
ASSERT(t.getMetaType()->equal(metapp::getMetaType<int>())); // t is int
ASSERT(t.get<int>() == 5);
metapp::Variant u(38.2);
ASSERT(u.getMetaType()->equal(metapp::getMetaType<double>())); // u is double

t = u;
ASSERT(t.getMetaType()->equal(metapp::getMetaType<double>())); // t is double
ASSERT(t.get<double>() == 38.2);
```

<a id="mdtoc_83f2208d"></a>
#### getMetaType
```c++
const MetaType * getMetaType() const noexcept;
```
Return the meta type held by the variant. The result is always valid pointer.
Any variant, including the default constructed, always contains a meta type.

<a id="mdtoc_4273f5f8"></a>
#### canGet
```c++
template <typename T>
bool canGet() const; // #1

bool canGet(const MetaType * toMetaType) const; // #2
```
Return true if `myVariant.get<T>()` can be called to get the underlying value.  
#1 form is equivalent to `canGet(metapp::getMetaType<T>())`. 

The rules to determine `canGet`, assume the underlying value has type V,  
1. If both T and V are references, `canGet` returns true.  
2. If both T and V are pointers after reference is removed, `canGet` returns true.  
3. If both T and V are C array after reference is removed, `canGet` returns true.  
4. If either T or V is reference, the other one is not reference,
`canGet` returns true only if the referred type is same as the non-reference type.  
5. If neither T and V are reference or pointer, `canGet` returns true only if T is same as V.  

`canGet` and `get` expect either T is same as underlying type V, or T and V are reference are pointer.  
If You need to get the underlying value as different type, use `canCast` and `cast`.  

**Example**  

```c++
int n = 5;
metapp::Variant v1(metapp::Variant::create<int &>(n)); // reference
ASSERT(v1.canGet<int>()); // rule 3
ASSERT(v1.canGet<int &>()); // rule 1
ASSERT(v1.canGet<const int &>()); // rule 1
ASSERT(v1.get<int>() == 5);
ASSERT(v1.get<int &>() == 5);
ASSERT(v1.get<const int &>() == 5);
ASSERT(v1.canGet<long &>()); // rule 1
v1.get<long &>(); // this may return wrong value, because long & is not int &

metapp::Variant v2(38); // int
ASSERT(v2.canGet<int>()); // rule 4
ASSERT(v2.canGet<int &>()); // rule 3
ASSERT(! v2.canGet<long>()); // rule 4

int m = 9;
metapp::Variant v3(&m); // pointer
ASSERT(v3.canGet<int *>()); // rule 2
ASSERT(v3.canGet<const int *>()); // rule 2
ASSERT(! v3.canGet<int>()); // rule 2
```

<a id="mdtoc_fd3b2e70"></a>
#### get
```c++
template <typename T>
T get() const;
```
If `canGet<T>()` returns true, `get` returns the underlying value as T.  
If `canGet<T>()` returns false, it throws exception `metapp::BadCastException`.  
If T is array such as int[3], the return type is the reference to the array, e.g, int(&)[3].
If T is function type, the return type is function pointer.  

T can be reference of the underlying type. For example, if the a Variant `v` holds a std::string,
we can call `v.get<std::string &>()`, or `v.get<const std::string &>()` to get a reference
instead of copy the value. That helps to improve the performance.  
We should always getting as reference to avoid copying, unless you do want to copy the value.  

Note: `canGet` and `get` are not type safe when either types (T and the type inside the Variant)
are pointer and reference. The reason is `canGet` and `get` assume the user have knowledge on the type
held by the Variant and they provide a fast way to access the underlying pointer and reference.  
If you pursuit better type safety, use `canCast` and `cast`.  
If you want to `get` with strong type safety when there are pointers or reference, you can compare
the types before `get`, for example,  

```c++
metapp::Variant v;
if(metapp::getNonReferenceMetaType(v)->equal(metapp::getMetaType<int *>())) {
  // We are sure v holds a pointer to int, such as `int *`, or `const int *`, etc.
  int * p = v.get<int *>(); // safe
  *p = 5;
}
// Below check is not safe, `canGet` returns true even if v holds a `double *`.
if(v.canGet<int *>()) {
}
```

<a id="mdtoc_7e7d9c01"></a>
#### getAddress
```c++
void * getAddress() const;
```
Return the address of underlying value. This is the same semantic as the address-of operator & in C++.  
If the Variant holds a reference, returns the address of the value it refers to.  
Otherwise, return the address of the underlying value.  

**Example**  

```c++
metapp::Variant v1(5);
ASSERT(v1.get<int>() == 5);

// equivalent native C++: *&v1 = 38;
*(int *)v1.getAddress() = 38;
ASSERT(v1.get<int>() == 38);
int n1 = 8;
int n2 = 9;
// pointer, equivalent native C++: int * v2 = &n1;
metapp::Variant v2(&n1);
ASSERT(* v2.get<int *>() == 8);
// equivalent native C++: *&v2 = &n2;
*(int **)v2.getAddress() = &n2;
ASSERT(* v2.get<int *>() == 9);

int m = 10;
// reference, equivalent native C++: int & v3 = m;
metapp::Variant v3(metapp::Variant::reference(m));
ASSERT(m == 10);
// equivalent native C++: *&v3 = 15;
*(int *)v3.getAddress() = 15;
ASSERT(m == 15);
```

<a id="mdtoc_f164fa3f"></a>
#### canCast
```c++
bool canCast(const MetaType * toMetaType) const;

// This template form is same as
// canCast(metapp::getMetaType<T>());
template <typename T>
bool canCast() const;
```

Return true if `myVariant.cast(toMetaType)` can be called to cast the underlying value to `toMetaType`.   
Note: if both the Variant and `toMetaType` are class pointers or references,
the function will use the global MetaRepoList returned by `metapp::getMetaRepoList()` to cast between base and derived classes.  
Below table shows the rules to determine `canCast`, assume the underlying value has meta type `from`,
and we want to cast it to type `to` (which is `toMetaType`), `F` and `T` are value type, they are not reference, not pointer.  

| from | to  | canCast returns                                                                                            |
|------|-----|------------------------------------------------------------------------------------------------------------|
| F &  | T & | If both F and T are registered class, return the result of canCast on F and T.<br/>If F == T, return referent to T. Otherwise, return cast F to T, which is not a reference any more. |
| F    | T & | the result of canCast on F and T                                                                           |
| F &  | T   | the result of canCast on F and T                                                                           |
| F *  | T * | If both F and T are registered class, return the result of canCast on F and T.<br/>Otherwise, return F == T |
| F *  | T   | false                                                                                                      |
| F    | T * | false                                                                                                      |
| F    | T   | determined by canCast                                                                                      |

<a id="mdtoc_12b8b9f6"></a>
#### cast
```c++
Variant cast(const MetaType * toMetaType) const;

// This template form is same as
// cast(metapp::getMetaType<T>());
template <typename T>
Variant cast() const;
```

If `canCast` returns true, `cast` returns the casted variant which type matches `toMetaType` or `T`.  
If `canCast<T>()` returns false, it throws exception `metapp::BadCastException`.  
To get the casted value, call `get` on the returned variant. For example, `int castedValue = v.cast<int>().get<int>()`.  

<a id="mdtoc_7ff798fb"></a>
#### castSilently
```c++
Variant castSilently(const MetaType * toMetaType) const;
template <typename T>
Variant castSilently() const;
```

Similar to `cast`, the only difference is that if `canCast<T>()` returns false,
`castSilently` doesn't throw exception, and returns an empty Variant (which isEmpty() is true).  

**Hint**: when to use `canCast`, `cast`, and `castSilently`   
If you only want to check if it's cast-able, but don't need to perform the cast, use `canCast`.  
If you want a variant must be casted, and throw exception if it can't be casted,
use `cast` without checking `canCast` explicitly because `cast` will check `canCast`, and `canCast` is almost
as expensive on performance as `cast`.  
If you want a variant be casted, and allow the cast fail, use `castSilently`, then check if the result is empty.  

<a id="mdtoc_a01163fe"></a>
#### isEmpty
```c++
bool isEmpty() const noexcept;
```
Return true if the variant holds `tkVoid`. A default constructed variant holds `tkVoid`.
Such a variant can't be got value, and can't be casted.  

<a id="mdtoc_ec6dedd8"></a>
#### clone
```c++
Variant clone() const;
```

Clone the underlying object and return a Variant that holds the cloned object.  
To understand how `clone` works, please see the section "Memory management in Variant".  

<a id="mdtoc_7222a9a1"></a>
#### assign

```c++
Variant & assign(const Variant & other);
```

Assign `other` to `this`.  
Firstly the function casts `other` to the meta type in `this`, then copy the data in the casted Variant to the data in `this`.  
If `this` is a Variant of reference, the referred-to object is modified. Otherwise, the object contained by the Variant is modified.
This function is particular useful to set value to the referred-to object referred by a reference.  

This function is complete different with the `Variant & operator = (const Variant & other) noexcept`.  
The `operator =` is class semantic. That's to say, when using `operator =`, `this` is a fresh new Variant that's copied from `other`.  
Function `assign` is C++ value assignment semantic. That's to say, it's similar to do the expression `v = u`.  
Let's see examples, first let's see how C++ assignment works.  

```c++
T t;
U u;
t = u;
```
In above code, `u` is casted to type `T`, then assign to `t`. After the assignment, `t` still has type `T`. If `t` is reference,  

```c++
T n;
T & t = n;
U u;
t = u;
```
In above code, after the assignment, `n` will receive the new value of `u`.  

Now let's see how Variant `assign` works.  

```c++
// Assign to value.
metapp::Variant t(5);
ASSERT(t.getMetaType()->equal(metapp::getMetaType<int>())); // t is int
ASSERT(t.get<int>() == 5);
metapp::Variant u(38.2);
ASSERT(u.getMetaType()->equal(metapp::getMetaType<double>())); // u is double

t.assign(u);
ASSERT(t.getMetaType()->equal(metapp::getMetaType<int>())); // t is still int
ASSERT(t.get<int>() == 38); // t receives new value (int)38.2, that's 38
```

```c++
// Assign to reference.
int n = 5;
metapp::Variant t = metapp::Variant::reference(n);
ASSERT(t.getMetaType()->equal(metapp::getMetaType<int &>())); // t is int &
ASSERT(t.get<int>() == 5);
metapp::Variant u(38.2);
ASSERT(u.getMetaType()->equal(metapp::getMetaType<double>())); // u is double

t.assign(u);
ASSERT(t.getMetaType()->equal(metapp::getMetaType<int &>())); // t is still int &
ASSERT(t.get<int>() == 38); // t receives new value (int)38.2, that's 38
ASSERT(n == 38); // n is also modified
```

<a id="mdtoc_25938561"></a>
#### swap
```c++
void swap(Variant & other) noexcept;
```

Swap with another variant.

<a id="mdtoc_dafb9086"></a>
## Free functions

<a id="mdtoc_9973f311"></a>
#### getTypeKind
```c++
TypeKind getTypeKind(const Variant & v);
```

Get the TypeKind held by the variant. This is a shortcut function for `v.getMetaType()->getTypeKind()`.

<a id="mdtoc_c136ecac"></a>
#### Streaming operators
```c++
std::istream & operator >> (std::istream & stream, Variant & v);
std::ostream & operator << (std::ostream & stream, const Variant & v);
```

Variant supports input and output stream if the underlying value supports the stream.  
If the underlying value doesn't support streaming, invoking the I/O streaming operators wll throw `metapp::UnsupportedException`.

<a id="mdtoc_25938562"></a>
#### swap
```c++
void swap(Variant & a, Variant & b) noexcept;

```
Swap two variants.


<a id="mdtoc_246aa466"></a>
## Memory management in Variant

<a id="mdtoc_a4e1bd2"></a>
#### The data storage in Variant is similar to native C++

If the underlying value is pointer or reference, Variant only stores the pointer or reference,
it doesn't store the data pointed by the pointer or reference.  
If the underlying value is C array, the array is copied to the internal memory.  
If the underlying value is function, it's decayed to function pointer.  
If the underlying value is not a pointer or reference, Variant copies the value to the internal memory,
and destroy the value (call the destructor if the value is an object) when the Variant is destroyed, or assigned with another value.  

<a id="mdtoc_3a417e4a"></a>
#### Copying variants is different from native C++

For value which is fundamental types such as int, long, or pointer, or any POD struct
which size is smaller enough (the max size is 8 or 16 bytes, depending on the platform and the compiler),
the value is stored in Variant directly. That means when the Variant is copied, the value is copied too.  
For value which size is not small, or not POD data, the value is stored on the heap using a `std::shared_ptr`
that's managed by Variant. That's to say, when the Variant is copied, the value is not copied.
If you want the value be copied, use `Variant::clone`.  
Copying Variant is always trivial, there is not any memory allocation.  

