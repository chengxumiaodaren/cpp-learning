#include <iostream>
#include <string>
#include <variant>

struct Visitor {
    void operator()(int i) const { std::cout << "int " << i << "\n"; }

    void operator()(float f) const { std::cout << "float " << f << "\n"; }

    void operator()(std::string s) const { std::cout << "string " << s << "\n"; }
};

void test_visitor_functor() {
    std::variant<int, float, std::string> var;
    var = 1;
    std::visit(Visitor(), var);
    var = 2.90f;
    std::visit(Visitor(), var);
    var = std::string("hello world");
    std::visit(Visitor(), var);
}

void test_visitor_lambda() {
    std::variant<int, float, std::string> var;
    var = 1;
    std::visit([](const auto& value) { std::cout << "value " << value << "\n"; }, var);
    var = 2.90f;
    std::visit([](const auto& value) { std::cout << "value " << value << "\n"; }, var);
    var = std::string("hello world");
    std::visit([](const auto& value) { std::cout << "value " << value << "\n"; }, var);
    var = std::string("hello type");
    std::visit(
        [](const auto& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "int value " << value << "\n";
            } else if constexpr (std::is_same_v<T, float>) {
                std::cout << "float value " << value << "\n";
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "string value " << value << "\n";
            }
        },
        var);
}

int main() {
    test_visitor_functor();
    std::cout << "=========== \n";
    test_visitor_lambda();
}
