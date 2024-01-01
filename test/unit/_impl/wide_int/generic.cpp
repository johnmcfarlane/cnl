
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for \ref cnl::wide_integer operators

#include <cnl/_impl/wide_tag/custom_operator.h>

#include <cnl/cmath.h>
#include <cnl/wide_integer.h>
#include <test.h>

namespace {
    TEST(wide_integer, multiply)  // NOLINT
    {
        using namespace cnl::literals;
        using w = cnl::wide_integer<130>;
        constexpr auto expected = w{584178474632390847141000000000000000000_wide};
        constexpr auto lhs = w{584178474632390847141_wide};
        constexpr auto rhs = w{1000000000000000000_wide};
        constexpr auto actual = lhs * rhs;
        CNL_ASSERT_EQ(expected, actual);
    }

    namespace test_equals {
        static_assert(
                identical(
                        true, cnl::custom_operator<
                                      cnl::_impl::equal_op,
                                      cnl::op_value<cnl::wide_integer<15>>,
                                      cnl::op_value<cnl::wide_integer<15>>>{}(
                                      cnl::wide_integer<15>{-3}, cnl::wide_integer<15>{-3})),
                "cnl::wide_integer equality");
        static_assert(
                identical(
                        true, cnl::custom_operator<
                                      cnl::_impl::equal_op,
                                      cnl::op_value<cnl::wide_integer<15>>,
                                      cnl::op_value<cnl::wide_integer<63>>>{}(
                                      cnl::wide_integer<15>{-3}, cnl::wide_integer<63>{-3})),
                "cnl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::wide_integer<64, unsigned>{3} == cnl::wide_integer<16, unsigned>{3}),
                "cnl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::wide_integer<64, unsigned>{3} == cnl::wide_integer<16, unsigned>{3}),
                "cnl::wide_integer equality");
    }

    namespace test_minus {
        static_assert(
                identical(
                        cnl::wide_integer<>{-3},
                        cnl::custom_operator<cnl::_impl::minus_op, cnl::op_value<int, cnl::wide_tag<31>>>{}(
                                cnl::wide_integer<>{3})),
                "cnl::wide_integer minus");
        static_assert(
                identical(cnl::wide_integer<>{-3}, -cnl::wide_integer<>{3}),
                "cnl::wide_integer minus");

#if !defined(_MSC_VER)
        static_assert(
                identical(
                        cnl::wide_integer<500, int>{-17292375928362489LL},
                        -cnl::wide_integer<500, int>{17292375928362489LL}));
#endif

        TEST(wide_integer, minus)  // NOLINT
        {
            constexpr auto expected = cnl::wide_integer<1000, int>{-17292375928362489LL};
            constexpr auto actual = -cnl::wide_integer<1000, int>{17292375928362489LL};
            CNL_ASSERT_EQ(expected, actual);
        }
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::wide_integer<>{123 + 234},
                        cnl::wide_integer<>{123} + cnl::wide_integer<>{234}),
                "cnl::wide_integer add");
        static_assert(
                identical(cnl::wide_integer<>{123 + 234}, cnl::wide_integer<>{123} + 234),
                "cnl::wide_integer add");

        static_assert(
                identical(
                        cnl::wide_integer<8, signed>{5},
                        cnl::custom_operator<
                                cnl::_impl::add_op,
                                cnl::op_value<cnl::wide_integer<8, unsigned>>,
                                cnl::op_value<cnl::wide_integer<7, signed>>>{}(
                                cnl::wide_integer<8, unsigned>{2},
                                cnl::wide_integer<7, signed>{3})),
                "cnl::wide_integer add");
        static_assert(
                identical(
                        cnl::wide_integer<8, signed>{5},
                        cnl::wide_integer<8, unsigned>{2} + cnl::wide_integer<7, signed>{3}),
                "cnl::wide_integer add");
    }

    namespace test_divide {
        static_assert(
                identical(
                        cnl::wide_integer<16, unsigned>{0x12},
                        cnl::custom_operator<
                                cnl::_impl::divide_op,
                                cnl::op_value<cnl::wide_integer<16, unsigned>>,
                                cnl::op_value<cnl::wide_integer<16, unsigned>>>{}(0x1234, 0x100)));
        static_assert(
                identical(
                        cnl::wide_integer<16, signed>{0x12},
                        cnl::custom_operator<
                                cnl::_impl::divide_op,
                                cnl::op_value<cnl::wide_integer<16, unsigned>>,
                                cnl::op_value<cnl::wide_integer<16, signed>>>{}(0x1234, 0x100)));

        TEST(wide_integer, divide)  // NOLINT
        {
            using namespace cnl::literals;
            using cnl::wide_integer;
            constexpr auto expected = wide_integer<200>{0x5555555555555555555555555555555555555555555555555_wide};

            constexpr auto nume = wide_integer<200>{1} << 196;
            constexpr auto denom = wide_integer<200>{3};
            constexpr auto actual = nume / denom;

            CNL_ASSERT_EQ(expected, actual);
        }
    }

    namespace test_shift_left {
        static_assert(
                identical(cnl::wide_integer<63>{0xAA << 1}, cnl::wide_integer<63>{0xAA} << 1),
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
                        cnl::wide_integer<16, std::int8_t>{123 << 4},
                        cnl::wide_integer<16, std::uint8_t>{123} << 4),
                "cnl::wide_integer<16, std::uint8_t> << cnl::constant");
    }
}
