
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Signed 15:16 Fixed-Point Average Function Using cnl::elastic_scaled_integer

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_scaled_integer.h>

#include <gtest/gtest.h>

using namespace cnl;
using namespace cnl::literals;

// average two nunbers using 15:16 fixed-point arithmetic using native types
constexpr float average_integer(float input1, float input2) {
    // user must fixed_width_scale values by the correct amount
    auto fixed1 = static_cast<int32_t>(input1 * 65536.F);
    auto fixed2 = static_cast<int32_t>(input2 * 65536.F);

    // user must remember to widen the result to avoid overflow
    auto sum = int64_t{fixed1} + fixed2;

    // user must elide the two divisions (averaging and scaling) together
    return sum / (2 * 65536.F);
}

// the same function using cnl::elastic_integer -
// a numeric type which widens to avoid overflow
constexpr float average_elastic_integer(float input1, float input2) {
    // elastic_integer behaves a lot like native ints
    auto fixed1 = elastic_integer<31>{input1 * 65536.F};
    auto fixed2 = elastic_integer<31>{input2 * 65536.F};

    // sum is one bit wider to reduce risk of overflow
    auto sum = fixed1 + fixed2;

    // but the user must still do all the scaling themselves
    return sum / (2 * 65536.F);
}

// the same function using cnl::scaled_integer
constexpr float average_scaled_integer(float input1, float input2) {
    // scaled_integer handles scaling
    auto fixed1 = scaled_integer<int32_t, cnl::power<-16>>{input1};
    auto fixed2 = scaled_integer<int32_t, cnl::power<-16>>{input2};

    // but it uses int under the hood; user must still widen
    auto sum = scaled_integer<int64_t, cnl::power<-16>>{fixed1} + fixed2;

    // divide-by 
    return static_cast<float>(sum >> 1_c);
}

// finally, the composition of scaled_integer and elastic_integer
constexpr float average_elastic(float input1, float input2) {
    // define optimally-scaled quantity types with this user-defined literal;
    // e.g. 65536_elastic uses 2 bits of storage
    // and 1_elastic/65536_elastic uses 3 bits of storage!
    using namespace literals;

    // elastic_scaled_integer<31, -16> aliases to scaled_integer<elastic_integer<31, int>, -16>
    auto fixed1 = elastic_scaled_integer<31, -16>{input1};
    auto fixed2 = elastic_scaled_integer<31, -16>{input2};

    // concise, overflow-resistant and zero-cost!
    auto sum = fixed1 + fixed2;

    return static_cast<float>(sum >> 1_c);
}

using namespace literals;
using cnl::_impl::identical;
static_assert(identical(65536_elastic, elastic_scaled_integer<1, 16>{65536}), "mistaken comment in average_elastic");
static_assert(identical(1_elastic/65536_elastic, elastic_scaled_integer<1, -16>{0.0000152587890625}), "mistaken comment in average_elastic");

#if (__cpp_constexpr >= 201304L)
static_assert(identical(average_integer(32000.125, 27805.75), 29902.9375F), "average_integer test failed");
static_assert(identical(average_elastic_integer(32000.125, 27805.75), 29902.9375F), "average_elastic_integer test failed");
static_assert(identical(average_scaled_integer(32000.125, 27805.75), 29902.9375F), "average_scaled_integer test failed");
static_assert(identical(average_elastic(32000.125, 27805.75), 29902.9375F), "average_elastic test failed");
#else
TEST(zero_cost_average, integer) {  // NOLINT
    ASSERT_EQ(average_integer(32000.125, 27805.75), 29902.9375F);
}

TEST(zero_cost_average, elastic_integer) {  // NOLINT
    ASSERT_EQ(average_elastic_integer(30000, 0.125), 15000.0625F);
}

TEST(zero_cost_average, scaled_integer) {  // NOLINT
    ASSERT_EQ(average_scaled_integer(30000, 0.125), 15000.0625F);
}

TEST(zero_cost_average, elastic_scaled_integer) {  // NOLINT
    ASSERT_EQ(average_elastic(30000, 0.125), 15000.0625F);
}
#endif
