# metapp performance benchmark

## Overview

This document contains some performance benchmark data for metapp. As a reference counterpart, performance of Qt meta system
is also tested.  

In brief, metapp performance is roughly similar to Qt meta system. The performance should be good enough for
most use cases. You may not use any reflection system in the render process in a console game, but you may want reflection
system for script binding in a console game, so metapp can be useful in high performance applications.  

This benchmark is not intended to compare performance between metapp and Qt, that doesn't make any sense. Qt is used as
a "reference" to give you a rough feeling on the performance, since Qt is so widely used, and there are not many widely
used reflection libraries for reference.

Note: the types used to benchmark are pretty primary, such as `int`, `double`, and string. This is because that there are
not much overlapped types supported by both metapp and Qt. So for more complicated types, the performance may be different.  

Note: the sample code in the document is only for demonstration purpose, it's not for compiling. There are compile-able code
in folder `metapp/tests/benchmark`. To build the benchmark, follow the unittests build instruction in root readme.

## Benchmark environment

**Hardware**  

HP laptop, Intel(R) Core(TM) i5-8300H CPU @ 2.30GHz, 16 GB RAM.

**Software**  

OS: Windows 10 Pro, 21H2  

C++ compiler for metapp: MinGW gcc version 11.3.0, optimization level is -O3  

C++ compiler for Qt: MinGW gcc version 11.3.0, optimization level is -O3  
Qt version: 5.12.10

## Benchmarks

### Variant constructing and assignment, with fundamental

10M iterations, metapp uses 806 ms, Qt uses 517 ms.  

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

### Variant constructing and assignment, with string

10M iterations, metapp uses 2529 ms, Qt uses 3623 ms.  

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

The performance looks like not good. Most time should be spent on the last assignment of `std::string` and `QString`, because
that will copy the string to the variant internal buffer.  

Tip: in metapp, for non-fundamental types, use `Variant of reference` when possible to avoid copying.

### Variant casting

10M iterations, metapp uses 831 ms, Qt uses 1752 ms.  

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

### Get accessible (Property)

10M iterations, metapp uses 263 ms, Qt uses 755 ms.  

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

### Set accessible (Property)

10M iterations, metapp uses 465 ms, Qt uses 383 ms.  

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

### Invoke method `int (int, int)` with argument `(int, int)`, no casting

10M iterations, metapp uses 744 ms, Qt uses 602 ms.  

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

When invoking Qt `QMetaMethod`, Qt doesn't very thin wrapper on the arguments. To my limited knowledge, Qt only gets arguments' addresses,
does basic type checking, and pass the arguments' addresses to the target method directly (that's why Qt requires the arguments types
must match the parameters types in the target method).  
For metapp, the arguments are the general `Variant`, there is more to do than Qt when passing `Variant` to the target method.  

### Invoke method `int (int, int)` with argument `(double, double)`, with casting

10M iterations, metapp uses 1358 ms, Qt uses 1034 ms.  

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

