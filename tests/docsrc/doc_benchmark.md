# metapp performance benchmark

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

Note: the data was revised on Jul.14, 2022. The previous data was tested with CPU power set to 96% to avoid overheat. After
changing it to 100%, the performance data is almost doubled. No code improvement, only test environment was changed in the revision.

## How is performance optimized in metapp

Optimization is continuously done on metapp. Here lists several optimizations that have done.  

1. The arithmetic meta types use O(1) algorithm for casting instead of O(N) linear search algorithm in `metapp::CastToTypes`.  
2. `MetaType::equal` related code is optimized. It's the very core and very frequently called function.
3. `Variant` constructors and assignments support universal reference and move semantic.
4. When constructing data in `Variant`, now using `std::make_shared` instead previously raw `new` to reduce extra heap allocations.
5. When constructing and copying `Variant` from compile time data type, compiling time constructing function is used instead of relying on runtime meta type. That not only increases `Variant` constructing performance, but also enables the possibilities for the compiler to inline and optimize out the code.

Those optimizations, and others, have improved the performance significantly.

## Performance tips

1. When construct `Variant` with large object, move the object instead of copying it.
2. If the data type in `Variant` is known at runtime, operate on the native data type instead of using meta interfaces. Meta interfaces are highly abstraction with performance cost. For example, if we have a `Variant v` of `std::vector<int>` and we need to set large amount of data in it, then we can write,  
```c++
std::vector<int> & vec = v.get<std::vector<int> &>();
vec.resize(100000);
// set 100000 elements here
```

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

10M iterations, metapp uses 282 ms, Qt uses 288 ms.  

Code for metapp

```c++
for(int i = 0; i < iterations; ++i) {
	metapp::Variant v = 5;
	v = 38.0;
	v = (long long)38;
	v = (unsigned short)9;
	v = true;
	v = 1.5f;
	dontOptimizeAway(v);
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
	dontOptimizeAway(v);
}
```

**Remarks**  

Since [the optimization in this commit](https://github.com/wqking/metapp/commit/382817969dac3cc9e61d90bda8cda73b8f274800),
now constructing and copying Variant with template value (here is fundamental value) can be inlined and the code
may be optimized out. If we remove the line `dontOptimizeAway(v)`, then the code in metapp can be optimized out and the
timing can be as fast as 16 ms (Qt is 517 ms if the line is removed). The function `dontOptimizeAway` is to keep the variable
from being removed by the optimization.

### Variant constructing and assignment, with string

10M iterations, metapp uses 629 ms, Qt uses 1995 ms.  

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

### Variant constructing and assignment, with heavy copy but trivial move object

10M iterations, metapp uses 1280 ms, Qt uses 3584 ms.  

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

### Variant casting

10M iterations, metapp uses 282 ms, Qt uses 951 ms.  

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

10M iterations, metapp uses 105 ms, Qt uses 409 ms.  

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

10M iterations, metapp uses 155 ms, Qt uses 201 ms.  

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

### Invoke method `void ()`

10M iterations, metapp uses 63 ms, Qt uses 238 ms.  

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

### Invoke method `int (int, int)` with argument `(int, int)`, no casting

10M iterations, metapp uses 227 ms, Qt uses 324 ms.  

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

### Invoke method `int (int, int)` with argument `(double, double)`, with casting

10M iterations, metapp uses 457 ms, Qt uses 555 ms.  

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

