
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
                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{
                                UINT64_C(0xFFFFFFFFFFFFFFFF), UINT64_C(0xFFFFFFFFFFFFFFFF)},
                        ~cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{
                                UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000000)}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>, std::uint64_t>{
                                {UINT64_C(0xFFFFFFFFFFFFFFFF), UINT64_C(0xFFFFFFFFFFFFFFFF)},
                                UINT64_C(0xFFFFFFFFFFFFFFFF)},
                        ~cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>, std::uint64_t>{
                                {UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000000)},
                                UINT64_C(0x0000000000000000)}));
    }

    namespace test_minus {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{-1},
                        -cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{1}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{-1729348762983LL},
                        -cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{1729348762983LL}));
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{-1},
                        -cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{1}));

        TEST(duplex_integer, minus)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                    cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{-1};
            auto actual = -cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                    cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{1};
            ASSERT_EQ(expected, actual);
        }
#endif
    }

    TEST(duplex_integer, add)  // NOLINT
    {
        using namespace cnl::literals;
        using narrower = cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>;
        using wider = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>;
        auto const expected{wider{0x1'00000000'00000000_wide}};
        auto const lhs{narrower{0}};
        auto const rhs{wider{0x1'00000000'00000000_wide}};
        auto const actual(lhs + rhs);
        ASSERT_EQ(expected, actual);
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x1234 + 0x7625},
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x1234}
                                + cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x7625}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234 + 0x7625},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                + cnl::_impl::duplex_integer<int, unsigned>{0x7625}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{123 + 654},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{123}
                                + cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{654}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                786192872LL * 928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                786192872LL * 928178263LL}
                                + cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{0}));
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{
                                39487298182LL + 8238723LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{39487298182LL}
                                + cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint64_t, std::uint64_t>>{8238723LL}));
#endif
    }

    namespace test_subtract {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x1234 - 0x7625},
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x1234}
                                - cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x7625}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x7625 - 0x1234},
                        cnl::_impl::duplex_integer<int, unsigned>{0x7625}
                                - cnl::_impl::duplex_integer<int, unsigned>{0x1234}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234LL - 0x7625LL},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                - cnl::_impl::duplex_integer<int, unsigned>{0x7625}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>, std::uint32_t>{
                                2315645440ULL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>, std::uint32_t>{
                                5000000000ULL}
                                - cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        std::uint32_t>{2684354560ULL}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                {0, 0}, {0x40000000, 0}},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                {0, 1}, {0, 0}}
                                - cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint32_t,
                                                std::uint32_t>>{{0, 0}, {0xC0000000, 0}}));
    }

    TEST(duplex_integer, multiply1)  // NOLINT
    {
        using namespace cnl::literals;
        using t = cnl::_impl::narrowest_integer_t<97, unsigned>;
        auto expected = t{0x1'00000000'00000000'00000000_wide};
        auto lhs = t{0x1'00000000LL};
        auto rhs = t{0x1'00000000'00000000_wide};
        auto actual = lhs * rhs;
        ASSERT_EQ(expected, actual);
    }

    TEST(duplex_integer, multiply2)  // NOLINT
    {
        using namespace cnl::literals;
        using t = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<int, unsigned int>,
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                cnl::_impl::duplex_integer<unsigned int, unsigned int>>;
        auto const expected{t{1} << 96};
        auto const lhs{t{1} << 64};
        auto const rhs{t{1} << 32};
        auto const actual(lhs * rhs);
        ASSERT_EQ(expected, actual);
    }

    TEST(duplex_integer, multiply3)  // NOLINT
    {
        using namespace cnl::literals;
        using t = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<int, unsigned int>,
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                cnl::_impl::duplex_integer<unsigned int, unsigned int>>;
        auto const lhs{t{0x10000000'00000000'00000000'00000000_wide}};
        auto const rhs{t{0x10'00000000LL}};
        auto const actual(lhs * rhs);
        auto const expected{t{0x1'00000000'00000000'00000000'00000000'00000000_wide}};
        ASSERT_EQ(expected, actual);
    }

    TEST(duplex_integer, multiply4)  // NOLINT
    {
        using namespace cnl::literals;
        using t = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<int, unsigned int>,
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                cnl::_impl::duplex_integer<unsigned int, unsigned int>>;
        auto const expected{t{0x8'0000000A'00000007'00000004'00000001_wide}};
        auto const lhs{t{0x4'00000003'00000002'00000001_wide}};
        auto const rhs{t{0x2'00000001LL}};
        auto const actual(lhs * rhs);
        ASSERT_EQ(expected, actual);
    }

    TEST(duplex_integer, multiply5)  // NOLINT
    {
        using namespace cnl::literals;
        using t = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<int, unsigned int>,
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                cnl::_impl::duplex_integer<unsigned int, unsigned int>>;
        auto const expected{t{0x14FD00EEBA0BB3DB0D0F411252A0FB7F58_wide}};
        auto const lhs{t{0x12725DD1D243ABA0E75FE645C_wide}};
        auto const rhs{t{0x123456789ALL}};
        auto const actual(lhs * rhs);
        ASSERT_EQ(expected, actual);
    }

    TEST(duplex_integer, multiply6)  // NOLINT
    {
        using namespace cnl::literals;
        using t = cnl::_impl::duplex_integer<
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<int, unsigned int>,
                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                cnl::_impl::duplex_integer<unsigned int, unsigned int>>;
        auto const expected{t{1532495540865888858358347027150309183000000000000000000_wide}};
        auto const lhs{t{1532495540865888858358347027150309183_wide}};
        auto const rhs{t{1000000000000000000LL}};
        auto const actual(lhs * rhs);
        ASSERT_EQ(expected, actual);
    }

    namespace test_multiply {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x1234 * 0x7625},
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x1234}
                                * cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x7625}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234 * 0x7625},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                * cnl::_impl::duplex_integer<int, unsigned>{0x7625}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                786192872LL * 928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{786192872}
                                * cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint32_t, std::uint32_t>>{928178263}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                786192872LL * 928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{786192872LL}
                                * cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint32_t, std::uint32_t>>{928178263LL}));
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{
                                786192872LL * 928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{786192872}
                                * cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint64_t, std::uint64_t>>{928178263}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{
                                786192872 * 928178263LL},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{786192872}
                                * cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint64_t, std::uint64_t>>{928178263LL}));
#endif
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                786192872LL * 928178263LL},
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{786192872LL}
                                * cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint32_t, std::uint32_t>>{928178263LL}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                786192872LL * 928178263LL},
                        786192872LL
                                * cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<
                                                std::uint32_t, std::uint32_t>>{928178263LL}));
    }

    namespace test_long_multiply {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{7 * 13},
                        cnl::_impl::long_multiply<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{}(
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{7},
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{13})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>>{
                                7 * 13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>>{}(
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{7},
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{13})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{7 * 13},
                        cnl::_impl::long_multiply<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{}(
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{7},
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{13})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{7 * 13},
                        cnl::_impl::long_multiply<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{}(
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{7},
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>{13})));
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{7 * 13},
                        cnl::_impl::long_multiply<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{}(
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{7},
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{13})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>,
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>>{
                                7 * 13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>>{}(
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{7},
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                        cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{13})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{7 * 13},
                        cnl::_impl::long_multiply<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{}(
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{7},
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{13})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>,
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{7 * 13},
                        cnl::_impl::long_multiply<
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>{}(
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{7},
                                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{13})));
#endif
    }

    namespace test_divide {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x12},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                / cnl::_impl::duplex_integer<int, unsigned>{0x100}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{
                                1000000000ULL},
                        cnl::custom_operator<
                                cnl::_impl::divide_op,
                                cnl::op_value<cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>>,
                                cnl::op_value<cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>>>{}(
                                5000000000ULL, 5)));
        static_assert(
                identical(0x12, 0x1234 / cnl::_impl::duplex_integer<int, unsigned>{0x100}));

        TEST(duplex_integer, divide)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned>{0x12};
            auto actual = cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                        / cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, int_divide_by_duplex)  // NOLINT
        {
            auto expected = 0x12;
            auto actual = 0x1234 / cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, divide128)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                    cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>;
            constexpr auto exponent = 64;

            auto expected = type({{0, 0}, {0x55555555, 0x55555555}});

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator / denominator;

            ASSERT_EQ((expected), (actual));
        }

        TEST(duplex_integer, divide160)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                            cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>,
                    std::uint32_t>;
            constexpr auto exponent = 96;

            auto expected = type({{0, 0}, {0x55555555, 0x55555555}}, 0x55555555);

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator / denominator;

            ASSERT_EQ((expected), (actual));
        }

#if defined(CNL_INT128_ENABLED)
        TEST(duplex_integer, divide255)  // NOLINT
        {
            using duplex_integer = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                    cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>;

            auto expected = duplex_integer{
                    {INT64_C(0x031C71C71C71C71C), UINT64_C(0x71C71C71C71C71C7)},
                    {UINT64_C(0x1C71C71C71C71C71), UINT64_C(0xC71C71C71C71C71C)}};

            auto nume = duplex_integer(7) << 250;
            auto denom = duplex_integer(9);
            auto actual = nume / denom;

            ASSERT_EQ(expected, actual);
        }
#endif
    }

    namespace test_modulo {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x34},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                % cnl::_impl::duplex_integer<int, unsigned>{0x100}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{0},
                        cnl::custom_operator<
                                cnl::_impl::modulo_op,
                                cnl::op_value<cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>>,
                                cnl::op_value<cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>>>{}(
                                5000000000ULL, 5)));
        static_assert(
                identical(0x12, 0x1234 % cnl::_impl::duplex_integer<int, unsigned>{0x100}));

        TEST(duplex_integer, modulo)  // NOLINT
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned>{0x34};
            auto actual = cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                        % cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, int_modulo_by_duplex)  // NOLINT
        {
            auto expected = 0x12;
            auto actual = 0x1234 % cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }

        TEST(duplex_integer, modulo128)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                    cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>;
            constexpr auto exponent = 64;

            auto expected = type({{0, 0}, {0, 1}});

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator % denominator;

            ASSERT_EQ((expected), (actual));
        }

        TEST(duplex_integer, modulo160)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>,
                            cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>,
                    std::uint32_t>;
            constexpr auto exponent = 96;

            auto expected = type({{0, 0}, {0, 0}}, 1);

            auto const numerator = type{1} << exponent;
            auto const denominator = type{3};
            auto actual = numerator % denominator;

            ASSERT_EQ((expected), (actual));
        }

#if defined(CNL_INT128_ENABLED)
        TEST(duplex_integer, modulo255)  // NOLINT
        {
            using type = cnl::_impl::duplex_integer<
                    cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>,
                    cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>>;
            auto expected = type{{0, 0}, {0, 7}};

            auto const numerator = type{{0, 0}, {76, UINT64_C(5242979323791729901)}};
            auto const denominator = type{10};
            auto actual = numerator % denominator;

            ASSERT_EQ((expected), (actual));
        }
#endif
    }

    namespace test_bitwise_and {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<unsigned, unsigned>{0xa005},
                        cnl::_impl::duplex_integer<unsigned, unsigned>{0xaa55}
                                & cnl::_impl::duplex_integer<unsigned, unsigned>{0xa5a5}));
    }

    namespace test_equal {
        static_assert(
                cnl::_impl::duplex_integer<int, unsigned>{0}
                == cnl::_impl::duplex_integer<int, unsigned>{});
        static_assert(
                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123}
                == cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123});
        static_assert(
                !(cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{123}
                  == cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{124}));
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123}
                  == cnl::_impl::duplex_integer<short, unsigned short>{-124}));
    }

    namespace test_not_equal {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0}
                  != cnl::_impl::duplex_integer<int, unsigned>{}));
        static_assert(
                !(cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123}
                  != cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123}));
        static_assert(
                cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{512}
                != cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{1024});
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123}
                != cnl::_impl::duplex_integer<short, unsigned short>{-124});

        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123} != std::int8_t{-124});
        static_assert(
                std::int64_t{-12356} < cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{12356});
    }

    namespace test_less {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0}
                  < cnl::_impl::duplex_integer<int, unsigned>{}));
        static_assert(
                !(cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123}
                  < cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123}));
        static_assert(
                cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{123}
                < cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{124});
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123}
                  < cnl::_impl::duplex_integer<short, unsigned short>{-124}));
    }

    namespace test_greater_than_or_equal {
        static_assert(
                cnl::_impl::duplex_integer<int, unsigned>{0}
                >= cnl::_impl::duplex_integer<int, unsigned>{});
        static_assert(
                cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123}
                >= cnl::_impl::duplex_integer<std::uint64_t, std::uint64_t>{123});
        static_assert(
                !(cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{123}
                  >= cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{124}));
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123}
                >= cnl::_impl::duplex_integer<short, unsigned short>{-124});
        static_assert(
                cnl::_impl::duplex_integer<
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>,
                        cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>>{123}
                >= cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{-124});
    }

    namespace test_bitwise_and_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{0x1234 & 0x5858},
                        cnl::custom_operator<
                                cnl::_impl::bitwise_and_op,
                                cnl::op_value<cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>>,
                                cnl::op_value<cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>>>{}(
                                0x1234, 0x5858)));
    }

    namespace test_shift_left_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{0},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<signed, unsigned>>,
                                cnl::op_value<cnl::_impl::duplex_integer<signed, unsigned>>>{}(0, 0)));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{0x2468ACE0},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>>,
                                cnl::op_value<cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>>>{}(
                                0x12345670, 1)));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{1},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<signed, unsigned>>,
                                cnl::op_value<cnl::_impl::duplex_integer<signed, unsigned>>>{}(1, 0)));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{246},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<signed, unsigned>>,
                                cnl::op_value<cnl::_impl::duplex_integer<signed, unsigned>>>{}(
                                cnl::_impl::duplex_integer<signed, unsigned>{123},
                                cnl::_impl::duplex_integer<signed, unsigned>{1})));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                signed, cnl::_impl::duplex_integer<unsigned, unsigned>>{0},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<
                                        signed, cnl::_impl::duplex_integer<unsigned, unsigned>>>,
                                cnl::op_value<int>>{}(0, 1)));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{1LL << 34},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<int, unsigned int>>,
                                cnl::op_value<int>>{}(1, 34)));

        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                cnl::_impl::duplex_integer<unsigned int, unsigned int>>{1LL << 34},
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>>,
                                cnl::op_value<int>>{}(1, 34)));
    }

    namespace test_shift_right_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>{0},
                        cnl::custom_operator<
                                cnl::_impl::shift_right_op,
                                cnl::op_value<cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>>,
                                cnl::op_value<int>>{}(0, 9)));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>{0x7394ULL >> 0},
                        cnl::custom_operator<
                                cnl::_impl::shift_right_op,
                                cnl::op_value<cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>>,
                                cnl::op_value<cnl::_impl::duplex_integer<std::int8_t, std::uint8_t>>>{}(0x7394ULL, 0)));
    }

    namespace test_shift_left {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{246},
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{123} << 1));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{246},
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{123}
                                << cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{1}));
    }

    namespace test_shift_right {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>, unsigned int>{
                                0x1234},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>, unsigned int>{
                                0x1234567890ABCDEFLL}
                                >> 48));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>, unsigned int>{0},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<int, unsigned int>, unsigned int>{
                                0x1234567890ABCDEFLL}
                                >> 64));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{
                                {0x01234567890ABCDELL, 0xFFEDCBA987654321ULL}, 0x08765432U},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{
                                {0x1234567890ABCDEFLL, 0xFEDCBA9876543210ULL}, 0x87654321U}
                                >> 4));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{
                                {0x0000000012345678LL, 0x90ABCDEFFEDCBA98ULL}, 0x76543210U},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{
                                {0x1234567890ABCDEFLL, 0xFEDCBA9876543210ULL}, 0x87654321U}
                                >> 32));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{
                                {0x0000000000000000LL, 0x1234567890ABCDEFLL}, 0xFEDCBA98U},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<
                                        cnl::_impl::duplex_integer<int, unsigned int>,
                                        cnl::_impl::duplex_integer<unsigned int, unsigned int>>,
                                unsigned int>{
                                {0x1234567890ABCDEFLL, 0xFEDCBA9876543210ULL}, 0x87654321U}
                                >> 64));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{123},
                        cnl::_impl::duplex_integer<signed, unsigned>{246} >> 1));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{123},
                        cnl::_impl::duplex_integer<signed, unsigned>{246}
                                >> cnl::_impl::duplex_integer<signed, unsigned>{1}));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{0x007FFFFF, 0xFFFFFFFF},
                        cnl::_impl::duplex_integer<int, unsigned int>{0x7FFFFFFF, 0xFFFFFFFF} >> 8));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{-1},
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{-1, 0x40000000} >> 32));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>, std::uint32_t>{
                                {-1, 0xFFFFFFFF}, 0x40000000},
                        cnl::_impl::duplex_integer<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>, std::uint32_t>{
                                {-1, 0x40000000}, 0}
                                >> 32));
    }

    namespace test_sensible_right_shift {
        static_assert(
                identical(
                        std::int32_t{-1},
                        cnl::_impl::sensible_right_shift<std::int32_t>(INT32_C(-1), 32)));
        static_assert(
                identical(
                        static_cast<std::uint16_t>(-1 >> 16),
                        cnl::_impl::sensible_right_shift<std::uint16_t>(
                                cnl::_impl::duplex_integer<std::int16_t, std::uint16_t>{-1, 65535},
                                16)));
        static_assert(identical(short{-1}, cnl::_impl::sensible_right_shift<short>(-1, 0)));
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{-1},
                        cnl::_impl::sensible_right_shift<
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>>(
                                cnl::_impl::duplex_integer<std::int32_t, std::uint32_t>{-1, 0x40000000},
                                32)));
    }
}
