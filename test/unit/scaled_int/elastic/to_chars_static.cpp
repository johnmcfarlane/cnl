
//          Copyright John McFarlane 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/charconv/to_chars.h>

#include <cnl/elastic_scaled_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    TEST(to_chars_static, elastic_scaled_123_456)  // NOLINT
    {
        using namespace cnl::literals;
        constexpr auto expected{cnl::to_chars_static_result<10>{{'1', '2', '3', '.', '4', '5', '6'}, 7}};
        constexpr auto actual{cnl::to_chars_static(123.456_cnl)};
#if !defined(_MSC_VER)
        static_assert(identical(expected, actual));
#endif
        ASSERT_EQ(expected, actual);
    }

    TEST(to_chars_static, elastic_scaled_12345_6)  // NOLINT
    {
        using namespace cnl::literals;
        constexpr auto expected{cnl::to_chars_static_result<8>{{'1', '2', '3', '4', '5', '.', '6'}, 7}};
        constexpr auto actual{cnl::to_chars_static(12345.6_cnl)};
#if !defined(_MSC_VER)
        static_assert(identical(expected, actual));
#endif
        ASSERT_EQ(expected, actual);
    }

    TEST(to_chars_static, elastic_scaled_123456_)  // NOLINT
    {
        using namespace cnl::literals;
        constexpr auto expected{cnl::to_chars_static_result<7>{{'1', '2', '3', '4', '5', '6'}, 6}};
        constexpr auto actual{cnl::to_chars_static(123456._cnl)};
#if !defined(_MSC_VER)
        static_assert(identical(expected, actual));
#endif
        ASSERT_EQ(expected, actual);
    }

    TEST(to_chars_static, elastic_scaled_n123456_)  // NOLINT
    {
        using namespace cnl::literals;
        constexpr auto expected{cnl::to_chars_static_result<7>{{'-', '1', '9', '3', '4', '5', '6'}, 7}};
        constexpr auto actual{cnl::to_chars_static(-123456._cnl)};
#if !defined(_MSC_VER)
        // static_assert(identical(expected, actual));
#endif
        ASSERT_EQ(expected, actual);
    }
}
