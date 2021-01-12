
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)NODISCARD

#include <cnl/static_number.h>

#include <gtest/gtest.h>

#include <array>
#include <string>

using x = std::common_type<
        cnl::_impl::wrapper<
                cnl::_impl::wrapper<long int, cnl::wide_tag<32, int>>,
                cnl::native_rounding_tag>,
        cnl::_impl::wrapper<
                cnl::_impl::wrapper<int, cnl::wide_tag<31, int>>,
                cnl::native_rounding_tag>>;

// namespace {
//     TEST(static_number, to_chars)  // NOLINT
//     {
//         auto const num_chars = 12;
//         auto buffer = std::array<char, num_chars + 1>{};
//         auto* chars_first = buffer.data();
//         auto* chars_last = chars_first + num_chars;
//         auto result =
//                 cnl::to_chars(chars_first, chars_last, cnl::static_number<24, -8>{-45678.765625});
//         ASSERT_EQ(std::errc{}, result.ec);
//         ASSERT_EQ(chars_last, result.ptr);
//         (*chars_last) = '\0';

//         ASSERT_EQ(std::string{"-45678.76562"}, chars_first);
//     }

// #if defined(CNL_INT128_ENABLED)
//     TEST(static_number, to_chars_wider)  // NOLINT
//     {
//         auto const num_chars = 12;
//         auto buffer = std::array<char, num_chars + 1>{};
//         auto* chars_first = buffer.data();
//         auto* chars_last = chars_first + num_chars;
//         auto result =
//                 cnl::to_chars(chars_first, chars_last, cnl::static_number<28, -8>{937016.765625});
//         ASSERT_EQ(std::errc{}, result.ec);
//         ASSERT_EQ(chars_last, result.ptr);
//         *chars_last = '\0';

//         ASSERT_EQ(std::string{"937016.76562"}, chars_first);
//     }
// #endif
// }
