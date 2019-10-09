
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Signed 15:16 Fixed-Point Square Function Using cnl::elastic_scaled_integer

// Here's how to use the CNL library on Godbolt.org.
// Normally, you'd just add `#include <cnl/all.h>`.
#include <cnl/all.h>
using namespace cnl;

// square a nunber using 15:16 fixed-point arithmetic
// without using a fixed-point library
float square_int(float input) {
    // user must fixed_width_scale values by the correct amount
    auto fixed = static_cast<int32_t>(input * 65536.F);

    // user must remember to widen the result to avoid overflow
    auto prod = int64_t{fixed} * fixed;

    // user must remember that the fixed_width_scale also was squared
    return static_cast<float>(prod) / 4294967296.F;
}

// the same function using cnl::elastic_integer
float square_elastic_integer(float input) {
    auto fixed = elastic_integer<31>{input * 65536.F};

    // elastic_integer automatically widens the result
    auto prod = fixed * fixed;

    // but the user must still do all the scaling themselves
    return static_cast<float>(prod) / 4294967296.F;
}

// the same function using cnl::scaled_integer
float square_scaled_integer(float input) {
    // scaled_integer handles scaling
    auto fixed = scaled_integer<int32_t, power<-16>>{input};

    // but it uses int under the hood; user must still widen
    auto prod = scaled_integer<int64_t, power<-16>>{fixed} * fixed;

    return static_cast<float>(prod);
}

// finally, the composition of scaled_integer and elastic_integer
float square_elastic(float input) {
    // alias to scaled_integer<elastic_integer<31, int>, -16>
    auto fixed = elastic_scaled_integer<15, 16>{input};

    // concise, safe and zero-cost!
    auto prod = fixed * fixed;

    return static_cast<float>(prod);
}
