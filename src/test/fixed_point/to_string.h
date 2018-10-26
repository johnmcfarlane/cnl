
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_TEST_FIXED_POINT_TO_STRING_H
#define CNL_TEST_FIXED_POINT_TO_STRING_H

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace {
    namespace test_to_string {
        TEST(to_string, fixed_point_negative)
        {
            ASSERT_EQ(
                    std::string{"-5016.5091400146484375"},
                    cnl::to_string(cnl::fixed_point<int, -16>(-5016.5091400146484375)));
        }

        TEST(to_string, fixed_point_small)
        {
            ASSERT_EQ(
                    std::string{"0.00390625"},
                    cnl::to_string(cnl::fixed_point<int, -16>(0.00390625)));
        }

        TEST(to_string, fixed_point_wide_lowest)
        {
            // -9223372036854775808÷4294967296
            ASSERT_EQ(
                    std::string{"-2147483648"},
                    cnl::to_string(cnl::numeric_limits<cnl::fixed_point<cnl::int64, -32>>::lowest()));
        }

        TEST(to_string, fixed_point_wide_negative_max)
        {
            // -9223372036854775807÷4294967296
            ASSERT_EQ(
                    std::string{"-2147483647.99999999976716935634613037109375"},
                    cnl::to_string(-cnl::numeric_limits<cnl::fixed_point<cnl::int64, -32>>::max()));
        }

        TEST(to_string, fixed_point_wide_min)
        {
            // 1÷4294967296
            ASSERT_EQ(
                    std::string{"0.00000000023283064365386962890625"},
                    cnl::to_string(cnl::numeric_limits<cnl::fixed_point<cnl::int64, -32>>::min()));
        }

        TEST(to_string, fixed_point_wide_max)
        {
            // 9223372036854775807÷4294967296
            ASSERT_EQ(
                    std::string{"2147483647.99999999976716935634613037109375"},
                    cnl::to_string(cnl::numeric_limits<cnl::fixed_point<cnl::int64, -32>>::max()));
        }
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_STRING_H
