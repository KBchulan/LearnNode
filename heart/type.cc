#include <tuple>
#include <variant>
#include <optional>
#include <iostream>

int main(){
    std::variant<int, double> a = 2;
    std::cout << std::get<int>(a) << std::endl;
    std::visit([](auto &args){
        std::cout << args << std::endl;
    }, a);

    std::optional<int> b = std::make_optional<int>(6);
    std::cout << b.value() << std::endl;
    std::cout << *b << std::endl;

    std::tuple<int, double, std::string> c = std::make_tuple<int, double, std::string>(3, 4.0, "string");
    std::cout << std::get<int>(c) << std::endl;
    std::cout << std::get<2>(c) << std::endl;
}