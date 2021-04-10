
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_TEST_FIXED_POINT_TO_CHARS_H)
#define CNL_TEST_FIXED_POINT_TO_CHARS_H

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/elastic_integer.h>
#include <cnl/scaled_integer.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace {
    using cnl::_impl::assert_same_v;

    namespace test_max_to_chars_chars {
        static_assert(
                4 == cnl::_impl::max_to_chars_chars<scaled_integer<uint8, cnl::power<2>>>::value,
                "cnl::_impl::max_to_chars_chars");  // 1020
        static_assert(
                7 == cnl::_impl::max_to_chars_chars<scaled_integer<int8, cnl::power<-3>>>::value,
                "cnl::_impl::max_to_chars_chars");  // -15.875
        static_assert(
                6 == cnl::_impl::max_to_chars_chars<scaled_integer<uint16, cnl::power<>>>::value,
                "cnl::_impl::max_to_chars_chars");  // 65536
        static_assert(
                41
                        == cnl::_impl::max_to_chars_chars<cnl::scaled_integer<
                                cnl::elastic_integer<41>, cnl::power<-38>>>::value,
                "cnl::_impl::max_to_chars_chars");
        static_assert(
                45
                        == cnl::_impl::max_to_chars_chars<
                                cnl::scaled_integer<int64, cnl::power<-32>>>::value,
                "cnl::_impl::max_to_chars_chars");  // −2147483647.99999999976716935634613037109375
    }

    namespace test_to_chars_natural {
        template<int BufferSize, typename Integer>
        void test(Integer const& value)
        {
            auto expected = std::to_string(static_cast<long long>(value));
            auto expected_length = expected.length();

            auto buffer = std::array<char, BufferSize>{};
            auto const buffer_begin = buffer.data();
            auto const buffer_end = buffer.data() + buffer.size();

            auto buffer_last = cnl::_impl::to_chars_natural(buffer_begin, buffer_end, value);
            if (BufferSize < expected_length) {
                ASSERT_EQ(nullptr, buffer_last);
                return;
            }

            ASSERT_EQ(expected, std::string(buffer_begin, buffer_last));
        }

        TEST(to_chars_natural, zero)  // NOLINT
        {
            test<0>(0);
        }

        TEST(to_chars_natural, one)  // NOLINT
        {
            test<1>(1);
        }

        TEST(to_chars_natural, one_too_short)  // NOLINT
        {
            test<0>(1);
        }

        TEST(to_chars_natural, nine)  // NOLINT
        {
            test<1>(9);
        }

        TEST(to_chars_natural, ten)  // NOLINT
        {
            test<2>(10);
        }

        TEST(to_chars_natural, fifty_too_short)  // NOLINT
        {
            test<1>(50);
        }

        TEST(to_chars_natural, forty_two_way_too_short)  // NOLINT
        {
            test<0>(42);
        }

        TEST(to_chars_natural, ninety_nine)  // NOLINT
        {
            test<2>(99);
        }

        TEST(to_chars_natural, one_hundred)  // NOLINT
        {
            test<3>(100);
        }

        TEST(to_chars_natural, int8_max)  // NOLINT
        {
            test<3>(cnl::numeric_limits<int8>::max());
        }

        TEST(to_chars_natural, scaled_integer)  // NOLINT
        {
            test<5>(scaled_integer<int, cnl::power<>>{12345});
        }

        TEST(to_chars_natural, large_number)  // NOLINT
        {
            test<200>(23947569375693275LL);
        }

        TEST(to_chars_natural, large_number_way_too_short)  // NOLINT
        {
            test<1>(23947569375693275LL);
        }
    }

    namespace test_to_chars {
        template<int BufferSize, typename Scalar>
        void test(std::string const& expected, Scalar value)
        {
            auto buffer = std::array<char, BufferSize>{};
            auto const buffer_begin = buffer.data();
            auto const buffer_end = buffer.data() + buffer.size();

            auto result = cnl::to_chars(buffer_begin, buffer_end, value);
            if (result.ec != std::errc{}) {
                ASSERT_EQ(buffer_end, result.ptr);
                return;
            }

            ASSERT_EQ(expected, std::string(buffer_begin, result.ptr));
        }

        TEST(to_chars, scaled_integer)  // NOLINT
        {
            test<22>(
                    "-5016.50914001464843",
                    cnl::scaled_integer<int, cnl::power<-16>>(-5016.5091400146484375));
        }

        TEST(to_chars, scaled_integer_truncated)  // NOLINT
        {
            test<8>("5016.509",
                    cnl::scaled_integer<unsigned, cnl::power<-16>>(5016.5091400146484375));
        }

        TEST(to_chars, scaled_integer_just_big_enough)  // NOLINT
        {
            test<5>("-5016", cnl::scaled_integer<short, cnl::power<-2>>(-5016.5091400146484375));
        }

        TEST(to_chars, scaled_integer_too_small)  // NOLINT
        {
            test<4>("", cnl::scaled_integer<short, cnl::power<-2>>(-5016.5091400146484375));
        }

        TEST(to_chars, scaled_integer_small)  // NOLINT
        {
            test<7>(".003906", cnl::scaled_integer<int, cnl::power<-16>>(0.00390625));
        }

        TEST(to_chars, scaled_integer_small_single_digit)  // NOLINT
        {
            test<1>("0", cnl::scaled_integer<int, cnl::power<-16>>(0.00390625));
        }

        TEST(to_chars, scaled_integer_small_double_digit)  // NOLINT
        {
            test<2>("0", cnl::scaled_integer<int, cnl::power<-16>>(0.00390625));
        }

        TEST(to_chars, scaled_integer_small_tripple_digit)  // NOLINT
        {
            test<3>("0", cnl::scaled_integer<int, cnl::power<-16>>(0.00390625));
        }

        TEST(to_chars, scaled_integer_small_negative_single_digit)  // NOLINT
        {
            test<1>("", cnl::scaled_integer<int, cnl::power<-16>>(-0.00390625));
        }

        TEST(to_chars, scaled_integer_small_negative_double_digit)  // NOLINT
        {
            test<2>("-0", cnl::scaled_integer<int, cnl::power<-16>>(-0.00390625));
        }

        TEST(to_chars, scaled_integer_small_negative_tripple_digit)  // NOLINT
        {
            test<3>("-0", cnl::scaled_integer<int, cnl::power<-16>>(-0.00390625));
        }

        TEST(to_chars, scaled_integer_small_negative_quadruple_digit)  // NOLINT
        {
            test<4>("-0", cnl::scaled_integer<int, cnl::power<-16>>(-0.00390625));
        }

        TEST(to_chars, scaled_integer_very_few_integer_digits)  // NOLINT
        {
            test<4>("-7.0", cnl::scaled_integer<int, cnl::power<-28>>(-7.00390625));
        }

        TEST(to_chars, scaled_integer_decimal_positive)  // NOLINT
        {
            test<6>("17.917", cnl::scaled_integer<int, cnl::power<-3, 10>>(17.917));
        }

        TEST(to_chars, scaled_integer_decimal_negative)  // NOLINT
        {
            test<5>("-5.25", cnl::scaled_integer<int, cnl::power<-2, 10>>(-5.25));
        }

        TEST(to_chars, scaled_integer_decimal_no_fractional)  // NOLINT
        {
            test<7>("-517523", cnl::scaled_integer<int, cnl::power<0, 10>>(-517523));
        }

        TEST(to_chars, scaled_integer_binary_negative_highly_fractional)  // NOLINT
        {
            test<7>("-4.e-11", cnl::scaled_integer<int, cnl::power<-40>>{-.00000000005});
        }

        TEST(to_chars, scaled_integer_binary_positive_highly_fractional)  // NOLINT
        {
            test<7>("4.9e-11", cnl::scaled_integer<int, cnl::power<-40>>{.00000000005});
        }

        TEST(to_chars, scaled_integer_binary_highly_fractional10)  // NOLINT
        {
            test<10>("4.9112e-11", cnl::scaled_integer<int, cnl::power<-40>>{.00000000005});
        }

        TEST(to_chars, scaled_integer_octal_positive)  // NOLINT
        {
            test<9>("634124.25", cnl::scaled_integer<int, cnl::power<-1, 8>>(634124.25));
        }

        TEST(to_chars, scaled_integer_octal_negative)  // NOLINT
        {
            test<7>("-33.125", cnl::scaled_integer<int, cnl::power<-1, 8>>(-33.125));
        }

        TEST(to_chars, scaled_integer_ternary)  // NOLINT
        {
            test<7>(".666666", cnl::scaled_integer<int, cnl::power<-1, 3>>(2) / 3);
        }
    }

    namespace test_descale {
        TEST(descale, 31_positive_negexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{125, -2}};
            auto const ac{cnl::_impl::descale<int32>(20, cnl::power<-4>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_positive_tinyexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{491127127, -19}};
            auto const ac{cnl::_impl::descale<int32>(54, cnl::power<-40>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_positive_posexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{48, 1}};
            auto const ac{cnl::_impl::descale<int32>(30, cnl::power<4>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_negative_negexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{-125, -2}};
            auto const ac{cnl::_impl::descale<int32>(-20, cnl::power<-4>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_negative_posexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{-48, 1}};
            auto const ac{cnl::_impl::descale<int32>(-30, cnl::power<4>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 7_negative_tinyexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int8, 10>{-1, -1}};
            auto const ac{cnl::_impl::descale<int8>(int8{-127}, cnl::power<-10>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(int{ex.significand}, int{ac.significand});
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 8_positive_tinyexp)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int64, 10>{2490234375, -10}};
            auto const input{uint8{255}};
            auto const ac{cnl::_impl::descale<int64>(input, cnl::power<-10>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_positive_2525)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{2525, -2}};
            auto const ac{cnl::_impl::descale<int32>(26476544, cnl::power<-20>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_positive_5016)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int32, 10>{-501650907, -5}};
            auto const ac{cnl::_impl::descale<int32>(-328761943, cnl::power<-16>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }

        TEST(descale, 31_positive_5016b)  // NOLINT
        {
            auto const ex{cnl::_impl::descaled<int64, 10>{-501650914001464843LL, -14}};
            auto const ac{cnl::_impl::descale<int64>(-328761943, cnl::power<-16>{})};
            static_assert(assert_same_v<decltype(ex), decltype(ac)>);
            ASSERT_EQ(ex.significand, ac.significand);
            ASSERT_EQ(ex.exponent, ac.exponent);
        }
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_CHARS_H
