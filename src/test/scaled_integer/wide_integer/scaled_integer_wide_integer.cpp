//
////          Copyright John McFarlane 2018.
//// Distributed under the Boost Software License, Version 1.0.
////  (See accompanying file ../../LICENSE_1_0.txt or copy at
////          http://www.boost.org/LICENSE_1_0.txt)
//
//#include <cnl/_impl/num_traits/digits.h>
//#include <cnl/scaled_integer.h>
//#include <cnl/wide_integer.h>
//
//#include <cinttypes>
//
//#include <gtest/gtest.h>
//
//// TODO: Every `#if !defined(TEST_WIDE_INTEGER)` is a TODO
//#define TEST_WIDE_INTEGER
//#define TEST_WIDE_INTEGER_INT
//#define TEST_LABEL wide_integer_  // NOLINT(cppcoreguidelines-macro-usage)
//
//////////////////////////////////////////////////////////////////////////////////
//// wide_integer type used as scaled_integer Rep type
//
//using test_int = cnl::wide_integer<cnl::digits<int>::value, int>;
//
//////////////////////////////////////////////////////////////////////////////////
//// perform scaled_integer tests with this type of scaled_integer specialization
//#include "../scaled_integer_common.h"
//
//TEST(scaled_integer_wide_integer, to_string)  // NOLINT
//{
//    auto const expected = "0.714285714285714285714285713708718880752631365184564249393162072010454721748828887939453125";
//    auto const actual = cnl::to_string(cnl::scaled_integer<cnl::wide_integer<100, unsigned>, cnl::power<-90>>{5.}/7);
//    ASSERT_EQ(expected, actual);
//}
//
//#if !defined(__arm__)
//TEST(scaled_integer_wide_integer, quotient)  // NOLINT
//{
//    using scaled_integer = cnl::scaled_integer<cnl::wide_integer<129, unsigned>, cnl::power<-96>>;
//    auto expected = scaled_integer{1.L/3};
//    auto actual = scaled_integer{cnl::make_fraction(1, 3)};
//    ASSERT_EQ(double(expected), double(actual));
//}
//
//#if !defined(__GNUC__) || defined(__clang__)
//TEST(scaled_integer_wide_integer, quotient200)  // NOLINT
//{
//    using scaled_integer = cnl::scaled_integer<cnl::wide_integer<200, unsigned>, cnl::power<-196>>;
//    auto expected = scaled_integer{5.L/7};
//    auto actual = scaled_integer{cnl::make_fraction(5, 7)};
//    ASSERT_EQ(double(expected), double(actual));
//}
//
//TEST(scaled_integer_wide_integer, ctor_fraction)  // NOLINT
//{
//    using namespace cnl::literals;
//    using scaled_integer = cnl::scaled_integer<cnl::wide_integer<200>, cnl::power<-100>>;
//
//    auto expected = cnl::_impl::from_rep<scaled_integer>(cnl::wide_integer<200>{0x5555555555555555555555555_wide});
//    auto actual = scaled_integer{cnl::fraction<int>(1, 3)};
//    ASSERT_EQ(expected, actual);
//}
//#endif
//#endif
