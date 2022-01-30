
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/charconv/to_chars.h>

#include <test.h>

#include <cstdint>
#include <limits>

#if !defined(_MSC_VER)
namespace {
    static_assert(identical(
            cnl::to_chars_static_result<11>{{'4', '2'}, 2},
            cnl::to_chars_static(std::int32_t{42})));
}
#endif

TEST(charconv_to_chars, int32_42)  // NOLINT
{
    auto const expected{cnl::to_chars_static_result<11>{{'4', '2'}, 2}};
    auto const actual{cnl::to_chars_static(std::int32_t{42})};
    ASSERT_EQ(expected, actual);
}

TEST(charconv_to_chars, int32_min)  // NOLINT
{
    auto const expected{cnl::to_chars_static_result<11>{
            {'-', '2', '1', '4', '7', '4', '8', '3', '6', '4', '7'}, 11}};
    auto const actual{cnl::to_chars_static(std::numeric_limits<std::int32_t>::min() + 1)};
    ASSERT_EQ(expected, actual);
}

TEST(charconv_to_chars, uint16_max)  // NOLINT
{
    auto const expected{cnl::to_chars_static_result<5>{
            {'6', '5', '5', '3', '5'}, 5}};
    auto const actual{cnl::to_chars_static(std::numeric_limits<std::uint16_t>::max())};
    ASSERT_EQ(expected, actual);
}

TEST(charconv_to_chars, int64_zero)  // NOLINT
{
    auto const expected{cnl::to_chars_static_result<20>{{'0'}, 1}};
    auto const actual{cnl::to_chars_static(std::int64_t{0})};
    ASSERT_EQ(expected, actual);
}
