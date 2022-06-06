[//]: # (Auto generated file, don't modify this file.)

# Tutorial for using Variant

`metapp::Variant` allows to store data of any type and convert between types.  
Variant holds a single value of any type at the same time.
The type can be any C++ type, such as int, class, std::string, std::vector, function pointer, etc, any type.  

## Include headers
Include the header for metapp::Variant

```c++
#include "metapp/variant.h"
```

To use all declared meta types, include this header

```c++
#include "metapp/allmetatypes.h"
```

## Use Variant
v contains int.

```c++
metapp::Variant v(5);
ASSERT(v.get<int>() == 5);
```

We can get as reference, to avoid copy the value.

```c++
ASSERT(v.get<int &>() == 5);
ASSERT(v.get<const int &>() == 5);
```

We can even get as fancy const volatile reference.

```c++
ASSERT(v.get<const volatile int &>() == 5);
```

Now v contains std::string.

```c++
v = std::string("hello");
```

Get as std::string will copy the value, that's inefficient.  
We should get as reference.

```c++
ASSERT(v.get<std::string>() == "hello");
```

Get as reference to avoid copy.

```c++
ASSERT(v.get<const std::string &>() == "hello");
```

Whether the reference is const, it doesn't matter.

```c++
ASSERT(v.get<std::string &>() == "hello");
v.get<std::string &>() = "world";
ASSERT(v.get<const std::string &>() == "world");
```

Cast to const char *.

```c++
metapp::Variant casted = v.cast<const char *>();
const char * s = casted.get<const char *>();
ASSERT(strcmp(s, "world") == 0);
```

Now v contains char *.

```c++
v = "great";
ASSERT(strcmp(v.get<const char *>(), "great") == 0);
```

Let's see how Variant works with array

```c++
int array[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
```

Now v contains reference to int[2][3].
We can't simply assign array to v because the array type will be lost.
We need to call Variant::create to retain the array type.

```c++
v = metapp::Variant::create<int (&)[2][3]>(array);
ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
```

Since v is a reference to array, modify array will also modify v

```c++
array[1][2] = 10;
ASSERT(v.get<int (&)[2][3]>()[1][2] == 10);
```

Now we copy array into v.

```c++
int anotherArray[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
v = metapp::Variant::create<int [2][3]>(anotherArray);
ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
```

Since v is a copy of anotherArray, modify anotherArray will not affect v.

```c++
anotherArray[1][2] = 10;
ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
```
