
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_TEST_FIXED_POINT_TO_CHARS_H
#define CNL_TEST_FIXED_POINT_TO_CHARS_H

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <string>

namespace {
    using cnl::_impl::identical;

    void test_chars(
            std::string const& expected,
            char const* const buffer_first,
            char const* const buffer_last)
    {
        auto const expected_length = static_cast<int>(expected.length());
        auto const actual_length = std::distance(buffer_first, buffer_last);

        ASSERT_EQ(expected_length, actual_length)
                                << "result is wrong length";

        ASSERT_EQ(0, expected.compare(
                0, actual_length,
                buffer_first, expected_length))
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

            auto buffer_last = cnl::_impl::to_chars_natural(buffer_begin + 20, buffer_end + 20, value);
            if (BufferSize<expected_length) {
                ASSERT_EQ(nullptr, buffer_last);
                return;
            }

            test_chars(expected, buffer_begin, buffer_last);
        }

        TEST(to_chars_natural, zero)
        {
            test<0>(0);
        }

        TEST(to_chars_natural, one)
        {
            test<1>(1);
        }

        TEST(to_chars_natural, one_too_short)
        {
            test<0>(1);
        }

        TEST(to_chars_natural, nine)
        {
            test<1>(9);
        }

        TEST(to_chars_natural, ten)
        {
            test<2>(10);
        }

        TEST(to_chars_natural, fifty_too_short)
        {
            test<1>(50);
        }

        TEST(to_chars_natural, fourty_two_way_too_short)
        {
            test<0>(42);
        }

        TEST(to_chars_natural, ninety_nine)
        {
            test<2>(99);
        }

        TEST(to_chars_natural, one_hundred)
        {
            test<3>(100);
        }

        TEST(to_chars_natural, int8_max)
        {
            test<3>(cnl::numeric_limits<int8>::max());
        }

        TEST(to_chars_natural, fixed_point)
        {
            test<5>(fixed_point<int, 0>{12345});
        }

        TEST(to_chars_natural, large_number)
        {
            test<200>(23947569375693275LL);
        }

        TEST(to_chars_natural, large_number_way_too_short)
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
            if (result.ec!=std::errc{}) {
                ASSERT_EQ(buffer_end, result.ptr);
                return;
            }

            test_chars(expected, buffer_begin, result.ptr);
        }

        TEST(to_chars, fixed_point)
        {
            test<22>("-5016.5091400146484375", cnl::fixed_point<int, -16>(-5016.5091400146484375));
        }

        TEST(to_chars, fixed_point_truncated)
        {
            test<8>("5016.509", cnl::fixed_point<unsigned, -16>(5016.5091400146484375));
        }

        TEST(to_chars, fixed_point_just_big_enough)
        {
            test<5>("-5016", cnl::fixed_point<short, -2>(-5016.5091400146484375));
        }

        TEST(to_chars, fixed_point_too_small)
        {
            test<4>("", cnl::fixed_point<short, -2>(-5016.5091400146484375));
        }

        TEST(to_chars, fixed_point_small)
        {
            test<7>("0.00390", cnl::fixed_point<int, -16>(0.00390625));
        }

        TEST(to_chars, fixed_point_small_single_digit)
        {
            test<1>("0", cnl::fixed_point<int, -16>(0.00390625));
        }

        TEST(to_chars, fixed_point_small_double_digit)
        {
            test<2>("0", cnl::fixed_point<int, -16>(0.00390625));
        }

        TEST(to_chars, fixed_point_small_tripple_digit)
        {
            test<3>("0.0", cnl::fixed_point<int, -16>(0.00390625));
        }

        TEST(to_chars, fixed_point_small_negative_single_digit)
        {
            test<1>("", cnl::fixed_point<int, -16>(-0.00390625));
        }

        TEST(to_chars, fixed_point_small_negative_double_digit)
        {
            test<2>("-0", cnl::fixed_point<int, -16>(-0.00390625));
        }

        TEST(to_chars, fixed_point_small_negative_tripple_digit)
        {
            test<3>("-0", cnl::fixed_point<int, -16>(-0.00390625));
        }

        TEST(to_chars, fixed_point_small_negative_quadruple_digit)
        {
            test<4>("-0.0", cnl::fixed_point<int, -16>(-0.00390625));
        }

        TEST(to_chars, fixed_point_very_few_integer_digits)
        {
            test<4>("-7.0", cnl::fixed_point<int, -28>(-7.00390625));
        }
    }
}

#endif  // CNL_TEST_FIXED_POINT_TO_CHARS_H
