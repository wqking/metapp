#include <QCoreApplication>
#include <QMetaMethod>

#include <iostream>
#include <memory>
#include <chrono>
#include <vector>

constexpr int generalIterations = 10 * 1000 * 1000;

template <typename F>
uint64_t measureElapsedTime(F f)
{
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    f();
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count();
}

template <typename T>
void dontOptimizeAway(T && value)
{
    __asm__ __volatile__("" :: "m" (value));
}

class TestClass : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE void nothing() {
    }

    Q_INVOKABLE int add(int a, int b) {
        return a + b;
    }

    Q_PROPERTY(int value MEMBER value)

    int value;
};

void benchmarkMethodNothing()
{
    TestClass obj;
    QByteArray normalizedSignature = QMetaObject::normalizedSignature("nothing()");
    const int index = obj.metaObject()->indexOfMethod(normalizedSignature);
    QMetaMethod method = obj.metaObject()->method(index);
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([&method, &obj]() {
        for(int i = 0; i < iterations; ++i) {
            method.invoke(&obj, Qt::DirectConnection);
        }

    });
    std::cout << "Callable, invoke `void TestClass::nothing()`: " << t1 << std::endl;
}

void benchmarkMethodAdd()
{
    TestClass obj;
    QByteArray normalizedSignature = QMetaObject::normalizedSignature("add(int, int)");
    const int index = obj.metaObject()->indexOfMethod(normalizedSignature);
    QMetaMethod method = obj.metaObject()->method(index);
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([&method, &obj]() {
        int result = 0;
        for(int i = 0; i < iterations; ++i) {
            method.invoke(&obj, Qt::DirectConnection, Q_RETURN_ARG(int, result), Q_ARG(int, i), Q_ARG(int, i + 1));
        }

    });
    std::cout << "Callable, invoke `int TestClass::add(const int a, const int b)` with `int, int`: " << t1 << std::endl;
}

void benchmarkMethodCast()
{
    TestClass obj;
    QByteArray normalizedSignature = QMetaObject::normalizedSignature("add(int, int)");
    const int index = obj.metaObject()->indexOfMethod(normalizedSignature);
    QMetaMethod method = obj.metaObject()->method(index);
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([&method, &obj]() {
        int result = 0;
        for(int i = 0; i < iterations; ++i) {
            QVariant a((double)i);
            QVariant b((double)i + 1.0);
            method.invoke(&obj, Qt::DirectConnection, Q_RETURN_ARG(int, result), Q_ARG(int, a.toInt()), Q_ARG(int, b.toInt()));
        }

    });
    std::cout << "Callable, invoke `int TestClass::add(const int a, const int b)` with `double, double`: " << t1 << std::endl;
}

void benchmarkPropertyGet()
{
    TestClass obj;
    const int index = obj.metaObject()->indexOfProperty("value");
    QMetaProperty property = obj.metaObject()->property(index);
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([&property, &obj]() {
        for(int i = 0; i < iterations; ++i) {
            property.read(&obj);
        }
    });
    std::cout << "Property get: " << t1 << std::endl;
}

void benchmarkPropertySet()
{
    TestClass obj;
    const int index = obj.metaObject()->indexOfProperty("value");
    QMetaProperty property = obj.metaObject()->property(index);
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([&property, &obj]() {
        for(int i = 0; i < iterations; ++i) {
            property.write(&obj, i);
        }
    });
    std::cout << "Property set: " << t1 << std::endl;
}

void benchmarkVariantWithFundamental()
{
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([]() {
        for(int i = 0; i < iterations; ++i) {
            QVariant v = 5;
            v = 38.0;
            v = (long long)38;
            v = (unsigned short)9;
            v = true;
            v = 1.5f;
            dontOptimizeAway(v);
        }

    });
    std::cout << "Variant construct and assignment, with fundamental: " << t1 << std::endl;
}

void benchmarkVariantWithString()
{
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([]() {
        for(int i = 0; i < iterations; ++i) {
            QVariant v = 5;
            v = 38.0;
            v = "abc";
            v = QString("def");
        }

    });
    std::cout << "Variant construct and assignment, with string: " << t1 << std::endl;
}

void benchmarkVariantGetInt()
{
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([]() {
        QVariant v = 5;
        for(int i = 0; i < iterations; ++i) {
            dontOptimizeAway(v.toInt());
        }

    });
    std::cout << "Variant get int: " << t1 << std::endl;
}

void benchmarkVariantCast()
{
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([]() {
        QVariant v = 5;
        for(int i = 0; i < iterations; ++i) {
            v.convert(QMetaType::Double);
            v.convert(QMetaType::LongLong);
            v.convert(QMetaType::Int);
        }

    });
    std::cout << "Variant cast: " << t1 << std::endl;
}

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

Q_DECLARE_METATYPE(HeavyCopy)

void benchmarkHeavyCopy()
{
    constexpr int iterations = generalIterations;
    const auto t1 = measureElapsedTime([]() {
        for(int i = 0; i < iterations; ++i) {
            QVariant v = QVariant::fromValue(HeavyCopy());
            v = 5;
            v = QVariant::fromValue(HeavyCopy());
        }

    });
    std::cout << "Variant from heavy copy object: " << t1 << std::endl;
}

int main(int /*argc*/, char */*argv*/[])
{
    benchmarkMethodNothing();
    benchmarkMethodAdd();
    benchmarkMethodCast();
    benchmarkPropertyGet();
    benchmarkPropertySet();
    benchmarkVariantWithFundamental();
    benchmarkVariantWithString();
    benchmarkVariantGetInt();
    benchmarkVariantCast();
    benchmarkHeavyCopy();

    return 0;
}

#include "qtbenchmark.moc"
