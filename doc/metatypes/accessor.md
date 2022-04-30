[//]: # (Auto generated file, don't modify this file.)

# Meta type Accessor (tkAccessor)

Template `metapp::Accessor` is used to create accessible (MetaAccessible) with getter/setter or other advanced feature.  
If you don't need getter/setter, and only need variable pointer or member data pointer, you don't need `Accessor`.  

`metapp::Accessor` is an alias of `accessorpp::Accessor`. metapp supports `accessorpp::Accessor` directly.  
For more detailed documentations, please see documents in [accessorpp library](https://github.com/wqking/accessorpp).
accessorpp library is developed by the same developer (wqking) of metapp.  
Note you don't need install accessorpp to use metapp. metapp has bundled accessorpp source code.  

This document will show the basic usage of `Accessor`.  
We can use below functions to create `Accessor`,  

```c++
//form 1
template <typename T, typename G, typename S>
Accessor<T> createAccessor(G && getter, S && setter);

//form 2
template <typename G, typename S>
auto createAccessor(G && getter, S && setter);

//form 3
template <typename T, typename G>
Accessor<T> createReadOnlyAccessor(G && getter);

//form 4
template <typename G>
auto createReadOnlyAccessor(G && getter);
```

Form 1 creates an `Accessor` of `T`. Form 2 is similar to form 1, except `T` is deduced.
Form 3 creates a read-only `Accessor` of `T`. Form 4 is similar to form 3, except `T` is deduced.

Example code

We have this class

```c++
struct DaClass
{
  DaClass() : value() {}

  // getter
  int getValue() const {
    return value;
  }

  // setter
  void setValue(const int n) {
    value = n;
  }

  // member data
  int value;
};
```

```c++
DaClass obj;
```

Create an accessible using createAccessor

```c++
metapp::Variant accessible1 = (metapp::createAccessor(&DaClass::getValue, &DaClass::setValue));
ASSERT(metapp::accessibleGet(accessible1, &obj).get<int>() == 0);
metapp::accessibleSet(accessible1, &obj, 5);
ASSERT(metapp::accessibleGet(accessible1, &obj).get<int>() == 5);
ASSERT(obj.value == 5);
```