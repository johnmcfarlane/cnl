
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Signed 15:16 Fixed-Point Average Function Using cnl::elastic_fixed_point

#include "fixed_point/elastic/elastic_fixed_point.h"
#include <gtest/gtest.h>

using namespace cnl;

#if (__cplusplus>=201402L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

// average two nunbers using 15:16 fixed-point arithmetic using native types
CONSTEXPR float average_integer(float input1, float input2) {
    // user must scale values by the correct amount
    auto fixed1 = static_cast<int32_t>(input1 * 65536.f);
    auto fixed2 = static_cast<int32_t>(input2 * 65536.f);

    // user must remember to widen the result to avoid overflow
    auto sum = int64_t{fixed1} + fixed2;
    auto avg = sum / 2;

    // user must remember that the scale also was squared
    return avg / 65536.f;
}

// the same function using cnl::elastic_integer -
// a numeric type which widens to avoid overflow
CONSTEXPR float average_elastic_integer(float input1, float input2) {
    // use this user-defined literal - shorthand for std::integral_constant
    using namespace literals;

    // elastic_integer behaves a lot like native ints
    auto fixed1 = elastic_integer<31>{input1 * 65536.f};
    auto fixed2 = elastic_integer<31>{input2 * 65536.f};

    // sum is one bit wider to reduce risk of overflow
    auto sum = fixed1 + fixed2;
    auto avg = sum / 2_c;

    // but the user must still do all the scaling themselves
    return static_cast<float>(avg) / 65536.f;
}

// the same function using cnl::fixed_point
CONSTEXPR float average_fixed_point(float input1, float input2) {
    // fixed_point handles scaling
    auto fixed1 = fixed_point<int32_t, -16>{input1};
    auto fixed2 = fixed_point<int32_t, -16>{input2};

    // but it uses int under the hood; user must still widen
    auto sum = fixed_point<int64_t, -16>{fixed1} + fixed2;
    auto avg = divide(sum, 2);

    return static_cast<float>(avg);
}

// finally, the composition of fixed_point and elastic_integer
CONSTEXPR float average_elastic(float input1, float input2) {
    // define optimally-scaled quantity types with this user-defined literal;
    // e.g. 65536_elastic uses 2 bits of storage
    // and 1_elastic/65536_elastic uses 3 bits of storage!
    using namespace literals;

    // elastic_fixed_point<31, -16> aliases to fixed_point<elastic_integer<31, int>, -16>
    auto fixed1 = elastic_fixed_point<31, -16>{input1};
    auto fixed2 = elastic_fixed_point<31, -16>{input2};

    // concise, overflow-resistant and zero-cost!
    auto sum = fixed1 + fixed2;
    auto avg = sum / 2_elastic;

    return static_cast<float>(avg);
}

using namespace literals;
using cnl::_impl::identical;
static_assert(identical(65536_elastic, elastic_fixed_point<1, 16>{65536}), "mistaken comment in average_elastic");
static_assert(identical(1_elastic/65536_elastic, elastic_fixed_point<2, -17>{0.0000152587890625}), "mistaken comment in average_elastic");

#if (__cplusplus>=201402L)
static_assert(identical(average_integer(32000.125, 27805.75), 29902.9375f), "average_integer test failed");
static_assert(identical(average_elastic_integer(32000.125, 27805.75), 29902.9375f), "average_elastic_integer test failed");
static_assert(identical(average_fixed_point(32000.125, 27805.75), 29902.9375f), "average_fixed_point test failed");
static_assert(identical(average_elastic(32000.125, 27805.75), 29902.9375f), "average_elastic test failed");
#else
TEST(zero_cost_average, integer) {
    ASSERT_EQ(average_integer(32000.125, 27805.75), 29902.9375f);
}

TEST(zero_cost_average, elastic_integer) {
    ASSERT_EQ(average_elastic_integer(30000, 0.125), 15000.0625f);
}

TEST(zero_cost_average, fixed_point) {
    ASSERT_EQ(average_fixed_point(30000, 0.125), 15000.0625f);
}

TEST(zero_cost_average, elastic_fixed_point) {
    ASSERT_EQ(average_elastic(30000, 0.125), 15000.0625f);
}
#endif
