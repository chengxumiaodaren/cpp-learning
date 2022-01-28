#include <iostream>
#include <string>
#include <variant>

struct A {
    void func() const { std::cout << "func A \n"; }
};

struct B {
    void func() const { std::cout << "func B \n"; }
};

struct CallFunc {
    void operator()(const A& a) { a.func(); }
    void operator()(const B& b) { b.func(); }
};

void test_no_param_polymorphism() {
    std::variant<A, B> var;
    var = A();
    std::visit(CallFunc{}, var);
    var = B();
    std::visit(CallFunc{}, var);
}

struct C {
    void func(int value) const { std::cout << "func C " << value << "\n"; }
};

struct D {
    void func(int value) const { std::cout << "func D " << value << "\n"; }
};

struct CallFuncParam {
    void operator()(const C& c) { c.func(value); }
    void operator()(const D& d) { d.func(value); }

    int value;
};

void test_param_polymorphism() {
    std::variant<C, D> var;
    var = C();
    std::visit(CallFuncParam{1}, var);
    var = D();
    std::visit(CallFuncParam{2}, var);
}

void test_param_lambda_polymorphism() {
    std::variant<C, D> var;
    int value = 1;
    auto caller = [&value](const auto& v) { v.func(value); };
    std::visit(caller, var);
    value = 2;
    std::visit(caller, var);
}

int main() {
    test_no_param_polymorphism();
    std::cout << " ====== \n";
    test_param_polymorphism();
    std::cout << " ====== \n";
    test_param_lambda_polymorphism();
}
