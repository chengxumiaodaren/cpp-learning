#include <iostream>
#include <string>
#include <variant>

void test_index() {
    std::variant<std::monostate, int, float, std::string> var;
    var = 1;
    std::cout << var.index() << "\n";
    var = 2.90f;
    std::cout << var.index() << "\n";
    var = std::string("hello world");
    std::cout << var.index() << "\n";
}

template <typename T, typename>
struct get_index;

template <size_t I, typename... Ts>
struct get_index_impl {};

template <size_t I, typename T, typename... Ts>
struct get_index_impl<I, T, T, Ts...> : std::integral_constant<size_t, I> {};

template <size_t I, typename T, typename U, typename... Ts>
struct get_index_impl<I, T, U, Ts...> : get_index_impl<I + 1, T, Ts...> {};

template <typename T, typename... Ts>
struct get_index<T, std::variant<Ts...>> : get_index_impl<0, T, Ts...> {};

template <typename T, typename... Ts>
constexpr auto get_index_v = get_index<T, Ts...>::value;

using variant_t = std::variant<std::monostate, int, float, std::string>;

constexpr static auto kPlaceholderIndex = get_index_v<std::monostate, variant_t>;
constexpr static auto kIntIndex = get_index_v<int, variant_t>;
constexpr static auto kFloatIndex = get_index_v<float, variant_t>;
constexpr static auto kStringIndex = get_index_v<std::string, variant_t>;

void test_using_index() {
    std::cout << "kPlaceholderIndex " << kPlaceholderIndex << "\n";
    std::cout << "kIntIndex " << kIntIndex << "\n";
    std::cout << "kFloatIndex " << kFloatIndex << "\n";
    std::cout << "kStringIndex " << kStringIndex << "\n";

    auto custom_visitor = [](const auto& value) {
        switch (value.index()) {
            case kPlaceholderIndex:
                std::cout << "placehodler value "
                          << "\n";
                break;
            case kIntIndex:
                std::cout << "int value " << std::get<int>(value) << "\n";
                break;
            case kFloatIndex:
                std::cout << "float value " << std::get<float>(value) << "\n";
                break;
            case kStringIndex:
                std::cout << "string value " << std::get<std::string>(value) << "\n";
                break;
        }
    };
    variant_t var;
    custom_visitor(var);

    var = 1;
    custom_visitor(var);

    var = 2.90f;
    custom_visitor(var);

    var = std::string("hello world");
    custom_visitor(var);

    var = std::string("hello type");
}

int main() { test_using_index(); }
