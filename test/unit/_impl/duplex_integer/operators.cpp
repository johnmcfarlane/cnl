
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/operators.h>

#include <cnl/_impl/duplex_integer/ctors.h>
#include <cnl/_impl/duplex_integer/operators.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace test_not {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{
                            UINT64_C(0xFFFFFFFFFFFFFFFF), UINT64_C(0xFFFFFFFFFFFFFFFF)},
                        ~cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{
                                UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000000)}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>,
                                cnl::uint64>{{UINT64_C(0xFFFFFFFFFFFFFFFF), UINT64_C(0xFFFFFFFFFFFFFFFF)},
                                             UINT64_C(0xFFFFFFFFFFFFFFFF)},
                        ~cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>,
                                cnl::uint64>{{UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000000)},
                                             UINT64_C(0x0000000000000000)}),
                "");
    }

    namespace test_minus {
        static_assert(identical(
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{-1},
                -cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{1}), "");
        static_assert(identical(
                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{-1729348762983LL},
                -cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{1729348762983LL}), "");
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{-1},
                -cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{1}), "");

        TEST(duplex_integer, minus)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                    cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{-1};
            auto actual = -cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                    cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{1};
            ASSERT_EQ(expected, actual);
        }
#endif
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234+0x7625},
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234}
                                +cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234+0x7625},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                +cnl::_impl::duplex_integer<int, unsigned>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{123+654},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{123}
                                +cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{654}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL*928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL*928178263LL}
                                +cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{0}),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{39487298182LL+8238723LL},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{39487298182LL}
                                +cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{8238723LL}),
                "");
#endif
    }

    namespace test_subtract {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234-0x7625},
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234}
                                -cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x7625-0x1234},
                        cnl::_impl::duplex_integer<int, unsigned>{0x7625}
                                -cnl::_impl::duplex_integer<int, unsigned>{0x1234}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234LL-0x7625LL},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                -cnl::_impl::duplex_integer<int, unsigned>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::uint32>{2315645440ULL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::uint32>{5000000000ULL}
                                -cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                        cnl::uint32>{2684354560ULL}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{{0, 0}, {0x40000000, 0}},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{{0, 1}, {0, 0}}
                                -cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{
                                                {0, 0}, {0xC0000000, 0}}), "");
    }

    namespace test_multiply {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234*0x7625},
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234}
                                *cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234*0x7625},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                *cnl::_impl::duplex_integer<int, unsigned>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL*928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872}
                        *cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{928178263}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL*928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL}
                        *cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{928178263LL}),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>,
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{786192872LL*928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>,
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{786192872}
                        *cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>,
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{928178263}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{786192872*928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{786192872}
                        *cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{928178263LL}),
                "");
#endif
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL*928178263LL},
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{786192872LL}
                        *cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{928178263LL}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{786192872LL*928178263LL},
                        786192872LL
                                *cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{928178263LL}),
                "");
    }

    namespace test_long_multiply {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{}(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{7},
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>>{}(
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7},
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{}(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{7},
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{}(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{7},
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{13}
                        )),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{}(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{7},
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>>{}(
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7},
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{}(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{7},
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{}(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{7},
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{13}
                        )),
                "");
#endif
    }

    namespace test_divide {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x12},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                /cnl::_impl::duplex_integer<int, unsigned>{0x100}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >{1000000000ULL},
                        cnl::binary_operator<
                                cnl::_impl::divide_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >>{}(5000000000ULL, 5)),
                "");
        static_assert(identical(0x12, 0x1234/cnl::_impl::duplex_integer<int, unsigned>{0x100}), "");

        TEST(duplex_integer, divide)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned>{0x12};
            auto actual = cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                    /cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, int_divide_by_duplex)  // NOLINT
        {
            auto expected = 0x12;
            auto actual = 0x1234/cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, divide128)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                    cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>;
            constexpr auto exponent = 64;

            auto expected = type({{0, 0}, {0x55555555, 0x55555555}});

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator/denominator;

            ASSERT_EQ((expected), (actual));
        }

        TEST(duplex_integer, divide160)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                            cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>,
                    cnl::uint32>;
            constexpr auto exponent = 96;

            auto expected = type({{0, 0}, {0x55555555, 0x55555555}}, 0x55555555);

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator/denominator;

            ASSERT_EQ((expected), (actual));
        }

#if defined(CNL_INT128_ENABLED)
        TEST(duplex_integer, divide255)  // NOLINT
        {
            using duplex_integer =
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                            cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>;

            auto expected = duplex_integer{
                {INT64_C(0x031C71C71C71C71C), UINT64_C(0x71C71C71C71C71C7)},
                {UINT64_C(0x1C71C71C71C71C71), UINT64_C(0xC71C71C71C71C71C)}
            };

            auto nume = duplex_integer(7) << 250;
            auto denom = duplex_integer(9);
            auto actual = nume/denom;

            ASSERT_EQ(expected, actual);
        }
#endif
    }

    namespace test_modulo {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x34},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                %cnl::_impl::duplex_integer<int, unsigned>{0x100}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >{0},
                        cnl::binary_operator<
                                cnl::_impl::modulo_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >>{}(5000000000ULL, 5)),
                "");
        static_assert(identical(0x12, 0x1234%cnl::_impl::duplex_integer<int, unsigned>{0x100}), "");

        TEST(duplex_integer, modulo)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned>{0x34};
            auto actual = cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                    %cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, int_modulo_by_duplex)  // NOLINT
        {
            auto expected = 0x12;
            auto actual = 0x1234%cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, modulo128)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                    cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>;
            constexpr auto exponent = 64;

            auto expected = type({{0, 0}, {0, 1}});

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator%denominator;

            ASSERT_EQ((expected), (actual));
        }

        TEST(duplex_integer, modulo160)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>,
                            cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>,
                    cnl::uint32>;
            constexpr auto exponent = 96;

            auto expected = type({{0, 0}, {0, 0}}, 1);

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator%denominator;

            ASSERT_EQ((expected), (actual));
        }

#if defined(CNL_INT128_ENABLED)
        TEST(duplex_integer, modulo255)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>,
                    cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>;
            auto expected = type{{0, 0}, {0, 7}};

            auto const numerator = type{{0, 0}, {76, UINT64_C(5242979323791729901)}};
            auto const denominator = type{10};
            auto actual = numerator%denominator;

            ASSERT_EQ((expected), (actual));
        }
#endif
    }

    namespace test_bitwise_and {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<unsigned, unsigned>{0xa005},
                        cnl::_impl::duplex_integer<unsigned, unsigned>{0xaa55}
                                &cnl::_impl::duplex_integer<unsigned, unsigned>{0xa5a5}), "");
    }

    namespace test_equal {
        static_assert(
                cnl::_impl::duplex_integer<int, unsigned>{0}==cnl::_impl::duplex_integer<int, unsigned>{},
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}==
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123}==
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123}==
                        cnl::_impl::duplex_integer<short, unsigned short>{-124}),
                "");
    }

    namespace test_not_equal {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0}!=cnl::_impl::duplex_integer<int, unsigned>{}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}!=
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{512}!=
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{1024},
                "");
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123}!=
                        cnl::_impl::duplex_integer<short, unsigned short>{-124},
                "");

        static_assert(cnl::_impl::duplex_integer<short, unsigned short>{123}!=cnl::int8{-124}, "");
        static_assert(cnl::int64{-12356}<cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{12356}, "");
    }

    namespace test_less {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0}<cnl::_impl::duplex_integer<int, unsigned>{}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}<
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123}<
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123}<
                        cnl::_impl::duplex_integer<short, unsigned short>{-124}),
                "");
    }

    namespace test_greater_than_or_equal {
        static_assert(
                cnl::_impl::duplex_integer<int, unsigned>{0}>=cnl::_impl::duplex_integer<int, unsigned>{},
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}>=
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123}>=
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123}>=
                        cnl::_impl::duplex_integer<short, unsigned short>{-124},
                "");
        static_assert(
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32> >{123}
                >=cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{-124},
                "");
    }

    namespace test_bitwise_and_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{0x1234 & 0x5858},
                        cnl::binary_operator<
                                cnl::_impl::bitwise_and_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>>{}(0x1234, 0x5858)),
                "");
    }

    namespace test_shift_left_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{0},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<signed, unsigned>,
                                cnl::_impl::duplex_integer<signed, unsigned>>{}(0, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x2468ACE0},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>,
                                cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>>{}(0x12345670, 1)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{1},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<signed, unsigned>,
                                cnl::_impl::duplex_integer<signed, unsigned>>{}(1, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{246},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<signed, unsigned>,
                                cnl::_impl::duplex_integer<signed, unsigned>>{}(
                                cnl::_impl::duplex_integer<signed, unsigned>{123},
                                cnl::_impl::duplex_integer<signed, unsigned>{1})),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, cnl::_impl::duplex_integer<unsigned, unsigned>>{0},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<signed, cnl::_impl::duplex_integer<unsigned, unsigned>>,
                                int>{}(0, 1)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{1LL << 34},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                int>{}(1, 34)),
                "");

        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<int, unsigned int>, cnl::_impl::duplex_integer<unsigned int, unsigned int>>{
                                1LL << 34},
                        cnl::shift_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int> >,
                                int>{}(1, 34)),
                "");
    }

    namespace test_shift_right_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0},
                        cnl::shift_operator<
                                cnl::_impl::shift_right_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>,
                                int>{}(0, 9)), "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{0x7394ULL >> 0},
                        cnl::shift_operator<
                                cnl::_impl::shift_right_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>>{}(0x7394ULL, 0)),
                "");
    }

    namespace test_shift_left {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{246},
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{123} << 1),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{246},
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{123}
                                << cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{1}),
                "");
    }

    namespace test_shift_right {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                unsigned int>{0x1234},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                unsigned int>{0x1234567890ABCDEFLL}
                                >> 48),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                unsigned int>{0},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                unsigned int>{0x1234567890ABCDEFLL}
                                >> 64),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{{0x01234567890ABCDELL, 0xFFEDCBA987654321ULL}, 0x08765432U},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{{0x1234567890ABCDEFLL, 0xFEDCBA9876543210ULL}, 0x87654321U}
                                >> 4),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{{0x0000000012345678LL, 0x90ABCDEFFEDCBA98ULL}, 0x76543210U},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{{0x1234567890ABCDEFLL, 0xFEDCBA9876543210ULL}, 0x87654321U}
                                >> 32),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{{0x0000000000000000LL, 0x1234567890ABCDEFLL}, 0xFEDCBA98U},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{{0x1234567890ABCDEFLL, 0xFEDCBA9876543210ULL}, 0x87654321U}
                                >> 64),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{123},
                        cnl::_impl::duplex_integer<signed, unsigned>{246} >> 1),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{123},
                        cnl::_impl::duplex_integer<signed, unsigned>{246}
                                >> cnl::_impl::duplex_integer<signed, unsigned>{1}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{0x007FFFFF, 0xFFFFFFFF},
                        cnl::_impl::duplex_integer<int, unsigned int>{0x7FFFFFFF, 0xFFFFFFFF} >> 8),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{-1},
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{-1, 0x40000000} >> 32), "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::uint32>{{-1, 0xFFFFFFFF}, 0x40000000},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                cnl::uint32>{{-1, 0x40000000}, 0} >> 32), "");
    }

    namespace test_sensible_right_shift {
        static_assert(
                identical(
                        cnl::int32(-1),
                        cnl::_impl::sensible_right_shift<cnl::int32>(INT32_C(-1), 32)),
                "");
        static_assert(
                identical(
                        cnl::uint16(-1 >> 16),
                        cnl::_impl::sensible_right_shift<cnl::uint16>(
                                cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{-1, 65535}, 16)),
                "");
        static_assert(identical(short{-1}, cnl::_impl::sensible_right_shift<short>(-1, 0)), "");
        static_assert(identical(
                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{-1},
                cnl::_impl::sensible_right_shift<cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>>(
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{-1, 0x40000000},
                        32)), "");
    }
}
