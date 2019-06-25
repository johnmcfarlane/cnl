
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_TEST_FIXED_POINT_TO_STRING_H
#define CNL_TEST_FIXED_POINT_TO_STRING_H

#include <cnl/scaled_integer.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace {
    namespace test_to_string {
        TEST(to_string, scaled_integer_negative)
        {
            ASSERT_EQ(
                    std::string{"-5016.5091400146484375"},
                    cnl::to_string(cnl::scaled_integer<int, cnl::power<-16>>(-5016.5091400146484375)));
        }

        TEST(to_string, scaled_integer_small)
        {
            ASSERT_EQ(
                    std::string{"0.00390625"},
                    cnl::to_string(cnl::scaled_integer<int, cnl::power<-16>>(0.00390625)));
        }
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_STRING_H
