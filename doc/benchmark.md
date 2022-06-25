[//]: # (Auto generated file, don't modify this file.)

# metapp performance benchmark
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [How is performance optimized in metapp](#mdtoc_9e7c67a5)
- [Benchmark environment](#mdtoc_2af9f9b6)
- [Benchmarks](#mdtoc_41bc1c58)
  - [Variant constructing and assignment, with fundamental](#mdtoc_9d4f2922)
  - [Variant constructing and assignment, with string](#mdtoc_ad1c37e3)
  - [Variant constructing and assignment, with heavy copy but trivial move object](#mdtoc_38682910)
  - [Variant casting](#mdtoc_3d72fa05)
  - [Get accessible (Property)](#mdtoc_d2467693)
  - [Set accessible (Property)](#mdtoc_d368514c)
  - [Invoke method `void ()`](#mdtoc_3a32abb0)
  - [Invoke method `int (int, int)` with argument `(int, int)`, no casting](#mdtoc_d289b5b9)
  - [Invoke method `int (int, int)` with argument `(double, double)`, with casting](#mdtoc_e989de5b)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

This document contains some performance benchmark data for metapp. As a reference counterpart, performance of Qt meta system
is also tested.  

In brief, metapp performance is roughly similar to or better than Qt meta system. The performance should be good enough for
most use cases. You may not use any reflection system in the render process in a console game, but you may want reflection
system for script binding in a console game, so metapp can be useful in high performance applications.  

This benchmark is not intended to compare performance between metapp and Qt, that doesn't make any sense. Qt is used as
a "reference" to give you a rough feeling on the performance, since Qt is so widely used, and there are not many widely
used reflection libraries for reference.

Note: the types used to benchmark are pretty primary, such as `int`, `double`, and string. This is because that there are
not much overlapped types supported by both metapp and Qt. So for more complicated types, the performance may be different.  

Note: the sample code in the document is only for demonstration purpose, it's not for compiling. There are compile-able code
in folder `metapp/tests/benchmark`. To build the benchmark, follow the unittests build instruction in root readme.

<a id="mdtoc_9e7c67a5"></a>
## How is performance optimized in metapp

Optimization is continuously done on metapp. Here lists several optimizations that have done.  

1. The arithmetic meta types use O(1) algorithm for casting instead of O(N) linear search algorithm in `metapp::CastToTypes`.  
2. `MetaType::equal` related code is optimized. It's the very core and very frequently called function.
3. `Variant` constructors and assignments support universal reference and move semantic.
4. When constructing data in `Variant`, now using `std::make_shared` instead previously raw `new` to reduce extra heap allocations.
5. When constructing and copying `Variant` from compile time data type, compiling time constructing function is used instead of relying on runtime meta type. That not only increases `Variant` constructing performance, but also enables the possibilities for the compiler to inline and optimize out the code.

Those optimizations, and others, have improved the performance significantly.

<a id="mdtoc_2af9f9b6"></a>
## Benchmark environment

**Hardware**  

HP laptop, Intel(R) Core(TM) i5-8300H CPU @ 2.30GHz, 16 GB RAM.

**Software**  

OS: Windows 10 Pro, 21H2  

C++ compiler for metapp: MinGW gcc version 11.3.0, optimization level is -O3  

C++ compiler for Qt: MinGW gcc version 11.3.0, optimization level is -O3  
Qt version: 5.12.10

<a id="mdtoc_41bc1c58"></a>
## Benchmarks

<a id="mdtoc_9d4f2922"></a>
### Variant constructing and assignment, with fundamental

10M iterations, metapp uses 16 ms, Qt uses 517 ms.  

Code for metapp

```c++
for(int i = 0; i < iterations; ++i) {
  metapp::Variant v = 5;
  v = 38.0;
  v = (long long)38;
  v = (unsigned short)9;
  v = true;
  v = 1.5f;
}
```

Code for Qt

```c++
for(int i = 0; i < iterations; ++i) {
  QVariant v = 5;
  v = 38.0;
  v = (long long)38;
  v = (unsigned short)9;
  v = true;
  v = 1.5f;
}
```

**Remarks**  

Since [the optimization in this commit](https://github.com/wqking/metapp/commit/382817969dac3cc9e61d90bda8cda73b8f274800),
now constructing and copying Variant with template value (here is fundamental value) can be inlined and the code
may be optimized out, that's why metapp is so fast.

<a id="mdtoc_ad1c37e3"></a>
### Variant constructing and assignment, with string

10M iterations, metapp uses 1153 ms, Qt uses 3623 ms.  

Code for metapp

```c++
for(int i = 0; i < iterations; ++i) {
  metapp::Variant v = 5;
  v = 38.0;
  v = "abc";
  v = std::string("def");
}
```

Code for Qt

```c++
for(int i = 0; i < iterations; ++i) {
  QVariant v = 5;
  v = 38.0;
  v = "abc";
  v = QString("def");
}
```

**Remarks**  

In metapp, the statement `v = "abc"` will copy the char array to Variant internal buffer. The statement `v = std::string("def")` will
move the string to Variant internal buffer since it's a rvalue, but constructing and destroy the `std::string` will use a lot of time,
though it's not related to metapp.

<a id="mdtoc_38682910"></a>
### Variant constructing and assignment, with heavy copy but trivial move object

10M iterations, metapp uses 2245 ms, Qt uses 6908 ms.  

Code for metapp

```c++
struct HeavyCopy
{
  HeavyCopy() : value(0) {}

  HeavyCopy(const HeavyCopy & other) : value(other.value) {
    doHeavyWork();
  }

  HeavyCopy & operator = (const HeavyCopy & other) {
    value = other.value;
    doHeavyWork();
    return *this;
  }

  HeavyCopy(HeavyCopy && other) noexcept : value(other.value) {
  }

  HeavyCopy & operator = (HeavyCopy && other) noexcept {
    value = other.value;
    return *this;
  }

  void doHeavyWork() {
    std::vector<int> dataList(128, 6);
    for(const int item : dataList) {
      value += item;
    }
  }
  int value;
};

for(int i = 0; i < iterations; ++i) {
  metapp::Variant v = HeavyCopy();
  v = 5;
  v = HeavyCopy();
}
```

Code for Qt

```c++
Q_DECLARE_METATYPE(HeavyCopy)

for(int i = 0; i < iterations; ++i) {
  QVariant v = QVariant::fromValue(HeavyCopy());
  v = 5;
  v = QVariant::fromValue(HeavyCopy());
}
```

**Remarks**  

`HeavyCopy` is a class that copy constructor and assignment are heavy and move constructor and assignment are trivial.  
`std::vector` has similar characteristic. The reason not using `std::vector` here is that `std::vector` will do heap
allocation on constructing, that will take a lot of extra time (about 2500 ms extra time on both metapp and Qt) and that's
not related to the benchmark. `HeavyCopy` acts as a `std::vector` without heap allocation.  

Note metapp needs about 2 seconds to finish this benchmark, most time is spent on allocate `HeavyCopy` on the heap.

<a id="mdtoc_3d72fa05"></a>
### Variant casting

10M iterations, metapp uses 573 ms, Qt uses 1752 ms.  

Code for metapp

```c++
metapp::Variant v = 5;
for(int i = 0; i < iterations; ++i) {
  v.cast<double>();
  v.cast<long long>();
  v.cast<int>();
}
```

Code for Qt

```c++
QVariant v = 5;
for(int i = 0; i < iterations; ++i) {
  v.convert(QMetaType::Double);
  v.convert(QMetaType::LongLong);
  v.convert(QMetaType::Int);
}
```

<a id="mdtoc_d2467693"></a>
### Get accessible (Property)

10M iterations, metapp uses 241 ms, Qt uses 755 ms.  

Code for metapp

```c++
struct TestClass
{
  int value;
};

metapp::Variant v = &TestClass::value;
TestClass obj;
metapp::Variant instance = &obj;
const metapp::MetaAccessible * metaAccessible = v.getMetaType()->getMetaAccessible();
for(int i = 0; i < iterations; ++i) {
  metaAccessible->get(v, instance);
}
```

Code for Qt

```c++
class TestClass : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE int add(int a, int b) {
        return a + b;
    }

    Q_PROPERTY(int value MEMBER value)

    int value;
};

TestClass obj;
const int index = obj.metaObject()->indexOfProperty("value");
QMetaProperty property = obj.metaObject()->property(index);
for(int i = 0; i < iterations; ++i) {
  property.read(&obj);
}
```

<a id="mdtoc_d368514c"></a>
### Set accessible (Property)

10M iterations, metapp uses 462 ms, Qt uses 383 ms.  

Code for metapp

```c++
metapp::Variant v = &TestClass::value;
TestClass obj;
metapp::Variant instance = &obj;
const metapp::MetaAccessible * metaAccessible = v.getMetaType()->getMetaAccessible();
for(int i = 0; i < iterations; ++i) {
  metaAccessible->set(v, instance, i);
}
```

Code for Qt

```c++
TestClass obj;
const int index = obj.metaObject()->indexOfProperty("value");
QMetaProperty property = obj.metaObject()->property(index);
constexpr int iterations = generalIterations;
for(int i = 0; i < iterations; ++i) {
  property.write(&obj, i);
}
```

<a id="mdtoc_3a32abb0"></a>
### Invoke method `void ()`

10M iterations, metapp uses 213 ms, Qt uses 433 ms.  

Code for metapp

```c++
struct TestClass
{
  void nothing()
  {
  }
};

metapp::Variant v = &TestClass::nothing;
TestClass obj;
metapp::Variant instance = &obj;
const metapp::MetaCallable * metaCallable = v.getMetaType()->getMetaCallable();
for(int i = 0; i < iterations; ++i) {
  metaCallable->invoke(v, instance, {});
}
```

Code for Qt

```c++
TestClass obj;
QByteArray normalizedSignature = QMetaObject::normalizedSignature("nothing()");
const int index = obj.metaObject()->indexOfMethod(normalizedSignature);
QMetaMethod method = obj.metaObject()->method(index);
constexpr int iterations = generalIterations;
for(int i = 0; i < iterations; ++i) {
  method.invoke(&obj, Qt::DirectConnection);
}
```

<a id="mdtoc_d289b5b9"></a>
### Invoke method `int (int, int)` with argument `(int, int)`, no casting

10M iterations, metapp uses 551 ms, Qt uses 602 ms.  

Code for metapp

```c++
struct TestClass
{
  int add(const int a, const int b)
  {
    return a + b;
  }
};

metapp::Variant v = &TestClass::add;
TestClass obj;
metapp::Variant instance = &obj;
const metapp::MetaCallable * metaCallable = v.getMetaType()->getMetaCallable();
for(int i = 0; i < iterations; ++i) {
  metapp::Variant arguments[] { i, i + 1 };
  metaCallable->invoke(v, instance, arguments);
}
```

Code for Qt

```c++
TestClass obj;
QByteArray normalizedSignature = QMetaObject::normalizedSignature("add(int, int)");
const int index = obj.metaObject()->indexOfMethod(normalizedSignature);
QMetaMethod method = obj.metaObject()->method(index);
int result = 0;
for(int i = 0; i < iterations; ++i) {
  method.invoke(&obj, Qt::DirectConnection, Q_RETURN_ARG(int, result), Q_ARG(int, i), Q_ARG(int, i + 1));
}
```

**Remarks**  

When invoking Qt `QMetaMethod`, Qt does very thin wrapper on the arguments. To my limited knowledge, Qt only gets arguments' addresses,
does basic type checking, and passes the arguments' addresses to the target method directly (that's why Qt requires the arguments types
must match the parameters types in the target method).  
For metapp, the arguments are the general `Variant`, there is more to do than Qt when passing `Variant` to the target method. metapp needs
to create Variant from the argument (in the code it's `i` and `i+1`), then convert the Variant back to the target arguments (in the code
it's `int` and `int`), then invoke the callable.

<a id="mdtoc_e989de5b"></a>
### Invoke method `int (int, int)` with argument `(double, double)`, with casting

10M iterations, metapp uses 938 ms, Qt uses 1034 ms.  

Code for metapp

```c++
metapp::Variant v = &TestClass::add;
TestClass obj;
metapp::Variant instance = &obj;
const metapp::MetaCallable * metaCallable = v.getMetaType()->getMetaCallable();
for(int i = 0; i < iterations; ++i) {
  metapp::Variant arguments[] { (double)i, (double)i + 1.0 };
  metaCallable->invoke(v, instance, arguments);
}
```

Code for Qt

```c++
TestClass obj;
QByteArray normalizedSignature = QMetaObject::normalizedSignature("add(int, int)");
const int index = obj.metaObject()->indexOfMethod(normalizedSignature);
QMetaMethod method = obj.metaObject()->method(index);
int result = 0;
for(int i = 0; i < iterations; ++i) {
  QVariant a((double)i);
  QVariant b((double)i + 1.0);
  method.invoke(&obj, Qt::DirectConnection, Q_RETURN_ARG(int, result), Q_ARG(int, a.toInt()), Q_ARG(int, b.toInt()));
}
```

