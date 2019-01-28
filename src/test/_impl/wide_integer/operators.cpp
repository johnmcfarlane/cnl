
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/operators.h>

#include <cnl/_impl/wide_integer/operators.h>

#include <cnl/_impl/wide_integer/literals.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cmath.h>

#include <gtest/gtest.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_equals {
        static_assert(
                identical(
                        true,
                        cnl::_impl::comparison_operator<
                                cnl::_impl::equal_op,
                                cnl::_impl::wide_integer<15>,
                                cnl::_impl::wide_integer<63>>{}(
                                        cnl::_impl::wide_integer<15>{-3},
                                        cnl::_impl::wide_integer<63>{-3})),
                "cnl::_impl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::_impl::wide_integer<64, unsigned>{3}==cnl::_impl::wide_integer<16, unsigned>{3}),
                "cnl::_impl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::_impl::wide_integer<64, unsigned>{3}==cnl::_impl::wide_integer<16, unsigned>{3}),
                "cnl::_impl::wide_integer equality");
    }

    namespace test_minus {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<>{-3},
                        -cnl::_impl::wide_integer<>{3}),
                "cnl::_impl::wide_integer minus");

#if !defined(_MSC_VER)
        // requires elevated value of -fconstexpr-steps
        static_assert(identical(
                cnl::_impl::wide_integer<500, int>{-17292375928362489LL},
                -cnl::_impl::wide_integer<500, int>{17292375928362489LL}), "");
#endif

        TEST(wide_integer, minus)
        {
            auto expected = cnl::_impl::wide_integer<1000, int>{-17292375928362489LL};
            auto actual = -cnl::_impl::wide_integer<1000, int>{17292375928362489LL};
            ASSERT_EQ(expected, actual);
        }
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<>{123+234},
                        cnl::_impl::wide_integer<>{123}+cnl::_impl::wide_integer<>{234}),
                "cnl::_impl::wide_integer add");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<>{123+234},
                        cnl::_impl::wide_integer<>{123}+234),
                "cnl::_impl::wide_integer add");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<8, signed>{5},
                        cnl::_impl::wide_integer<8, unsigned>{2}+cnl::_impl::wide_integer<7, signed>{3}),
                "cnl::_impl::wide_integer add");
    }

    namespace test_divide {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<16, unsigned>{0x12},
                        cnl::_impl::binary_operator<
                                cnl::_impl::divide_op,
                                cnl::_impl::wide_integer<16, unsigned>,
                                cnl::_impl::wide_integer<16, unsigned>>{}(0x1234, 0x100)),
                "");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<16, signed>{0x12},
                        cnl::_impl::binary_operator<
                                cnl::_impl::divide_op,
                                cnl::_impl::wide_integer<16, unsigned>,
                                cnl::_impl::wide_integer<16, signed>>{}(0x1234, 0x100)),
                "");

#if !defined(_MSC_VER) && (defined(__clang__) || !defined(__GNUG__) || __GNUG__ < 7)
        TEST(wide_integer, divide)
        {
            using namespace cnl::_impl;
            auto expected = 0x5555555555555555555555555555555555555555555555555_wide;

            auto nume = wide_integer<200>{1} << 196;
            auto denom = wide_integer<200>{3};
            auto actual = nume/denom;

            ASSERT_EQ(expected, actual);
        }
#endif
    }

    namespace test_shift_left {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<63>{0xAA<<1},
                        cnl::_impl::wide_integer<63>{0xAA}<<1),
                "cnl::_impl::wide_integer<63> << int");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<255>{0x400000000LL},
                        cnl::_impl::wide_integer<255>{1} << cnl::_impl::wide_integer<6>{34}),
                "cnl::_impl::wide_integer<255> << cnl::_impl::wide_integer<6>");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<255>{0x400000000LL},
                        cnl::_impl::wide_integer<255>{1} << cnl::constant<34>{}),
                "cnl::_impl::wide_integer<255> << cnl::constant");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<16, cnl::uint8>{123 << 4},
                        cnl::_impl::wide_integer<16, cnl::uint8>{123} << 4),
                "cnl::_impl::wide_integer<16, uint8> << cnl::constant");
    }
}
