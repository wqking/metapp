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
1. If both T and V are reference, `canGet` returns true.  
2. If both T and V are pointer, `canGet` returns true.  
3. If either T or V is reference, the other one is not reference, `canGet` returns true only if the referred type is same as the non-reference type.  
4. If neither T and V are reference or pointer, `canGet` returns true only if T is same as V.  

`canGet` and `get` expect either T is same as underlying type V, or T and V are reference are pointer.  
If You need to get the underlying value as different type, use `canCast` and `cast`.  

Example code,
```c++
int n = 5;
metapp::Variant v1(metapp::Variant::create<int &>(n)); // reference
assert(v1.canGet<int>()); // rule 3
assert(v1.canGet<int &>()); // rule 1
assert(v1.canGet<const int &>()); // rule 1
assert(v1.get<int>() == 5);
assert(v1.get<int &>() == 5);
assert(v1.get<const int &>() == 5);
assert(v1.canGet<long &>()); // rule 1
v1.get<long &>(); // this may return wrong value, because long & is not int &

metapp::Variant v2(38);
assert(v2.canGet<int>()); // rule 4
assert(v2.canGet<int &>()); // rule 3
assert(! v2.canGet<long>()); // rule 4

int m = 9;
metapp::Variant v3(&m); // pointer
assert(v3.canGet<int *>()); // rule 2
assert(v3.canGet<const int *>()); // rule 2
assert(! v3.canGet<int>()); // rule 2
```

#### get
```c++
template <typename T>
T get() const;
```
If `canGet<T>()` returns true, `get` returns the underlying value as T.  
If `canGet<T>()` returns false, it throws exception `metapp::BadCastException`.  
If T is C array or function type, the return type is std::decay<T>::type.  

#### getAddress
```c++
void * getAddress() const;
```
Return the address of underlying value. This is the same semantic as the address-of operator & in C++.  
If the Variant holds a reference, returns the address of the value it refers to.  
Otherwise, return the address of the underlying value.  

Example code,  
```c++
metapp::Variant v1(5);
assert(v1.get<int>() == 5);

// equivalent native C++: *&v1 = 38;
*(int *)v1.getAddress() = 38;
assert(v1.get<int>() == 38);
int n1 = 8;
int n2 = 9;
// pointer, equivalent native C++: int * v2 = &n1;
metapp::Variant v2(&n1);
assert(* v2.get<int *>() == 8);
// equivalent native C++: *&v2 = &n2;
*(int **)v2.getAddress() = &n2;
assert(* v2.get<int *>() == 9);

int m = 10;
// reference, equivalent native C++: int & v3 = m;
metapp::Variant v3(metapp::Variant::create<int &>(m));
assert(m == 10);
// equivalent native C++: *&v3 = 15;
*(int *)v3.getAddress() = 15;
assert(m == 15);
```

#### toReference
```c++
Variant toReference() const;
```
Return a Variant which is a reference that refers to the underlying in `this` Variant.  
If `this` Variant is a reference, `*this` is returned.  
If `this` Variant is a pointer, the returned reference refers to the value that the pointer points to.  
If `this` Variant is a value, the returned reference refers to the value.  
`toReference` only makes reference, it doesn't copy any underlying value.  

Example code,  
```c++
metapp::Variant v1(5); // value
assert(v1.get<int>() == 5);
metapp::Variant r1(v1.toReference());
assert(r1.get<int &>() == 5);
r1.get<int &>() = 38;
assert(v1.get<int>() == 38);
assert(r1.get<int &>() == 38);

int n = 9;
// pointer, points to n;
metapp::Variant v2(&n);
assert(n == 9);
assert(*v2.get<int *>() == 9);
// r2 refers to n
metapp::Variant r2(v2.toReference()); 
assert(r2.get<int &>() == 9);
r2.get<int &>() = 10;
assert(n == 10);
assert(*v2.get<int *>() == 10);

int m = 10;
// reference, refers to m;
metapp::Variant v3(metapp::Variant::create<int &>(m));
assert(m == 10);
assert(v3.get<int &>() == 10);
// r3 refers to m
metapp::Variant r3(v3.toReference()); 
assert(r3.get<int &>() == 10);
r3.get<int &>() = 15;
assert(m == 15);
assert(v3.get<int &>() == 15);
assert(r3.get<int &>() == 15);
```

#### dereference
```c++
Variant dereference() const;
```
Return the value that the underlying pointer or reference points to. This is the same semantic as the dereference operator * in C++.  
If `this` Variant is a reference, return the value that `this` refers to.  
If `this` Variant is a pointer, return the value that `this` points to.  
If `this` Variant is a value, return `*this`.  
Note: if `this` Variant is a reference or pointer, `dereference` will copy the underlying value to the result Variant, which may be expensive.  

#### canCast
```c++
bool canCast(const MetaType * toMetaType) const;

// This template form is same as
// canCast(metapp::getMetaType<T>());
template <typename T>
bool canCast() const;
```

Return true if `myVariant.cast(toMetaType)` can be called to cast the underlying value to `toMetaType`.  
Below table shows the rules to determine `canCast`, assume the underlying value has meta type `from`, and we want to cast it to type `to` (which is `toMetaType`), `F` and `T` are value type, they are not reference, not pointer.  

| from | to  | canCast returns                  |
|------|-----|----------------------------------|
| F &  | T & | the result of canCast on F and T |
| F    | T & | the result of canCast on F and T |
| F &  | T   | the result of canCast on F and T |
| F *  | T * | the result of canCast on F and T |
| F *  | T   | false                            |
| F    | T * | false                            |
| F    | T   | determined by canCast            |

#### cast
```c++
Variant cast(const MetaType * toMetaType) const;

// This template form is same as
// cast(metapp::getMetaType<T>());
template <typename T>
Variant cast() const;
```


## Memory management in Variant

