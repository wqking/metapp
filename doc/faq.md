[//]: # (Auto generated file, don't modify this file.)

# Infrequently Asked Questions
<!--begintoc-->
- [Variant](#mdtoc_f143bfad)
  - [How to convert a Variant of pointer to non-pointer?](#mdtoc_35ab6d9b)
  - [How to convert an integer to a Variant of enum without knowing the enum native type?](#mdtoc_9482bca6)
  - [What's the difference between metapp::Variant and std::variant?](#mdtoc_56a306)
  - [What's the difference between metapp::Variant and std::any?](#mdtoc_1beb8c3a)
  - [When a Variant holds a container such as std::vector or std::map, isn't convenient to use array operator [] to access the elements?](#mdtoc_70965496)
- [MetaType](#mdtoc_6e39905e)
  - [metapp doesn't recognize the built-in types such as int, it gives type kind `tkObject` instead of `tkInt` to `int`.](#mdtoc_6bf5fe4d)
  - [When getting meta interface from a MetaType, I need to check if it's reference and skip the reference, that's cumbersome.](#mdtoc_2f0fb23d)
  - [Why can't I get the type name from MetaType?](#mdtoc_d00ee4b9)
- [Miscellaneous](#mdtoc_427b45fe)
  - [What's the difference between metapp and the compile time reflection feature in coming C++ standard?](#mdtoc_5e7363e0)
  - [Declaring meta type manually is boring!](#mdtoc_3a7bbcef)
  - [To register meta data in MetaRepo and MetaClass, there are different functions such as registerAccessible, registerCallable, registerConstructor, etc, can't we make a single registering function that detects the meta data type automatically?](#mdtoc_2919fd91)
<!--endtoc-->

<a id="mdtoc_f143bfad"></a>
## Variant

<a id="mdtoc_35ab6d9b"></a>
### How to convert a Variant of pointer to non-pointer?

In some meta interface functions, such as `MetaAccessible::get/set`, `MetaCallable::invoke`, the first argument
is a Variant that requires to be either value or reference, but not pointer.
If we have a pointer and want to convert it to non-pointer so that we can pass it to those functions, we can
call function `depointer` in `utility.h` to covert it to equivalent non-pointer. For example,  

```c++
int n = 5;
// pn is a pointer to n
metapp::Variant pn = &n;
// Convert pn to non-pointer. Now rn is a referent to n.
metapp::Variant rn = metapp::depointer(pn);
REQUIRE(rn.get<int>() == 5);
// Since rn is a reference, changing n will also change rn.
n = 38;
REQUIRE(rn.get<int>() == 38);
```

<a id="mdtoc_9482bca6"></a>
### How to convert an integer to a Variant of enum without knowing the enum native type?

```c++
const metapp::MetaType * enumMetaType = what ever meta type of an enum;
const long long value = 123;
```

Assume we have an `enumMetaType` of an enum, we only know it's an enum but not knowing the C++ type nor the enum underlying type, and
we have an integer `value` such as a `long long`, how to convert the value to a Variant of enum?

Method 1, bad and wrong method

```c++
metapp::Variant enumVar(enumMetaType, &value);
```

This method copies the value directly to the enum. It may works in most cases, until,
1. The program runs on a computer with big endian.
2. Or the integer binary representation is sign magnitude instead of 2's Complement. I don't know which C++ compiler produces sign magnitude,
but C++ standard allows it.

Method 2, correct method

```c++
metapp::Variant enumVar = metapp::Variant(value).cast(enumMetaType);
```

It's slower than method 1, but it guarantees to generate the correct result on all platforms.

<a id="mdtoc_56a306"></a>
### What's the difference between metapp::Variant and std::variant?

`std::variant` requires you to know the contained types beforehand. You don't need to know any type when using `metapp::Variant`.  
`std::variant` can only contain data for limit types that you specified. `metapp::Variant` can contain data in infinite types.

<a id="mdtoc_1beb8c3a"></a>
### What's the difference between metapp::Variant and std::any?

`std::any` only contains type of `std::type_info` which has very limited information. `metapp::Variant` contains `metapp::MetaType` which
has much richer information than `std::type_info`.  
If you don't know what type a `std::any` contains, you have to guess, or trial and error by checking `std::type_info`. If you don't
know what type a `metapp::Variant` contains, you can inspect the `metapp::MetaType` for almost every aspects of a C++ type. And even
better, you can use meta interface to perform runtime generic operations without knowing the underlying type.  

<a id="mdtoc_70965496"></a>
### When a Variant holds a container such as std::vector or std::map, isn't convenient to use array operator [] to access the elements?

For example, `someVariant[1] = 5`.  
The problem is, metapp supports `MetaIndexable` and `MetaMappable`, both of which allow to array style access.  
To support the `[]` operator, `Variant` has to try to get `MetaMappable` at first, then if not it tries to get `MetaIndexable`.
Such kinds of "trials", or to say, "guess", is not only having performance cost, but also is error prone and the user may
abuse using the `[]` operator.  
Also, most likely metapp will support operators in meta type in the future, then that will conflict if `Variant` supports `[]`
specially for now.

<a id="mdtoc_6e39905e"></a>
## MetaType

<a id="mdtoc_6bf5fe4d"></a>
### metapp doesn't recognize the built-in types such as int, it gives type kind `tkObject` instead of `tkInt` to `int`.

Your forget to `#include "metapp/allmetatypes.h"`. The header needs to be included where ever `metapp` is used.

<a id="mdtoc_2f0fb23d"></a>
### When getting meta interface from a MetaType, I need to check if it's reference and skip the reference, that's cumbersome.

The utility function `getNonReferenceMetaType` is our friend. The function will get the referred type if the meta type
is a reference, otherwise it returns the meta type directly. Example code,

```c++
//Include the header for `getNonReferenceMetaType`
#include "metapp/utilities/utility.h"
```

```c++
const metapp::MetaType * metaType = metapp::getMetaType<Foo>();

// Good
const metapp::MetaClass * metaClass = metapp::getNonReferenceMetaType(metaType)->getMetaClass();

// Bad, it's verbose and the logic is duplicated everywhere
const metapp::MetaClass * metaClass;
if(metaType->isReference()) {
  metaClass = metaType->getUpType()->getMetaClass();
}
else {
  metaClass = metaType->getMetaClass();
}

// Worse, don't do it. There is no MetaClass on a reference.
const metapp::MetaClass * metaClass = metaType->getMetaClass();
```

<a id="mdtoc_d00ee4b9"></a>
### Why can't I get the type name from MetaType?

There are various reasons that there is no type name in `metapp::MetaType`,

1. A type is a type, no matter it has name or not. A type is used to define what a value can do, a name has nothing for that function.  
2. A type can have more than one names. In native C++ we can use `typedef` or `using` to create type alias.  
3. Storing string name in `metapp::MetaType` will bloat the binary and memory size dramatically.  
4. We usually use type name to recognize a type. We can use `TypeKind` and `MetaType::equal` to achieve the same purpose in metapp.  

<a id="mdtoc_427b45fe"></a>
## Miscellaneous

<a id="mdtoc_5e7363e0"></a>
### What's the difference between metapp and the compile time reflection feature in coming C++ standard?

metapp is runtime reflection. A program runs almost all time at "running time". With metapp, we can
get type information from dynamical libraries, or bind meta data to script engine, or get property data
from external file to use in a GUI property editor. Those are not easy to do at compile time.  

<a id="mdtoc_3a7bbcef"></a>
### Declaring meta type manually is boring!

I agree! I believe there should be a tool to generate the meta types from source code automatically.
Ideally the tool should be Clang based. (In my old cpgf library, there was a Doxygen based tool that works pretty well).  
Since I'm not Clang expert, I wish someone else can start such a project then let's develop it together.  

<a id="mdtoc_2919fd91"></a>
### To register meta data in MetaRepo and MetaClass, there are different functions such as registerAccessible, registerCallable, registerConstructor, etc, can't we make a single registering function that detects the meta data type automatically?

Yes it's possible, we can make a function, such as `versatileRegister`, then we can write,  

```c++
mc.versatileRegister(&MyClass::value); // register as accessible
mc.versatileRegister(&MyClass::getName); // register as callable
```

The problem is, it's quite confusing and error prone. How to register a Variant that implements both MetaCallable and MetaAccessible?
`versatileRegister` may register it in wrong type without noticeable error.  
The API should be clear, less confusing, less implicitly operations.

