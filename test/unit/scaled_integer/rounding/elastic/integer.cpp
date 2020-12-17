
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/scaled_integer.h>

#include <string>

#include <gtest/gtest.h>

namespace {
    template<int Digits, int Exponent>
    using scaled_integer_rounding_elastic_integer =
        cnl::scaled_integer<cnl::rounding_integer<cnl::elastic_integer<Digits>>, cnl::power<Exponent>>;

    TEST(scaled_integer_rounding_elastic_integer, to_chars)  // NOLINT
    {
        auto expected = std::string{"25.25"};
        auto actual = std::string{cnl::to_chars(scaled_integer_rounding_elastic_integer<24, -20>{25.25}).data()};
        ASSERT_EQ(expected, actual);
    }

    TEST(scaled_integer_rounding_elastic_integer, ostream)  // NOLINT
    {
        testing::internal::CaptureStdout();
        std::cout << scaled_integer_rounding_elastic_integer<24, -20>{ 25.25 };
        ASSERT_EQ("25.25", testing::internal::GetCapturedStdout());
    }
}
