
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Signed 15:16 Fixed-Point Square Function Using cnl::elastic_fixed_point

// Here's how to use the fixed_point library on Godbolt.org.
// Normally, you'd just add `#include <fixed_point>`.
#include "fixed_point/elastic/elastic_fixed_point.h"
using namespace cnl;

// square a nunber using 15:16 fixed-point arithmetic
// without using a fixed-point library
float square_int(float input) {
    // user must scale values by the correct amount
    auto fixed = static_cast<int32_t>(input * 65536.f);

    // user must remember to widen the result to avoid overflow
    auto prod = int64_t{fixed} * fixed;

    // user must remember that the scale also was squared
    return prod / 4294967296.f;
}

// the same function using cnl::elastic_integer
float square_elastic_integer(float input) {
    auto fixed = elastic_integer<31>{input * 65536.f};

    // elastic_integer automatically widens the result
    auto prod = fixed * fixed;

    // but the user must still do all the scaling themselves
    return static_cast<float>(prod) / 4294967296.f;
}

// the same function using cnl::fixed_point
float square_fixed_point(float input) {
    // fixed_point handles scaling
    auto fixed = fixed_point<int32_t, -16>{input};

    // but it uses int under the hood; user must still widen
    auto prod = fixed_point<int64_t, -16>{fixed} * fixed;

    return static_cast<float>(prod);
}

// finally, the composition of fixed_point and elastic_integer
float square_elastic(float input) {
    // alias to fixed_point<elastic_integer<31, int>, -16>
    auto fixed = elastic_fixed_point<15, 16>{input};

    // concise, safe and zero-cost!
    auto prod = fixed * fixed;

    return static_cast<float>(prod);
}
