
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_TEST_FIXED_POINT_TO_STRING_H)
#define CNL_TEST_FIXED_POINT_TO_STRING_H

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace {  // NOLINT(cert-dcl59-cpp)
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
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_STRING_H
