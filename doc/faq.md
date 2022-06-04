[//]: # (Auto generated file, don't modify this file.)

# Infrequently Asked Questions
<!--begintoc-->
* [Variant](#a2_1)
  * [How can I convert a Variant of pointer to non-pointer?](#a3_1)
  * [What's the difference between metapp::Variant and std::variant?](#a3_2)
  * [What's the difference between metapp::Variant and std::any?](#a3_3)
  * [When a Variant holds a container such as std::vector or std::map, isn't convenient to use array operator [] to access the elements?](#a3_4)
* [MetaType](#a2_2)
  * [metapp doesn't recognize the built-in types such as int, it gives type kind `tkObject` instead of `tkInt` to `int`.](#a3_5)
  * [When getting meta interface from a MetaType, I need to check if it's reference and skip the reference, that's cumbersome.](#a3_6)
  * [Why can't I get the type name from MetaType?](#a3_7)
* [Miscellaneous](#a2_3)
  * [What's the difference between metapp and the compile time reflection feature in coming C++ standard?](#a3_8)
  * [To register meta data in MetaRepo and MetaClass, there are different functions such as registerAccessible, registerCallable, registerConstructor, etc, can't we make a single registering function that detects the meta data type automatically?](#a3_9)
<!--endtoc-->

<a id="a2_1"></a>
## Variant

<a id="a3_1"></a>
### How can I convert a Variant of pointer to non-pointer?

The first argument in some meta interface functions, such as `MetaAccessible::get/set`, `MetaCallable::invoke`, the first argument
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

<a id="a3_2"></a>
### What's the difference between metapp::Variant and std::variant?

`std::variant` requires you to know the contained types beforehand. You don't need to know any type when using `metapp::Variant`.  
`std::variant` can only contain data for limit types that you specified. `metapp::Variant` can contain data in infinite types.

<a id="a3_3"></a>
### What's the difference between metapp::Variant and std::any?

`std::any` only contains type of `std::type_info` which has very limited information. `metapp::Variant` contains `metapp::MetaType` which
has much richer information than `std::type_info`.  
If you don't know what type a `std::any` contains, you have to guess, or trial and error by checking `std::type_info`. If you don't
know what type a `metapp::Variant` contains, you can inspect the `metapp::MetaType` for almost every aspects of a C++ type. And even
better, you can use meta interface to perform runtime generic operations without knowing the underlying type.  

<a id="a3_4"></a>
### When a Variant holds a container such as std::vector or std::map, isn't convenient to use array operator [] to access the elements?

For example, `someVariant[1] = 5`.  
The problem is, metapp supports `MetaIndexable` and `MetaMappable`, both of which allow to array style access.  
To support the `[]` operator, `Variant` has to try to get `MetaMappable` at first, then if not it tries to get `MetaIndexable`.
Such kinds of "trials", or to say, "guess", is not only having performance cost, but also is error prone and the user may
abuse using the `[]` operator.  
Also, most likely metapp will support operators in meta type in the future, then that will conflict if `Variant` supports `[]`
specially for now.

<a id="a2_2"></a>
## MetaType

<a id="a3_5"></a>
### metapp doesn't recognize the built-in types such as int, it gives type kind `tkObject` instead of `tkInt` to `int`.

Your forget to `#include "metapp/allmetatypes.h"`. The header needs to be included where ever `metapp` is used.

<a id="a3_6"></a>
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

<a id="a3_7"></a>
### Why can't I get the type name from MetaType?

There are various reasons that there is no type name in `metapp::MetaType`,

1. A type is a type, no matter it has name or not. A type is used to define what a value can do, a name has nothing for that function.  
2. A type can have more than one names. In native C++ we can use `typedef` or `using` to create type alias.  
3. Storing string name in `metapp::MetaType` will bloat the binary and memory size dramatically.  
4. We usually use type name to recognize a type. We can use `TypeKind` and `MetaType::equal` to achieve the same purpose in metapp.  

<a id="a2_3"></a>
## Miscellaneous

<a id="a3_8"></a>
### What's the difference between metapp and the compile time reflection feature in coming C++ standard?

metapp is runtime reflection. A program runs almost all time at "running time". With metapp, we can
get type information from dynamical libraries, or bind meta data to script engine, or get property data
from external file to use in a GUI property editor. Those are not easy to do at compile time.  

<a id="a3_9"></a>
### To register meta data in MetaRepo and MetaClass, there are different functions such as registerAccessible, registerCallable, registerConstructor, etc, can't we make a single registering function that detects the meta data type automatically?

Yes it's possible, we can make a function, such as `versatileRegister`, then we can write,  

```c++
mc.versatileRegister(&MyClass::value); // register as accessible
mc.versatileRegister(&MyClass::getName); // register as callable
```

The problem is, it's quite confusing and error prone. How to register a Variant that implements both MetaCallable and MetaAccessible?
`versatileRegister` may register it in wrong type without noticeable error.  
The API should be clear, less confusing, less implicitly operations.
