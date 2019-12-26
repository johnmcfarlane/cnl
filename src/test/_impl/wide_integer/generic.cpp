
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/operators.h>

#include <cnl/_impl/wide_tag/generic.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cmath.h>
#include <cnl/wide_integer.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace test_equals {
        static_assert(
                identical(
                        true,
                        cnl::comparison_operator<
                                cnl::_impl::equal_op,
                                cnl::wide_integer<15>,
                                cnl::wide_integer<15>>{}(
                                cnl::wide_integer<15>{-3},
                                cnl::wide_integer<15>{-3})),
                "cnl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::comparison_operator<
                                cnl::_impl::equal_op,
                                cnl::wide_integer<15>,
                                cnl::wide_integer<63>>{}(
                                        cnl::wide_integer<15>{-3},
                                        cnl::wide_integer<63>{-3})),
                "cnl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::wide_integer<64, unsigned>{3}==cnl::wide_integer<16, unsigned>{3}),
                "cnl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::wide_integer<64, unsigned>{3}==cnl::wide_integer<16, unsigned>{3}),
                "cnl::wide_integer equality");
    }

    namespace test_minus {
        static_assert(
                identical(
                        cnl::wide_integer<>{-3},
                        cnl::unary_operator<cnl::_impl::minus_op, cnl::wide_tag<31>, int>{}(cnl::wide_integer<>{3})),
                "cnl::wide_integer minus");
        static_assert(
                identical(
                        cnl::wide_integer<>{-3},
                        -cnl::wide_integer<>{3}),
                "cnl::wide_integer minus");

#if !defined(_MSC_VER)
        // requires elevated value of -fconstexpr-steps
        static_assert(identical(
                cnl::wide_integer<500, int>{-17292375928362489LL},
                -cnl::wide_integer<500, int>{17292375928362489LL}), "");
#endif

#if !defined(__arm__) && defined(__clang__)
        TEST(wide_integer, minus)  // NOLINT
        {
            auto expected = cnl::wide_integer<1000, int>{-17292375928362489LL};
            auto actual = -cnl::wide_integer<1000, int>{17292375928362489LL};
            ASSERT_EQ(expected, actual);
        }
#endif
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::wide_integer<>{123+234},
                        cnl::wide_integer<>{123}+cnl::wide_integer<>{234}),
                "cnl::wide_integer add");
        static_assert(
                identical(
                        cnl::wide_integer<>{123+234},
                        cnl::wide_integer<>{123}+234),
                "cnl::wide_integer add");

        static_assert(
                identical(
                        cnl::wide_integer<8, signed>{5},
                        cnl::binary_operator<
                                cnl::_impl::add_op,
                                cnl::_impl::native_tag,
                                cnl::_impl::native_tag,
                                cnl::wide_integer<8, unsigned>,
                                cnl::wide_integer<7, signed>>{}(
                                        cnl::wide_integer<8, unsigned>{2},
                                        cnl::wide_integer<7, signed>{3})),
                "cnl::wide_integer add");
        static_assert(
                identical(
                        cnl::wide_integer<8, signed>{5},
                        cnl::wide_integer<8, unsigned>{2}+cnl::wide_integer<7, signed>{3}),
                "cnl::wide_integer add");
    }

    namespace test_divide {
        static_assert(
                identical(
                        cnl::wide_integer<16, unsigned>{0x12},
                        cnl::binary_operator<
                                cnl::_impl::divide_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::wide_integer<16, unsigned>,
                                cnl::wide_integer<16, unsigned>>{}(0x1234, 0x100)),
                "");
        static_assert(
                identical(
                        cnl::wide_integer<16, signed>{0x12},
                        cnl::binary_operator<
                                cnl::_impl::divide_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::wide_integer<16, unsigned>,
                                cnl::wide_integer<16, signed>>{}(0x1234, 0x100)),
                "");

#if defined(__clang__)
        TEST(wide_integer, divide)  // NOLINT
        {
            using namespace cnl::literals;
            using cnl::wide_integer;
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
                        cnl::wide_integer<63>{0xAA<<1},
                        cnl::wide_integer<63>{0xAA}<<1),
                "cnl::wide_integer<63> << int");
        static_assert(
                identical(
                        cnl::wide_integer<255>{0x400000000LL},
                        cnl::wide_integer<255>{1} << cnl::wide_integer<6>{34}),
                "cnl::wide_integer<255> << cnl::wide_integer<6>");
        static_assert(
                identical(
                        cnl::wide_integer<255>{0x400000000LL},
                        cnl::wide_integer<255>{1} << cnl::constant<34>{}),
                "cnl::wide_integer<255> << cnl::constant");

        static_assert(
                identical(
                        cnl::wide_integer<16, cnl::int8>{123 << 4},
                        cnl::wide_integer<16, cnl::uint8>{123} << 4),
                "cnl::wide_integer<16, uint8> << cnl::constant");
    }
}
