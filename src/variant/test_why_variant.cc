#include <iostream>
#include <string>
#include <variant>

union MyUnion {
    int a;
    float b;
    double c;
};

void test_simple_union() {
    MyUnion u;
    u.a = 1;
    std::cout << u.a << "\n";

    u.b = 1.32f;
    std::cout << u.b << "\n";

    u.c = 2.32;
    std::cout << u.c << "\n";
}

struct A {
    A() = default;
    A(int aa) : a{aa} { std::cout << "A() \n"; }
    ~A() { std::cout << "~A() \n"; }
    int a;
};

struct B {
    B() = default;
    B(float bb) : b{bb} { std::cout << "B() \n"; }
    ~B() { std::cout << "~B() \n"; }
    float b;
};

union MyStructUnion {
    A a;
    B b;
    /**
     * @brief 在析构函数中我要做什么？不知道当前类型究竟是A还是B
     * 那调用 a.~A() 还是 b.~B() ?
     */
    ~MyStructUnion() { std::cout << "~MyStructUnion() \n"; }
};

/**
 * @brief 需要手动调用析构函数
 *
 */
void test_struct_union() {
    MyStructUnion u;

    new (&u.a) A(1);
    std::cout << u.a.a << "\n";
    u.a.~A();

    u.b = B(2.3f);
    std::cout << u.b.b << "\n";
    u.b.~B();
}

struct C {
    C() = default;
    C(std::string cc) : c{cc} { std::cout << "C() \n"; }
    ~C() { std::cout << "~C() \n"; }
    std::string c;
};

/**
 * @brief 使用variant完全不需要手动调用构造和析构函数，它会自动处理好所有逻辑，非常方便
 *
 */
void test_variant() {
    std::variant<std::monostate, A, C> u;
    u = 1;
    std::cout << std::get<A>(u).a << "\n";
    u = std::string("dsd");
    std::cout << std::get<C>(u).c << "\n";
}

int main() { test_variant(); }
