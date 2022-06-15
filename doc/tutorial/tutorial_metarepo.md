[//]: # (Auto generated file, don't modify this file.)

# Tutorial for using MetaRepo and runtime meta data
<!--begintoc-->
- [Include headers](#mdtoc_e1acc517)
- [Define some C++ variables and types to reflect for](#mdtoc_8d30d5aa)
- [Register meta data](#mdtoc_7b306e77)
- [Use field meta data](#mdtoc_8cd7c280)
- [Use function meta data](#mdtoc_13307b6)
- [Use registered types](#mdtoc_eb7aec57)
- [Use registered nested repo](#mdtoc_c52fb9d2)
- [Use MetaItemView](#mdtoc_75bf69a5)
<!--endtoc-->

`MetaRepo` is used to register and retrieve meta types at running time.
The registered meta types can be any C++ types, no matter if they are declared with DeclareMetaType or not.   

<a id="mdtoc_e1acc517"></a>
## Include headers
Header for MetaRepo

```c++
#include "metapp/metarepo.h"
```

To use the built-in meta types, we must include "metapp/allmetatypes.h"

```c++
#include "metapp/allmetatypes.h"
```

<a id="mdtoc_8d30d5aa"></a>
## Define some C++ variables and types to reflect for

Global variable

```c++
std::vector<std::string> textList { "hello", "world" };
```

Global getter/setter

```c++
int value_ = 0;
void setValue(const int newValue)
{
  value_ = newValue;
}

int getValue()
{
  return value_;
}
```

Global function

```c++
std::string concat(const int a, const bool b, const std::string & c)
{
  return std::to_string(a)
    + (b ? "true" : "false")
    + c
  ;
}
```

Global class

```c++
class MyClass
{
};
```

Define the global variable `metaRepo`, we will register all global meta data there.

```c++
metapp::MetaRepo metaRepo;
```

<a id="mdtoc_7b306e77"></a>
## Register meta data

To get meta data from MetaRepo, we must register meta data first.  

Register an accessible. An accessible is any meta type that implements meta interface `MetaAccessible`.  
The second argument in `registerAccessible` is a `metapp::Variant`. Any data can be casted to `metapp::Variant`
implicitly, so we don't need to specify `metapp::Variant` explicitly.

```c++
  metaRepo.registerAccessible("textList", &textList);
  metaRepo.registerAccessible("value", metapp::createAccessor<int>(&getValue, &setValue));
```

Register an callable. A callable is any meta type that implements meta interface `MetaCallable`.  
A callable can be a pointer to function, pointer to member function, or std::function, and so on.  

```c++
  metaRepo.registerCallable("concat", &concat);
```

Register a C++ type, here is class MyClass.  
This is only to demonstrate how to use registered meta type. There is a separate completed tutorial for how to use MetaClass.  

```c++
  metaRepo.registerType("MyClass", metapp::getMetaType<MyClass>());
```

Register a nested MetaRepo, we can use it to simulate namespace.  

```c++
  metapp::MetaRepo nestedRepo;
  nestedRepo.registerVariable("one", 1);
  // For best efficiency, we move the `nestedRepo` into `metaRepo` to avoid copying.
  // `nestedRepo` can also be std::shared_ptr, or std::unique_ptr.
  // `nestedRepo` can also be raw pointer or reference to MetaRepo, in such case, the caller must ensure 
  // `nestedRepo` lives as long as `metaRepo`, otherwise `metaRepo` will hold dangling pointer.
  metaRepo.registerRepo("myNamespace", std::move(nestedRepo));
```

<a id="mdtoc_8cd7c280"></a>
## Use field meta data

Get the meta data of field "value"

```c++
metapp::MetaItem fieldValue = metaRepo.getAccessible("value");
```

Call metapp::accessibleGet to get the value of the field. The first parameter is the Variant.  
Call getTarget() to get the underlying Variant.

```c++
ASSERT(metapp::accessibleGet(fieldValue.asAccessible(), nullptr).get<int>() == 0);
```

getTarget() can also be omitted, the MetaItem can convert to Variant automatically

```c++
ASSERT(metapp::accessibleGet(fieldValue, nullptr).get<int>() == 0);
```

Now let's set a new value

```c++
metapp::accessibleSet(fieldValue, nullptr, 5);
ASSERT(getValue() == 5);
ASSERT(metapp::accessibleGet(fieldValue, nullptr).get<int>() == 5);
```

Now append some new texts to textList

```c++
metapp::MetaItem fieldtextList = metaRepo.getAccessible("textList");
metapp::accessibleGet(fieldtextList, nullptr).get<std::vector<std::string> &>().push_back("good");
ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[0] == "hello");
ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[1] == "world");
ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[2] == "good");
```

<a id="mdtoc_13307b6"></a>
## Use function meta data

Get the meta data of method "concat".

```c++
metapp::MetaItem methodConcat = metaRepo.getCallable("concat");
```

Call metapp::callableInvoke to invoke the method, and pass the arguments.  
The return value is a metapp::Variant.

```c++
metapp::Variant result = metapp::callableInvoke(methodConcat, nullptr, 38, true, "Great");
ASSERT(result.get<const std::string &>() == "38trueGreat");
```

<a id="mdtoc_eb7aec57"></a>
## Use registered types

```c++
  metapp::MetaItem myClassType = metaRepo.getType("MyClass");
  ASSERT(myClassType.asMetaType() == metapp::getMetaType<MyClass>());
```

<a id="mdtoc_c52fb9d2"></a>
## Use registered nested repo

```c++
  const auto repoItem = metaRepo.getRepo("myNamespace");
  const metapp::MetaRepo * nestedRepo = repoItem.asMetaRepo();
  const auto one = nestedRepo->getVariable("one");
  ASSERT(one.asVariable().get<int>() == 1);
```

<a id="mdtoc_75bf69a5"></a>
## Use MetaItemView

```c++
  // Get all registered accessible as MetaItemView
  const metapp::MetaItemView itemView = metaRepo.getAccessibleView();
  // We registered two accessibles
  ASSERT(itemView.size() == 2);
  // The first accessible is textList
  const metapp::MetaItem & firstItem = itemView[0];
  ASSERT(firstItem.getName() == "textList");
  // The second is value
  ASSERT(itemView[1].getName() == "value");

  // We can also use range based for loop
  std::string allNames = "";
  for(const metapp::MetaItem & item : itemView) {
    allNames += item.getName();
  }
  ASSERT(allNames == "textListvalue");
```
