// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testutil.h"

#include "metapp/allmetatypes.h"

/*desc
# Variant reference

## Overview

`metapp::Variant` allows to store data of any type and convert between types.  
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

## Header
desc*/

//code
#include "metapp/variant.h"
//code

/*desc
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
Since there is no way to specify the template parameter T explicitly when calling a constructor,
we can't construct reference (tkReference) or C array (tkArray) using this constructor,
because the type T is either removed reference, or decayed for array. To specify T explicitly, use `Variant::create`.  

#### Construct from a type

```c++
explicit Variant(const MetaType * metaType);
```

Construct a Variant of type `metaType`, and initialize the default value using `metaType`.  
Note: this constructor can't create Variant of pointer to a MetaType, to create MetaType pointer, use `Variant::create`.

#### Construct from a type and a value

```c++
Variant(const MetaType * metaType, const void * copyFrom);
```

Construct a Variant of type `metaType`, and initialize with the object pointed by `copyFrom`.
`copyFrom` must point to an object of the exact same type of `metaType`. The constructor doesn't and can't validate `copyFrom`.

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
This is similar with the constructor `template <typename T> Variant(T value)`,
the `create` function allows to specify T explicitly, which is useful to construct reference or array.  
If T is metapp::Variant or reference to metapp::Variant, value is returned directly.  

**Example**
desc*/

ExampleFunc
{
	//code
	int n = 5;
	// The type held by v is tkReference
	metapp::Variant v = metapp::Variant::create<int &>(n);
	//code
}

/*desc
#### reference
```c++
template <typename T>
static Variant reference(T && value);
```

Create a Variant of reference to `value`. `value` can be either lvalue or rvalue reference.  
This is equivalent to `Variant::create<T &>(value);`, but in `reference` the template argument can be deduced.

**Example**
desc*/

ExampleFunc
{
	//code
	int n = 5;
	// The type held by v is tkReference
	metapp::Variant v = metapp::Variant::reference(n);
	//code
}

/*desc
#### retype
```c++
static Variant retype(const MetaType * metaType, const Variant & var);
```

Return a Variant which data is the data in `var`, type is `metaType`.  
This is only useful if you are 100% sure the data in `var` can be in type `metaType`,
otherwise, you should cast the Variant instead of retyping it.  

#### takeFrom
```c++
static Variant takeFrom(const MetaType * metaType, void * instance);
```

Return a Variant which data is the object pointed by `instance`, type is `metaType`.  
`instance` is a pointer that points to an object allocated on the heap,
the constructed Variant will take and manage the ownership of `instance`, so `instance` should not be freed any elsewhere.  
`metaType` should the type that `instance` points to, not the pointer type.
The returned `Variant` is a value object, not a pointer.

**Example**  
desc*/

ExampleFunc
{
	//code
	std::string * instance = new std::string();
	metapp::Variant v = metapp::Variant::takeFrom(metapp::getMetaType<std::string>(), instance);
	// Now v will free instance when v is destroyed
	//code
}

/*desc
#### takeFrom another Variant
```c++
static Variant takeFrom(const Variant & var);
```

Return a Variant which data is the pointer in `var`, type is the type in `var`.  
`var` must hold a pointer. It must not hold the object direct.  
This function is not useful in most cases. The only useful case is to use it on the return value when invoking a meta constructor.

**Example**  
desc*/

ExampleFunc
{
	//code
	// This is wrong. In this case, var hold the ownership of MyClass
	// takeFrom can't take the ownership from var
	//metapp::Variant var(std::string());

	// This works, var hold the pointer, it doesn't hold the ownership of MyClass
	// Note: if we don't call takeFrom later, var will leak the object
	// because it doesn't hold the ownership.
	metapp::Variant var(new std::string());
	metapp::Variant v(metapp::Variant::takeFrom(var));
	//code
}

/*desc
## Member functions

#### Assign from value
```c++
template <typename T>
Variant & operator = (T value) noexcept;
```

Assign to the Variant with `value`.  
The previous value held by the variant is destroyed after assigned with the new value.  

#### Copy and move assignment
```c++
Variant & operator = (const Variant & other) noexcept;
Variant & operator = (Variant && other) noexcept;
```
Copy and move assignment.  
The previous value held by the variant is destroyed after assigned with the new variant.  

#### getMetaType
```c++
const MetaType * getMetaType() const noexcept;
```
Return the meta type held by the variant. The result is always valid pointer.
Any variant, including the default constructed, always contains a meta type.

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
desc*/

ExampleFunc
{
	//code
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
	//code
}

/*desc
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

#### getAddress
```c++
void * getAddress() const;
```
Return the address of underlying value. This is the same semantic as the address-of operator & in C++.  
If the Variant holds a reference, returns the address of the value it refers to.  
Otherwise, return the address of the underlying value.  

**Example**  
desc*/

ExampleFunc
{
	//code
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
	metapp::Variant v3(metapp::Variant::create<int &>(m));
	ASSERT(m == 10);
	// equivalent native C++: *&v3 = 15;
	*(int *)v3.getAddress() = 15;
	ASSERT(m == 15);
	//code
}

/*desc
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
use `cast` without checking `canCast` because 'canCast` is almost as expensive on performance as `cast`.  
If you want a variant be casted, and allow the cast fail, use `castSilently`, then check if the result is empty.  

#### isEmpty
```c++
bool isEmpty() const noexcept;
```
Return true if the variant holds `tkVoid`. A default constructed variant holds `tkVoid`.
Such a variant can't be got value, and can't be casted.  

#### clone
```c++
Variant clone() const;
```

Clone the underlying object and return a Variant that holds the cloned object.  
To understand how `clone` works, please see the section "Memory management in Variant".  

#### depointer
```c++
Variant depointer() const;
```
Convert a pointer to its non-pointer equivalence.  
Return a Variant that,  
If `this` Variant is a value or reference, `*this` is returned.  
If `this` Variant is a pointer, the returned reference refers to the value that the pointer points to.  
`depointer` only makes reference, it doesn't copy any underlying value.  

`depointer` is useful to write generic code. Assume a function accepts an argument of Variant.
Without `depointer`, the function either requires the Variant to be a value/reference, or a pointer,
but not both, or the function uses extra code to detect whether the argument is a value/reference or pointer.
With `depointer`, the argument can be value, reference, or pointer, then the function calls `depointer`
to normalize the argument, and use a single logic for all three kinds of Variants (value, reference pointer).

**Example**  
desc*/

ExampleFunc
{
	//code
	metapp::Variant v1(5); // value
	ASSERT(v1.get<int>() == 5);
	// r1 is a value too
	metapp::Variant r1(v1.depointer());
	ASSERT(r1.get<int &>() == 5);
	r1.get<int &>() = 38;
	// Assignment doesn't affect original value
	ASSERT(v1.get<int>() == 5);
	ASSERT(r1.get<int &>() == 38);

	int n = 9;
	// pointer, points to n;
	metapp::Variant v2(&n);
	ASSERT(n == 9);
	ASSERT(*v2.get<int *>() == 9);
	// r2 refers to n
	metapp::Variant r2(v2.depointer()); 
	ASSERT(r2.get<int &>() == 9);
	r2.get<int &>() = 10;
	ASSERT(n == 10);
	ASSERT(*v2.get<int *>() == 10);

	int m = 10;
	// reference, refers to m;
	metapp::Variant v3(metapp::Variant::create<int &>(m));
	ASSERT(m == 10);
	ASSERT(v3.get<int &>() == 10);
	// r3 refers to m
	metapp::Variant r3(v3.depointer()); 
	ASSERT(r3.get<int &>() == 10);
	r3.get<int &>() = 15;
	ASSERT(m == 15);
	ASSERT(v3.get<int &>() == 15);
	ASSERT(r3.get<int &>() == 15);
	//code
}

/*desc
#### dereference
```c++
Variant dereference() const;
```
Return the value that the underlying pointer or reference points to.
This is the same semantic as the dereference operator * in C++.  
If `this` Variant is a reference, return the value that `this` refers to.  
If `this` Variant is a pointer, return the value that `this` points to.  
If `this` Variant is a value, return `*this`.  
Note: if `this` Variant is a reference or pointer, `dereference` will copy the underlying value to the result Variant,
which may be expensive.  

#### swap
```c++
void swap(Variant & other) noexcept;
```

Swap with another variant.

## Free functions

#### getTypeKind
```c++
TypeKind getTypeKind(const Variant & v);
```

Get the TypeKind held by the variant. This is a shortcut function for `v.getMetaType()->getTypeKind()`.

#### Streaming operators
```c++
std::istream & operator >> (std::istream & stream, Variant & v);
std::ostream & operator << (std::ostream & stream, const Variant & v);
```

Variant supports input and output stream if the underlying value supports the stream.  
If the underlying value doesn't support the stream, invoking the I/O streaming operators wll throw `metapp::UnsupportedException`.

#### swap
```c++
void swap(Variant & a, Variant & b) noexcept;

```
Swap two variants.


## Memory management in Variant

#### The data storage in Variant is similar to native C++

If the underlying value is pointer or reference, Variant only stores the pointer or reference,
it doesn't store the data pointed by the pointer or reference.  
If the underlying value is C array, the array is copied to the internal memory.  
If the underlying value is function, it's decayed to function pointer.  
If the underlying value is not a pointer or reference, Variant copies the value to the internal memory,
and destroy the value (call the destructor if the value is an object) when the Variant is destroyed, or assigned with another value.  

#### Copying variants is different from native C++

For value which is fundamental types such as int, long, or pointer, or any POD struct
which size is smaller enough (the max size is 8 or 16 bytes, depending on the platform and the compiler),
the value is stored in Variant directly. That means when the Variant is copied, the value is copied too.  
For value which size is not small, or not POD data, the value is stored on the heap using a `std::shared_ptr`
that's managed by Variant. That's to say, when the Variant is copied, the value is not copied.
If you want the value be copied, use `Variant::clone`.  
Copying Variant is always trivial, there is not any memory allocation.  

desc*/
