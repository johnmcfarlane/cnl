
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/definition.h>

#include <cnl/_impl/duplex_integer/definition.h>

#include <cnl/_impl/duplex_integer/ctors.h>
#include <cnl/_impl/duplex_integer/operators.h>
#include <cnl/cstdint.h>

#include <test.h>

#include <limits>

namespace {
    namespace test_ctor {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{0x1234567890abcdef},
                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{
                                cnl::_impl::narrowest_integer_t<95, std::int32_t>{
                                        0x1234567890abcdef}}));

        TEST(duplex_integer, float_ctor)  // NOLINT
        {
            constexpr auto expected =
                    cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{static_cast<std::uint64_t>(1.23456e15)};
            auto const actual = cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{1.23456e15};
            CNL_ASSERT_EQ(expected, actual);
        }
    }

    namespace test_upper {
        static_assert(
                identical(std::int8_t{0}, cnl::_impl::duplex_integer<std::int8_t, unsigned>{}.upper()));
        static_assert(
                identical(std::int8_t{0}, cnl::_impl::duplex_integer<std::int8_t, unsigned>{0}.upper()));
        static_assert(
                identical(
                        std::int8_t{0}, cnl::_impl::duplex_integer<std::int8_t, unsigned>{650}.upper()));
        static_assert(
                identical(
                        std::int8_t{-1},
                        cnl::_impl::duplex_integer<std::int8_t, unsigned>{-650}.upper()));
        static_assert(
                identical(short{0}, cnl::_impl::duplex_integer<short, unsigned short>{123}.upper()));
        static_assert(
                identical(int{-1}, cnl::_impl::duplex_integer<int, unsigned>{-1}.upper()));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{0},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                cnl::_impl::duplex_integer<unsigned int, unsigned int>>{
                                0x600000000LL}
                                .upper()));

        TEST(duplex_integer, upper_int8_value)  // NOLINT
        {
            auto expected = std::int8_t{0};
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{}.upper();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_zero)  // NOLINT
        {
            auto expected = std::int8_t{0};
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{0}.upper();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_650)  // NOLINT
        {
            auto expected = std::int8_t{0};
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{650}.upper();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_minus_650)  // NOLINT
        {
            auto expected = std::int8_t{-1};
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{-650}.upper();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_123)  // NOLINT
        {
            auto expected = std::int8_t{0};
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{123}.upper();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_int8_minus_1)  // NOLINT
        {
            auto expected = std::int8_t{-1};
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{-1}.upper();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, upper_nested)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned int>{0};
            auto actual =
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<int, unsigned int>,
                            cnl::_impl::duplex_integer<unsigned int, unsigned int>>{0x600000000LL}
                            .upper();
            CNL_ASSERT_EQ(expected, actual);
        }
    }

    namespace test_lower {
        static_assert(identical(0U, cnl::_impl::duplex_integer<std::int8_t, unsigned>{}.lower()));
        static_assert(
                identical(0U, cnl::_impl::duplex_integer<std::int8_t, unsigned>{0}.lower()));
        static_assert(
                identical(650U, cnl::_impl::duplex_integer<std::int8_t, unsigned>{650}.lower()));
        static_assert(
                identical(
                        static_cast<unsigned>(-650),
                        cnl::_impl::duplex_integer<std::int8_t, unsigned>{-650}.lower()));
        static_assert(
                identical(
                        unsigned{std::numeric_limits<unsigned>::max()},
                        cnl::_impl::duplex_integer<int, unsigned>{-1}.lower()));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>{0x600000000LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                cnl::_impl::duplex_integer<unsigned int, unsigned int>>{
                                0x600000000LL}
                                .lower()));

        TEST(duplex_integer, lower_int8_value)  // NOLINT
        {
            auto expected = 0U;
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_zero)  // NOLINT
        {
            auto expected = 0U;
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{0}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_650)  // NOLINT
        {
            auto expected = 650U;
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{650}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_minus_650)  // NOLINT
        {
            auto expected = static_cast<unsigned>(-650);
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{-650}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_123)  // NOLINT
        {
            auto expected = 123U;
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{123}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_int8_minus_1)  // NOLINT
        {
            auto expected = 0xFFFFFFFFU;
            auto actual = cnl::_impl::duplex_integer<std::int8_t, unsigned>{-1}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, lower_nested)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<unsigned int, unsigned int>{0x600000000LL};
            auto actual =
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<int, unsigned int>,
                            cnl::_impl::duplex_integer<unsigned int, unsigned int>>{0x600000000LL}
                            .lower();
            CNL_ASSERT_EQ(expected, actual);
        }
    }

    namespace test_bool {
        static_assert(!cnl::_impl::duplex_integer<int, unsigned>{});
        static_assert(!cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>{0});
        static_assert(cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>{0x1234});
        static_assert(cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>{0x1234});
    }

    namespace test_int {
        static_assert(
                identical(
                        std::uint8_t{255},
                        static_cast<std::uint8_t>(cnl::_impl::duplex_integer<int, unsigned>{
                                std::uint8_t{255}})));
        static_assert(
                identical(
                        std::int16_t{1},
                        static_cast<std::int16_t>(cnl::_impl::duplex_integer<int, unsigned>{1})));
        static_assert(
                identical(
                        std::uint16_t{0x5678},
                        cnl::_impl::duplex_integer<signed short, unsigned short>{0x12345678}
                                .lower()));

        TEST(duplex_integer, lower)  // NOLINT
        {
            auto expected = std::uint16_t{0x5678};
            auto actual =
                    cnl::_impl::duplex_integer<signed short, unsigned short>{0x12345678}.lower();
            CNL_ASSERT_EQ(expected, actual);
        }

        static_assert(
                identical(
                        std::uint64_t{0x0102},
                        static_cast<std::uint64_t>(cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>{
                                0x0102})));
        static_assert(
                identical(
                        std::int64_t{0x0102},
                        static_cast<std::int64_t>(cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{
                                0x0102})));
        static_assert(
                identical(
                        std::uint64_t{0x12345678},
                        static_cast<std::uint64_t>(
                                cnl::_impl::duplex_integer<signed short, unsigned short>{
                                        0x12345678})));

        static_assert(
                static_cast<std::uint64_t>(cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{
                        UINT64_C(0xFEDCBA9876543210)}));
        static_assert(
                identical(
                        std::uint64_t{0xFEDCBA9876543210},
                        static_cast<std::uint64_t>(
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{
                                        UINT64_C(0xFEDCBA9876543210)})));

#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        CNL_INTMAX_C(-2029872569029388),
                        static_cast<cnl::int128_t>(
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{
                                        INT64_C(-2029872569029388)})));
        static_assert(
                identical(
                        CNL_INTMAX_C(0x0123456789ABCDEF),
                        static_cast<cnl::int128_t>(
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{
                                        INT64_C(0x0123456789ABCDEF)})));
#endif
        static_assert(
                identical(
                        static_cast<std::uint16_t>(-1 >> 16),
                        static_cast<std::uint16_t>(
                                cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{-1, 65535})));
        static_assert(
                identical(
                        INT32_C(-1),
                        static_cast<std::int32_t>(
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{-1})));
    }

    namespace test_to_rep {
        static_assert(
                identical(
                        4567U,
                        cnl::to_rep<cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<unsigned int, unsigned int>,
                                cnl::_impl::duplex_integer<unsigned int, unsigned int>>>{}(4567)));
    }
}
