
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>

#include <gtest/gtest.h>

#include <array>
#include <string>

namespace {
    TEST(static_number, to_chars_narrower)
    {
        auto const num_chars = 4;
        auto buffer = std::array<char, num_chars+1>{};
        auto chars_first = buffer.data();
        auto chars_last = chars_first+num_chars;
        auto result = cnl::to_chars(
                chars_first,
                chars_last,
                cnl::static_number<16, -10, cnl::native_rounding_tag>{20.45697});
        ASSERT_EQ(std::errc{}, result.ec);
        ASSERT_EQ(chars_last, result.ptr);
        *chars_last = '\0';

        ASSERT_EQ(std::string{"20.5"}, chars_first);
    }
}
