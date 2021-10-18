
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_TEST_FIXED_POINT_TO_STRING_H)
#define CNL_TEST_FIXED_POINT_TO_STRING_H

#include <cnl/scaled_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace test_to_string {
    TEST(to_string, scaled_integer_negative)  // NOLINT
    {
        ASSERT_EQ(
                std::string{"-5016.50914001464843"},
                cnl::to_string(
                        cnl::scaled_integer<int, cnl::power<-16>>(-5016.5091400146484375)));
    }

    TEST(to_string, scaled_integer_small)  // NOLINT
    {
        ASSERT_EQ(
                std::string{".00390625"},
                cnl::to_string(cnl::scaled_integer<int, cnl::power<-16>>(0.00390625)));
    }

    TEST(to_string, scaled_integer_very_large)  // NOLINT
    {
        ASSERT_EQ(
                std::string{"1099511627776000"},
                cnl::to_string(cnl::scaled_integer<int, cnl::power<40>>{1099511627776000LL}));
    }

    TEST(to_string, scaled_integer_very_small)  // NOLINT
    {
        ASSERT_EQ(
                std::string{"9.31322574615478515e-10"},
                cnl::to_string(cnl::scaled_integer<int, cnl::power<-40>>{
                        0.000000000931322574615478515625}));
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_STRING_H
