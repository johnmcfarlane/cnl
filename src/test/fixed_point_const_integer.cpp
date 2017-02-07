
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of sg14::const_integer specializations if sg14::fixed_point

#include <sg14/fixed_point>

#include <gtest/gtest.h>

namespace {
    using namespace sg14::literals;
    using sg14::_impl::identical;
    using sg14::const_integer;
    using sg14::fixed_point;

#if ! defined(_MSC_VER)
    constexpr auto u8_0_255 = fixed_point<const_integer<std::uint8_t, 255>, 0>{};
    static_assert(u8_0_255 == 255, "sg14::fixed_point<sg14::const_integer> test failed");

    constexpr auto u4_4_255 = u8_0_255 >> 4_c;
    static_assert(u4_4_255 == 15.9375, "sg14::fixed_point<sg14::const_integer> test failed");
#endif

    TEST(fixed_point_const_integer, multiply) {
#if defined(_MSC_VER)
        // const_integer instantiation of fixed_point cannot be statically default-constructed with MSVC
        // because default constructor of fixed_point cannot be marked as `constexpr`
        auto u8_0_255 = fixed_point<const_integer<std::uint8_t, 255>, 0>{};
        ASSERT_EQ(u8_0_255, 255);

        auto u4_4_255 = u8_0_255 >> 4_c;
        ASSERT_EQ(u4_4_255, 15.9375);
#endif

        auto product = u8_0_255 * u4_4_255;
        static_assert(sizeof(product) == 1, "sg14::fixed_point<sg14::const_integer> test failed");

        ASSERT_EQ(product, 4064.0625);
        ASSERT_EQ(product.data(), 65025_c);
    }
}
