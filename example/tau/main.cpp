#include <cnl/all.h>
#include <iostream>

int main() {
    using namespace cnl::literals;

    auto pi = cnl::math_constants::pi<cnl::elastic_number<10, -8>>;
    auto tau = pi * 2_elastic;

    std::cout << tau << '\n';
}
