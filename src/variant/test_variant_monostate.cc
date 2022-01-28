#include <iostream>
#include <variant>

/**
 * @brief 为什么要使用monostate
 *
 */

struct S {
    S(int i) : value{i} {}
    int value;
};

void test_monostate() {
    std::variant<std::monostate, S> var;
    var = 12;
    std::cout << std::get<S>(var).value << "\n";
}

/*
void test_monostate2() {
    ///< 编译失败，S如果没有构造函数，需要加monostate
    std::variant<S> var;
    var = 12;
    std::cout << std::get<S>(var).value << "\n";
}
*/

void test_variant() {
    std::variant<int, float> var;
    var = 12;
    std::cout << std::get<int>(var) << "\n";
    var = 12.1f;
    std::cout << std::get<float>(var) << "\n";
}

int main() {
    test_monostate();
    test_variant();
}
