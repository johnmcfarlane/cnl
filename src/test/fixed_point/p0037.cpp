
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::fixed_point;
using cnl::multiply;
using cnl::set_digits_t;
using cnl::sqrt;
using std::is_same;
using cnl::_impl::identical;

////////////////////////////////////////////////////////////////////////////////
// Tests of Examples in P0037

// Operator Overloads

static_assert(identical(fixed_point<uint8_t, -3>{8} + fixed_point<int8_t, -4>{3}, fixed_point<int, -4>{11}), "Incorrect information in P0037 section, Operator Overloads");
static_assert(identical(fixed_point<uint8_t, -3>{8} + 3, fixed_point<int, -3>{11}), "Incorrect information in P0037 section, Operator Overloads");
static_assert(identical(fixed_point<uint8_t, -3>{8} + float{3}, float{11}), "Incorrect information in P0037 section, Operator Overloads");

// Overflow
TEST(proposal, overflow) {
    switch (sizeof(int)) {
    case 4: {
        auto sum = fixed_point<uint32_t, -30>(3) + fixed_point<uint32_t, -30>(1);
        ASSERT_TRUE(sum == 0);
        break;
    }
    case 8: {
        auto sum = fixed_point<uint64_t, -62>(3) + fixed_point<uint64_t, -62>(1);
        ASSERT_TRUE(sum == 0);
        break;
    }
    default:
        FAIL() << "dying to know what architecture this is";
    }
}

// Underflow
static_assert(identical(fixed_point<int8_t>(15)/fixed_point<int8_t>(2), fixed_point<int, -7>(7.5f)),
        "Incorrect information in proposal section, Underflow");

// Named Arithmetic Functions
namespace named_arithmetic1 {
    constexpr auto f = fixed_point<uint8_t, -4>{15.9375};
    constexpr auto p = multiply(f, f);

    static_assert(identical(p, fixed_point<uint16_t, -8>{254.00390625}), "Incorrect information in proposal section, Named Arithmetic Functions");
}

TEST(proposal, named_arithmetic2)
{
    auto f = fixed_point<unsigned, -28>{15.9375};
    auto p = f * f;

    static_assert(is_same<decltype(p), fixed_point<unsigned, -56>>::value, "Incorrect information in proposal section, Named Arithmetic Functions");
    ASSERT_EQ(p, 0.);
}

namespace named_arithmetic3 {
    constexpr auto f = fixed_point<unsigned, -28>{15.9375};
    constexpr auto p = multiply(f, f);

    static_assert(identical(p, fixed_point<uint64_t, -56>{254.00390625}),
            "Incorrect information in proposal section, Named Arithmetic Functions");
}

namespace named_arithmetic4 {
    constexpr auto a1 = fixed_point<int8_t, 32>{0x7f00000000LL};
    constexpr auto a2 = fixed_point<int8_t, 0>{0x7f};
    constexpr auto s = add(a1, a2);

    static_assert(identical(s, fixed_point<int64_t, 0>{0x7f0000007fLL}), "Incorrect information in proposal section, Named Arithmetic Functions");
}

namespace named_arithmetic5 {
    constexpr auto n = fixed_point<uint32_t, -16>{1};
    constexpr auto d = fixed_point<uint32_t, -16>{2};

    constexpr auto q1 = n/d;
    static_assert(identical(q1, fixed_point<uint64_t, -32>{0.5}), "Incorrect information in proposal section, Named Arithmetic Functions");

    constexpr auto q2 = divide(n, d);
    static_assert(identical(q2, fixed_point<uint32_t, 0>{0}), "Incorrect information in proposal section, Named Arithmetic Functions");
}

// The `width` Helper Type
static_assert(cnl::digits<int16_t>::value == 15, "Incorrect information in proposal section, The `width` Helper Type");

// The `set_width` and `set_digits_t` Helper Types
static_assert(is_same<set_digits_t<int8_t, 63>, int64_t>::value, "Incorrect information in proposal section, The `set_width` and `set_digits_t` Helper Types");

// Examples
template<class Fp>
constexpr auto magnitude(Fp x, Fp y, Fp z)
-> decltype(sqrt(x*x+y*y+z*z))
{
    return sqrt(x*x+y*y+z*z);
}

TEST(proposal, examples)
{
    constexpr auto m = magnitude(
            fixed_point<uint16_t, -12>(1),
            fixed_point<uint16_t, -12>(4),
            fixed_point<uint16_t, -12>(9));
    static_assert(identical(m, fixed_point<int, -24>{9.8994948864}), "Incorrect information in proposal section, Examples");
}

TEST(proposal, zero)
{
    static fixed_point<> zero;
    ASSERT_EQ(zero, fixed_point<>(0));
}
