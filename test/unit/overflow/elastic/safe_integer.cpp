
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/overflow_integer.h>

#include "../../number_test.h"

#include <gtest/gtest.h>

namespace cnl {
    // safe elastic integer
    template<
            int IntegerDigits,
            class OverflowTag = undefined_overflow_tag,
            class Narrowest = int>
    using safe_integer = overflow_integer<
            elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            OverflowTag>;

    template<
            class OverflowTag = undefined_overflow_tag,
            class Narrowest = int,
            class Input = int>
    CNL_NODISCARD safe_integer<
            numeric_limits<Input>::digits,
            OverflowTag,
            Narrowest>
    constexpr make_safe_int(Input const& input)
    {
        return input;
    }
}

namespace {
    using std::is_same;
    using cnl::_impl::identical;
    using cnl::_impl::rep_t;

    namespace default_parameters {
        static_assert(
                is_same<rep_t<rep_t<cnl::safe_integer<1>>>, int>::value,
                "cnl::safe_integer parameter default test failed");
    }

    namespace test_numeric_limits {
        using safe_saturating_integer_2 = cnl::overflow_integer<cnl::elastic_integer<2, char>, cnl::saturated_overflow_tag>;
        static_assert(identical(
                cnl::numeric_limits<safe_saturating_integer_2>::lowest(),
                safe_saturating_integer_2{-3}), "");
        static_assert(identical(
                cnl::numeric_limits<safe_saturating_integer_2>::max(),
                safe_saturating_integer_2{3}), "");
        static_assert(cnl::numeric_limits<safe_saturating_integer_2>::lowest() < cnl::numeric_limits<safe_saturating_integer_2>::max(), "");
    }

    namespace test_comparison {
        static_assert(identical(
                cnl::convert<cnl::throwing_overflow_tag, cnl::_impl::native_tag, cnl::elastic_integer<10>>(0),
                cnl::elastic_integer<10>{0}), "");
        static_assert(cnl::safe_integer<10>(0b1010101010)==cnl::safe_integer<10>(0b1010101010), "");
    }

    namespace test_make_safe_int {
        static_assert(identical(cnl::make_safe_int(cnl::int16{7}), cnl::safe_integer<15>{7}), "");
        static_assert(identical(cnl::safe_integer<34>{0}, cnl::safe_integer<34>{0}), "");
    }

    namespace test_from_value {
        static_assert(
                identical(
                        cnl::safe_integer<3>{5},
                        cnl::from_value<cnl::safe_integer<1>, cnl::constant<5>>{}(cnl::constant<5>{})),
                "");
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::safe_integer<2>{2},
                        cnl::safe_integer<1>{1}+cnl::safe_integer<1>{1}),
                "cnl::safe_integer operator+");
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::safe_integer<2>{2}-cnl::safe_integer<2>{1},
                        cnl::safe_integer<3>{1}),
                "cnl::safe_integer operator+");
    }

    namespace test_multiply {
        static_assert(identical(cnl::safe_integer<6>{55}*cnl::safe_integer<6>{4}, cnl::safe_integer<12>{220}), "cnl::safe_integer operator*");
        static_assert(identical(cnl::safe_integer<3>{7}*cnl::safe_integer<4>{10}, cnl::safe_integer<7>{70}), "cnl::safe_integer operator*");
        static_assert(identical(cnl::safe_integer<3>{3}*.25, .75), "cnl::safe_integer operator*");
        static_assert(identical(cnl::safe_integer<3>{7}*cnl::safe_integer<4>{10}, cnl::safe_integer<7>{70}), "cnl::safe_integer operator*");
    }

    namespace test_divide {
        static_assert(identical(cnl::safe_integer<2>{3 / 4}, cnl::safe_integer<2>{3} / cnl::safe_integer<3>{4}), "");
    }

    namespace test_is_composite {
        using cnl::is_composite;

        static_assert(is_composite<cnl::safe_integer<10>>::value, "is_composite<cnl::safe_integer<10>> test failed");
    }

    namespace test_digits {
        using cnl::digits;
        using cnl::set_digits_t;

        static_assert(digits<cnl::safe_integer<3>>::value>=3, "cnl::digits / cnl::set_digits test failed");
        static_assert(identical(set_digits_t<cnl::safe_integer<1>, 3>{6}, cnl::safe_integer<3>{6}), "cnl::digits / cnl::set_digits test failed");
    }

    namespace test_used_digits {
        using cnl::throwing_overflow_tag;

        static_assert(cnl::used_digits(cnl::safe_integer<1, throwing_overflow_tag, char>{0})==0, "");
        static_assert(cnl::used_digits(cnl::safe_integer<22, throwing_overflow_tag>{77})==7, "");
    }

    namespace test_leading_bits {
        using cnl::leading_bits;
        using cnl::throwing_overflow_tag;

        static_assert(leading_bits(cnl::safe_integer<1, throwing_overflow_tag, char>{0}) == 1, "leading_bits(cnl::safe_integer)");
        static_assert(leading_bits(cnl::safe_integer<22, throwing_overflow_tag>{77}) == 15, "leading_bits(cnl::safe_integer)");
    }

    namespace test_power_value {
        static_assert(
                identical(
                        cnl::safe_integer<9>{256},
                        cnl::_impl::power_value<cnl::safe_integer<31>, 8, 2>()),
                "");
    }

    namespace test_scale {
        static_assert(identical(cnl::safe_integer<3>{2}, cnl::_impl::scale<1>(cnl::safe_integer<2>{1})),
                "cnl::scale<..., cnl::safe_integer<>>");

        static_assert(identical(cnl::safe_integer<2>{2}, cnl::_impl::scale<1>(cnl::safe_integer<1>{1})),
                "cnl::scale<..., cnl::safe_integer<>>");

        static_assert(
                identical(
                        cnl::safe_integer<39, cnl::saturated_overflow_tag>{123*256},
                        cnl::_impl::scale<8, 2>(cnl::safe_integer<31, cnl::saturated_overflow_tag>{123})),
                "");
    }

    namespace test_shift {
        static_assert(
                identical(
                        cnl::safe_integer<2>{2},
                        cnl::safe_integer<1>{1} << cnl::constant<1>{}),
                "");
    }

#if !defined(CNL_UNREACHABLE_UB_ENABLED)
    TEST(safe_integer, conversion_overflow) {  // NOLINT
        using si = cnl::safe_integer<5>;
        ASSERT_DEATH(si{32}, "positive overflow");
    }

    TEST(safe_integer, most_negative_number) {  // NOLINT
        static_assert(cnl::safe_integer<1>{1}, "in-range boundary test");
        static_assert(cnl::safe_integer<1>{-1}, "in-range boundary test");
        ASSERT_DEATH(cnl::safe_integer<1>{-2}, "negative overflow");
    }
#endif

    TEST(safe_integer, pre_increment) {  // NOLINT
        auto a = cnl::safe_integer<3>{6};
        auto& b = ++ a;
        static_assert(
                std::is_same<decltype(b), cnl::safe_integer<3>&>::value,
                "safe_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "safe_integer pre-increment return address";
        ASSERT_EQ(7, b) << "safe_integer pre-increment";
    }

    TEST(safe_integer, pre_decrement) {  // NOLINT
        auto a = cnl::safe_integer<3>{-6};
        auto& b = -- a;
        static_assert(
                std::is_same<decltype(b), cnl::safe_integer<3>&>::value,
                "safe_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "safe_integer pre-increment return address";
        ASSERT_EQ(-7, b) << "safe_integer pre-increment";
    }

    TEST(safe_integer, post_increment) {  // NOLINT
        auto a = cnl::safe_integer<3>{6};
        auto const& b = a ++;
        static_assert(
                std::is_same<decltype(b), cnl::safe_integer<3> const&>::value,
                "safe_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "safe_integer pre-increment return address";
        ASSERT_EQ(7, a) << "safe_integer pre-increment";
        ASSERT_EQ(6, b) << "safe_integer pre-increment";
    }

    TEST(safe_integer, post_decrement) {  // NOLINT
        auto a = cnl::safe_integer<3>{-6};
        auto const& b = a --;
        static_assert(
                std::is_same<decltype(b), cnl::safe_integer<3> const&>::value,
                "safe_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "safe_integer pre-increment return address";
        ASSERT_EQ(-7, a) << "safe_integer pre-increment";
        ASSERT_EQ(-6, b) << "safe_integer pre-increment";
    }

#if !defined(CNL_UNREACHABLE_UB_ENABLED)
    TEST(safe_integer, pre_increment_overflow) {  // NOLINT
        auto a = cnl::safe_integer<3>{7};
        ASSERT_DEATH(++ a, "positive overflow");
    }

    TEST(safe_integer, pre_decrement_overflow) {  // NOLINT
        auto a = cnl::safe_integer<3>{-7};
        ASSERT_DEATH(-- a, "negative overflow");
    }

    TEST(safe_integer, post_increment_overflow) {  // NOLINT
        auto a = cnl::safe_integer<3>{7};
        ASSERT_DEATH(a ++, "positive overflow");
    }

    TEST(safe_integer, post_decrement_overflow) {  // NOLINT
        auto a = cnl::safe_integer<3>{-7};
        ASSERT_DEATH(a --, "negative overflow");
    }
#endif
}

// given a rounding tag, invokes number_test_suite for integers of all built-in types
template<int NumDigits, class OverflowTag>
struct test_safe_int {
    template<class Rep>
    using test_subject = cnl::safe_integer<NumDigits, OverflowTag, Rep>;

    constexpr static number_test_by_rep<test_subject> instance{};
};

template struct test_safe_int<1, cnl::throwing_overflow_tag>;
template struct test_safe_int<5, cnl::throwing_overflow_tag>;
template struct test_safe_int<21, cnl::throwing_overflow_tag>;

template struct test_safe_int<2, cnl::saturated_overflow_tag>;
template struct test_safe_int<8, cnl::saturated_overflow_tag>;
template struct test_safe_int<34, cnl::saturated_overflow_tag>;

template struct test_safe_int<3, cnl::native_overflow_tag>;
template struct test_safe_int<13, cnl::native_overflow_tag>;
template struct test_safe_int<55, cnl::native_overflow_tag>;
