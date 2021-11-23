
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_integer.h>

#include <cnl/_impl/config.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    namespace test_minus {
        static_assert(
                identical(
                        cnl::wide_integer<300, int>{-1729348762983LL},
                        -cnl::wide_integer<300, int>{1729348762983LL}));
    }

    namespace test_division_native {
        TEST(static_integer, division_native)  // NOLINT
        {
            auto ex = cnl::static_integer<2, cnl::native_rounding_tag>{3 / 4};
            auto n = cnl::static_integer<2, cnl::native_rounding_tag>{3};
            auto d = cnl::static_integer<3, cnl::native_rounding_tag>{4};
            auto q = n / d;
            ASSERT_EQ(ex, q);
        }
        static_assert(
                identical(
                        cnl::static_integer<2, cnl::native_rounding_tag>{3 / 4},
                        cnl::static_integer<2, cnl::native_rounding_tag>{3}
                                / cnl::static_integer<3, cnl::native_rounding_tag>{4}));
        static_assert(
                identical(
                        cnl::static_integer<31, cnl::native_rounding_tag>{0},
                        -9 / cnl::static_integer<4, cnl::native_rounding_tag>{10}));
        static_assert(
                identical(
                        cnl::static_integer<2, cnl::native_rounding_tag>{0},
                        cnl::static_integer<2, cnl::native_rounding_tag>{-2} / 3));
        static_assert(
                identical(
                        cnl::static_integer<2, cnl::native_rounding_tag>{0},
                        cnl::static_integer<2, cnl::native_rounding_tag>{1} / -3));
    }

    namespace test_division_nearest {
        static_assert(
                identical(
                        cnl::static_integer<2, cnl::nearest_rounding_tag>{1},
                        cnl::static_integer<2, cnl::nearest_rounding_tag>{3}
                                / cnl::static_integer<3, cnl::nearest_rounding_tag>{4}));
        static_assert(
                identical(
                        cnl::static_integer<31, cnl::nearest_rounding_tag>{-1},
                        -9 / cnl::static_integer<4, cnl::nearest_rounding_tag>{10}));
        static_assert(
                identical(
                        cnl::static_integer<2, cnl::nearest_rounding_tag>{-1},
                        cnl::static_integer<2, cnl::nearest_rounding_tag>{-2} / 3));
        static_assert(
                identical(
                        cnl::static_integer<2, cnl::nearest_rounding_tag>{0},
                        cnl::static_integer<2, cnl::nearest_rounding_tag>{1} / -3));
    }

    namespace test_multiply {
        static_assert(
                identical(
                        cnl::static_integer<6>{7} * cnl::static_integer<13>{321},
                        cnl::static_integer<19>{2247}));
    }

#if defined(CNL_INT128_ENABLED)
    namespace test_divide {
        using namespace cnl::literals;

// can only be compiled with constant evaluation limits raised
#if defined(CNL_IMPL_ONEROUS_EVALUATION)
        static_assert(
                identical(
                        cnl::static_integer<225>(
                                3333333333333333333333333333333333333333333333333333333333333333333_wide),
                        cnl::make_static_integer(
                                10000000000000000000000000000000000000000000000000000000000000000000_wide)
                                / 3));
#endif

#if defined(__clang__)
        static_assert(
                identical(
                        cnl::static_integer<258>(
                                33333333333333333333333333333333333333333333333333333333333333333333333333333_wide),
                        cnl::make_static_integer(
                                100000000000000000000000000000000000000000000000000000000000000000000000000000_wide)
                                / 3));
#endif
    }
#endif

    namespace test_shift_right_native {
        static_assert(
                identical(
                        cnl::static_integer<1, cnl::native_rounding_tag>{1},
                        cnl::static_integer<3, cnl::native_rounding_tag>{7} >> cnl::constant<2>{}));
        static_assert(
                identical(
                        cnl::static_integer<3, cnl::native_rounding_tag>{1},
                        cnl::static_integer<3, cnl::native_rounding_tag>{7} >> 2));
        static_assert(
                identical(
                        cnl::static_integer<1, cnl::native_rounding_tag>{1},
                        cnl::static_integer<4, cnl::native_rounding_tag>{12} >> cnl::constant<3>{}));
    }

    namespace test_shift_right_nearest {
        static_assert(
                identical(
                        cnl::static_integer<5, cnl::nearest_rounding_tag>{0x15},
                        cnl::static_integer<7, cnl::nearest_rounding_tag>{0x55}
                                >> cnl::constant<2>{}));
        static_assert(
                identical(
                        cnl::static_integer<4, cnl::nearest_rounding_tag>{3},
                        cnl::static_integer<4, cnl::nearest_rounding_tag>{15} >> 2));
        static_assert(
                identical(
                        cnl::static_integer<3, cnl::nearest_rounding_tag>{4},
                        cnl::static_integer<5, cnl::nearest_rounding_tag>{16}
                                >> cnl::constant<2>{}));
    }

#if defined(CNL_DEBUG)
    TEST(static_integer, most_negative_number)  // NOLINT
    {
        static_assert(cnl::static_integer<1>{1}, "in-range boundary test");
        static_assert(cnl::static_integer<1>{-1}, "in-range boundary test");
        ASSERT_DEATH(cnl::static_integer<1>{-2}, "negative overflow");  // NOLINT
    }
#endif

    TEST(static_integer, pre_increment)  // NOLINT
    {
        auto a = cnl::static_integer<3>{6};
        auto& b = ++a;
        static_assert(
                std::is_same<decltype(b), cnl::static_integer<3>&>::value,
                "static_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(7, b) << "static_integer pre-increment";
    }

    TEST(static_integer, pre_decrement)  // NOLINT
    {
        auto a = cnl::static_integer<3>{-6};
        auto& b = --a;
        static_assert(
                std::is_same<decltype(b), cnl::static_integer<3>&>::value,
                "static_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(-7, b) << "static_integer pre-increment";
    }

    TEST(static_integer, post_increment)  // NOLINT
    {
        auto a = cnl::static_integer<3>{6};
        auto const& b = a++;
        static_assert(
                std::is_same<decltype(b), cnl::static_integer<3> const&>::value,
                "static_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(7, a) << "static_integer pre-increment";
        ASSERT_EQ(6, b) << "static_integer pre-increment";
    }

    TEST(static_integer, post_decrement)  // NOLINT
    {
        auto a = cnl::static_integer<3>{-6};
        auto const& b = a--;
        static_assert(
                std::is_same<decltype(b), cnl::static_integer<3> const&>::value,
                "static_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(-7, a) << "static_integer pre-increment";
        ASSERT_EQ(-6, b) << "static_integer pre-increment";
    }

#if defined(CNL_DEBUG)
    TEST(static_integer, pre_increment_overflow)  // NOLINT
    {
        auto a = cnl::static_integer<3>{7};
        ASSERT_DEATH(++a, "positive overflow");  // NOLINT
    }

    TEST(static_integer, pre_decrement_overflow)  // NOLINT
    {
        auto a = cnl::static_integer<3>{-7};
        ASSERT_DEATH(--a, "negative overflow");  // NOLINT
    }

    TEST(static_integer, post_increment_overflow)  // NOLINT
    {
        auto a = cnl::static_integer<3>{7};
        ASSERT_DEATH(a++, "positive overflow");  // NOLINT
    }

    TEST(static_integer, post_decrement_overflow)  // NOLINT
    {
        auto a = cnl::static_integer<3>{-7};
        ASSERT_DEATH(a--, "negative overflow");  // NOLINT
    }
#endif
}
