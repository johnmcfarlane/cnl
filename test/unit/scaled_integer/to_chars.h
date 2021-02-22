
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_TEST_FIXED_POINT_TO_CHARS_H)
#define CNL_TEST_FIXED_POINT_TO_CHARS_H

#include <cnl/elastic_integer.h>
#include <cnl/scaled_integer.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace {
    using cnl::_impl::identical;

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

    void test_chars(
            std::string const& expected, char const* const buffer_first,
            char const* const buffer_last)
    {
        auto const expected_length = static_cast<int>(expected.length());
        auto const actual_length = std::distance(buffer_first, buffer_last);

        ASSERT_EQ(expected_length, actual_length) << "result is wrong length";

#if defined(_MSC_VER)
#pragma warning(disable : 4365)
#endif
        ASSERT_EQ(0, expected.compare(0, actual_length, buffer_first, expected_length))
                << "result has wrong characters";
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

            test_chars(expected, buffer_begin, buffer_last);
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

            test_chars(expected, buffer_begin, result.ptr);
        }

        TEST(to_chars, scaled_integer)  // NOLINT
        {
            test<22>(
                    "-5016.5091400146484375",
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
            test<7>("0.0039", cnl::scaled_integer<int, cnl::power<-16>>(0.00390625));
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
            test<4>("-7", cnl::scaled_integer<int, cnl::power<-28>>(-7.00390625));
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

        TEST(to_chars, scaled_integer_octal_positive)  // NOLINT
        {
            test<9>("634124.25", cnl::scaled_integer<int, cnl::power<-1, 8>>(634124.25));
        }

        TEST(to_chars, scaled_integer_octal_negative)  // NOLINT
        {
            test<7>("-33.125", cnl::scaled_integer<int, cnl::power<-1, 8>>(-33.125));
        }
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_CHARS_H
