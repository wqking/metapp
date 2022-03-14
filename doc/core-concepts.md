# Core concepts and mechanism

## TypeKind - type kind type

`metapp::TypeKind` is a 16 bit unsigned integer that represents the meta type kind. For example, `metapp::tkInt` is the meta type for `int`.  
Each meta type has one and only one TypeKind, different meta types may have the same TypeKind, that's to say, TypeKind is not unique. For example, `metapp::tkStdSharedPtr` represents all meta types of `std::shared_ptr<T>`, in which T can be any C++ type.  

See [TypeKind document](typekinds.md) for list of built-in type kinds.

## MetaType - the core meta type

`metapp::MetaType` is the core class to represent the meta type. Unlike some other reflection libraries which are meta class based, everything in metapp is meta type. A class is a meta type, an enum is a meta type, the same for functions, constructors, containers, etc.  
With MetaType, we can construct the underlying object, destroy the object, get object value, cast the type, streaming in/out the object, etc.  
A MetaType can be obtained at compile time using function `metapp::getMetaType()`, or at run time via class `MetaRepository`.  

Prototype of `getMetaType()`  
```c++
template <typename T>
const MetaType * getMetaType();
```

The pointer returned by `getMetaType()` is always the same for the same T. So,  
```c++
getMetaType<int>() == getMetaType<int>();
getMetaType<std::string>() == getMetaType<std::string>();
```

**Note**: MetaType is CV-aware (CV means const-volatile). That's to say, for the same T, different CV qualified types will result different MetaType. So,  
```c++
getMetaType<int>() != getMetaType<const int>();
getMetaType<std::string>() != getMetaType<volatile std::string>();
```

**Note**: `getMetaType()` can be used on any C++ type, the tye doesn't need to be registered to metapp.  

## UnifiedType -- CV-unaware meta type

MetaType member function `getUnifiedType()` can retrieve the underlying UnifiedType.  

Prototype of `getUnifiedType()`  
```c++
const void * getUnifiedType() const noexcept;
```

Each MetaType has one and only one UnifiedType. UnifiedType is similar to MetaType, except that UnifiedType is CV-unaware. That's to say, for the same T, no matter it's qualified with const or volatile, the UnifiedType is always the same. So,  
```c++
getMetaType<int>()->getUnifiedType() == getMetaType<const int>()->getUnifiedType();
getMetaType<std::string>()->getUnifiedType() == getMetaType<volatile std::string>()->getUnifiedType();
```
`UnifiedType` is an opaque type, that's why it's a `const void *`. It's function is to identify a type.  

Both MetaType and UnifiedType can be used to identify C++ type, they are the C++ `typeid` in metapp.  

## UpType - the most powerful concept in the meta type system

A MetaType has 0, 1, or multiple UpTypes. An UpType is a MetaType object.  
Below member functions in MetaType can be used to retrieve UpType.  

```c++
const MetaType * getUpType() const noexcept;
const MetaType * getUpType(const size_t i) const;
size_t getUpTypeCount() const noexcept;
```

A MetaType only has one TypeKind, so it can represent only one type information, it can't represent compound information. For example, for type `int *`, the TypeKind of the MetaType is `tkPointer`, the `int` type is missed. For `std::vector<char>`, the TypeKind of the MetaType is `tkStdVector`, the `char` type is missed.  
UpType is used to represent the missed type information.  

Here are some examples,  

MetaType `int *` has one UpType.    
```c++
metapp::getMetaType<int *>()->getTypeKind() == metapp::tkPointer;
metapp::getMetaType<int *>()->getUpTypeCount() == 1;
metapp::getMetaType<int *>()->getUpType()->getTypeKind() == metapp::tkInt;
```

MetaType `std::vector<char>` has one UpType.    
```c++
metapp::getMetaType<std::vector<char> >()->getTypeKind() == metapp::tkStdVector;
metapp::getMetaType<std::vector<char> >()->getUpTypeCount() == 1;
metapp::getMetaType<std::vector<char> >()->getUpType()->getTypeKind() == metapp::tkChar;
```

MetaType `void (int *)` has two UpTypes, the first is the result type `void`, the second is the parameter `int *`. Since the parameter is a pointer, the parameter also has an UpType.    
```c++
metapp::getMetaType<void (int *)>()->getTypeKind() == metapp::tkFunction;
metapp::getMetaType<void (int *)>()->getUpTypeCount() == 2;
metapp::getMetaType<void (int *)>()->getUpType(0)->getTypeKind() == metapp::tkVoid;
metapp::getMetaType<void (int *)>()->getUpType(1)->getTypeKind() == metapp::tkPointer;
metapp::getMetaType<void (int *)>()->getUpType(1)->getUpType()->getTypeKind() == metapp::tkInt;
```

MetaType `int` doesn't have any UpType.    
```c++
metapp::getMetaType<int>()->getTypeKind() == metapp::tkInt;
metapp::getMetaType<int>()->getUpTypeCount() == 0;
metapp::getMetaType<int>()->getUpType() == nullptr;
```

UpType represents complicated C++ type recursively. With UpType, metapp can represent any C++ type.

See [TypeKind document](typekinds.md) for the UpTypes for each TypeKind.

## Meta interface 

