### Use Variant
Header for Variant

```c++
#include "metapp/variant.h"
```

To use all declared meta types, include this header

```c++
#include "metapp/allmetatypes.h"
```

v contains int.

```c++
metapp::Variant v(5);
```

Get the value

```c++
ASSERT(v.get<int>() == 5);
```

Now v contains std::string.

```c++
metapp::Variant v = std::string("hello");
```

Get as reference to avoid copy.

```c++
ASSERT(v.get<const std::string &>() == "hello");
```

Cast to const char *.

```c++
metapp::Variant casted = v.cast<const char *>();
const char * s = casted.get<const char *>();
ASSERT(strcmp(s, "hello") == 0);
```

### Use MetaType
int

```c++
const metapp::MetaType * metaType = metapp::getMetaType<int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
```

constness

```c++
auto metaType = metapp::getMetaType<const int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
ASSERT(metaType->isConst());
```

### Call function

```c++
struct MyClass {
  int value;

  int add(const int delta1, const int delta2) const {
    return value + delta1 + delta2;
  }
};

metapp::Variant v(&MyClass::add);
MyClass obj { 5 };
// The second argument is the pointer to obj, it's required when invoking member function
metapp::Variant result = metapp::callableInvoke(v, &obj, 3, 9);
ASSERT(result.get<int>() == 17);
```
