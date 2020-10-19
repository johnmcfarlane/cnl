
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/type.h>

#include <cnl/_impl/duplex_integer/ctors.h>
#include <cnl/_impl/duplex_integer/operators.h>
#include <cnl/_impl/duplex_integer/type.h>
#include <cnl/cstdint.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace test_ctor {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{0x1234567890abcdef},
                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{
                                cnl::_impl::instantiate_duplex_integer_t<95, cnl::int32>{0x1234567890abcdef}}),
                "");

        TEST(duplex_integer, float_ctor)  // NOLINT
        {
            constexpr auto expected = cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{cnl::uint64(1.23456e15)};
            auto const actual = cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{1.23456e15};
            ASSERT_EQ(expected, actual);
        }
    }

    namespace test_upper {
        static_assert(
                identical(
                        cnl::int8{0},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{}.upper()),
                "");
        static_assert(
                identical(
                        cnl::int8{0},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{0}.upper()),
                "");
        static_assert(
                identical(
                        cnl::int8{0},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{650}.upper()),
                "");
        static_assert(
                identical(
                        cnl::int8{-1},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{-650}.upper()),
                "");
        static_assert(
                identical(
                        short{0},
                        cnl::_impl::duplex_integer<short, unsigned short>{123}.upper()),
                "");
        static_assert(
                identical(
                        int{-1},
                        cnl::_impl::duplex_integer<int, unsigned>{-1}.upper()),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{0},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<int, unsigned int>, cnl::_impl::duplex_integer<unsigned int, unsigned int> >{
                                0x600000000LL}.upper()),
                "");

        TEST(duplex_integer, upper_int8_value)  // NOLINT
        {
            auto expected = cnl::int8{0};
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{}.upper();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_zero)  // NOLINT
        {
            auto expected = cnl::int8{0};
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{0}.upper();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_650)  // NOLINT
        {
            auto expected = cnl::int8{0};
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{650}.upper();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_minus_650)  // NOLINT
        {
            auto expected = cnl::int8{-1};
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{-650}.upper();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_123)  // NOLINT
        {
            auto expected = cnl::int8{0};
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{123}.upper();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_minus_1)  // NOLINT
        {
            auto expected = cnl::int8{-1};
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{-1}.upper();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_nested)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned int>{0};
            auto actual = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<int, unsigned int>,
                    cnl::_impl::duplex_integer<unsigned int, unsigned int> >{0x600000000LL}.upper();
            ASSERT_EQ(expected, actual);
        }
    }

    namespace test_lower {
        static_assert(
                identical(
                        0U,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{}.lower()),
                "");
        static_assert(
                identical(
                        0U,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{0}.lower()),
                "");
        static_assert(
                identical(
                        650U,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{650}.lower()),
                "");
        static_assert(
                identical(
                        unsigned(-650),
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{-650}.lower()),
                "");
        static_assert(
                identical(
                        unsigned{cnl::numeric_limits<unsigned>::max()},
                        cnl::_impl::duplex_integer<int, unsigned>{-1}.lower()),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>{0x600000000LL},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<int, unsigned int>, cnl::_impl::duplex_integer<unsigned int, unsigned int> >{
                                0x600000000LL}.lower()),
                "");

        TEST(duplex_integer, lower_int8_value)  // NOLINT
        {
            auto expected = 0U;
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{}.lower();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_zero)  // NOLINT
        {
            auto expected = 0U;
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{0}.lower();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_650)  // NOLINT
        {
            auto expected = 650U;
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{650}.lower();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_minus_650)  // NOLINT
        {
            auto expected = static_cast<unsigned>(-650);
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{-650}.lower();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_123)  // NOLINT
        {
            auto expected = 123U;
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{123}.lower();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_minus_1)  // NOLINT
        {
            auto expected = 0xFFFFFFFFU;
            auto actual = cnl::_impl::duplex_integer<cnl::int8, unsigned>{-1}.lower();
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_nested)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<unsigned int, unsigned int>{0x600000000LL};
            auto actual = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<int, unsigned int>,
                    cnl::_impl::duplex_integer<unsigned int, unsigned int> >{0x600000000LL}.lower();
            ASSERT_EQ(expected, actual);
        }
    }

    namespace test_bool {
        static_assert(!cnl::_impl::duplex_integer<int, unsigned>{}, "");
        static_assert(!cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0}, "");
        static_assert(cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0x1234}, "");
        static_assert(cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>{0x1234}, "");
    }

    namespace test_int {
        static_assert(
                identical(
                        cnl::uint8{255},
                        static_cast<cnl::uint8>(cnl::_impl::duplex_integer<int, unsigned>{cnl::uint8{255}})),
                "");
        static_assert(
                identical(
                        cnl::int16{1},
                        static_cast<cnl::int16>(cnl::_impl::duplex_integer<int, unsigned>{1})),
                "");
        static_assert(
                identical(
                        cnl::uint16{0x5678},
                        cnl::_impl::duplex_integer<signed short, unsigned short>{0x12345678}.lower()),
                "");

        TEST(duplex_integer, lower)  // NOLINT
        {
            auto expected = cnl::uint16{0x5678};
            auto actual = cnl::_impl::duplex_integer<signed short, unsigned short>{0x12345678}.lower();
            ASSERT_EQ(expected, actual);
        }

        static_assert(
                identical(
                        cnl::uint64{0x0102},
                        static_cast<cnl::uint64>(cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0x0102})),
                "");
        static_assert(
                identical(
                        cnl::int64{0x0102},
                        static_cast<cnl::int64>(cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{0x0102})),
                "");
        static_assert(
                identical(
                        cnl::uint64{0x12345678},
                        static_cast<cnl::uint64>(cnl::_impl::duplex_integer<signed short, unsigned short>{0x12345678})),
                "");

        static_assert(static_cast<cnl::uint64>(
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{UINT64_C(0xFEDCBA9876543210)}), "");
        static_assert(
                identical(
                        cnl::uint64{0xFEDCBA9876543210},
                        static_cast<cnl::uint64>(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{UINT64_C(0xFEDCBA9876543210)})),
                "");

#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        CNL_INTMAX_C(-2029872569029388),
                        static_cast<cnl::int128>(
                                cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>{INT64_C(-2029872569029388)})),
                "");
        static_assert(
                identical(
                        CNL_INTMAX_C(0x0123456789ABCDEF),
                        static_cast<cnl::int128>(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{INT64_C(0x0123456789ABCDEF)})),
                "");
#endif
        static_assert(
                identical(
                        cnl::uint16(-1>>16),
                        static_cast<cnl::uint16>(cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{-1, 65535})),
                "");
        static_assert(
                identical(
                        INT32_C(-1),
                        static_cast<cnl::int32>(
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                                        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{-1})),
                "");
    }

    namespace test_to_rep {
        static_assert(
                identical(
                        4567U,
                        cnl::to_rep<cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<unsigned int, unsigned int>,
                                cnl::_impl::duplex_integer<unsigned int, unsigned int> >>{}(
                                        4567)),
                "");
    }

    namespace test_from_value {
        using duplex100 = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<int, unsigned int>,
                cnl::_impl::duplex_integer<unsigned int, unsigned int>>;
        static_assert(
                identical(
                        duplex100{123},
                        cnl::from_value<duplex100, duplex100>{}(duplex100{123})),
                "cnl::from_value<cnl::_impl::duplex<...>>");
    }
}
