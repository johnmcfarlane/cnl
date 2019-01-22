
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/operators.h>

#include <cnl/_impl/wide_integer/operators.h>

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

        TEST(wide_integer, divide)
        {
            using type = cnl::_impl::wide_integer<200>;
            using rep = typename type::rep;
            auto expected = type{rep{
#if defined(CNL_INT128_ENABLED)
                    {INT64_C(5), UINT64_C(0x5555555555555555)},
                    {UINT64_C(0x5555555555555555), UINT64_C(0x5555555555555555)}
#else
                    {{UINT32_C(5), UINT32_C(0x55555555)}, {UINT32_C(0x55555555), UINT32_C(0x55555555)}},
                    {{UINT32_C(0x55555555), UINT32_C(0x55555555)}, UINT32_C(0x55555555)}
#endif
            }};

            auto nume = type{1} << 196;
            auto denom = type{3};
            auto actual = nume/denom;

            ASSERT_EQ(expected, actual);
        }
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
