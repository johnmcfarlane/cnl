#include <cnl/elastic_number.h>
#include <cnl/numeric.h>
#include <iostream>

int main() {
    using cnl::elastic_number;

    auto pi = cnl::math_constants::pi<elastic_number<10, -7>>;

    using namespace cnl::literals;
    auto tau = pi * 2_elastic;

    std::cout << (float)pi << '\n';
}
