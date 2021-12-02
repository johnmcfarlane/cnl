
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/fraction.h` definitions

#include <gtest/gtest.h>

#include <cnl/fraction.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cstdint.h>

namespace {
    using cnl::_impl::assert_same;
    using cnl::_impl::identical;

    namespace test_copy_ctor {
        static_assert(
                identical(
                        cnl::fraction<short>(123), cnl::fraction<short>(cnl::fraction<short>(123))),
                "cnl::fraction argument deduction");
    }

    namespace test_fraction_conversion_ctor {
        static_assert(
                identical(
                        cnl::fraction<std::int64_t>(123),
                        cnl::fraction<std::int64_t>(cnl::fraction<std::int8_t>(123))),
                "cnl::fraction widening conversion from fraction");
        static_assert(
                identical(
                        cnl::fraction<std::int64_t>(123),
                        cnl::fraction<std::int64_t>(cnl::fraction<std::int8_t>(123))),
                "cnl::fraction narrowing conversion from fraction");
    }

    template<typename NumDenom, typename FloatingPoint>
    void test_floating_ctor(FloatingPoint initializer, NumDenom numerator, NumDenom denominator)
    {
        auto actual = cnl::fraction<NumDenom>(initializer);
        static_assert(assert_same<NumDenom, decltype(actual.numerator)>::value);
        static_assert(assert_same<NumDenom, decltype(actual.denominator)>::value);
        ASSERT_EQ(numerator, actual.numerator);
        ASSERT_EQ(denominator, actual.denominator);
    }

    TEST(fraction, float_ctor_int64)  // NOLINT
    {
        test_floating_ctor<std::int64_t>(.1, 1, 10);
        test_floating_ctor<std::int64_t>(0.999, 999, 1000);
        test_floating_ctor<std::int64_t>(1., 1, 1);
        test_floating_ctor<std::int64_t>(1.001, 1001, 1000);
        test_floating_ctor<std::int64_t>(1.001001001001001001001001001001001001001, 1000, 999);
        test_floating_ctor<std::int64_t>(10., 10, 1);
        test_floating_ctor<std::int64_t>(10.25, 41, 4);
        test_floating_ctor<std::int64_t>(1e-9, 1, 1000000000);
        test_floating_ctor<std::int64_t>(1e-15, 1, 1000000000000000);
        test_floating_ctor<std::int64_t>(1e9, 1000000000, 1);
        test_floating_ctor<std::int64_t>(1e15, 1000000000000000, 1);
        test_floating_ctor<std::int64_t>(1.0 / 3.0, 1, 3);
        test_floating_ctor<std::int64_t>(1.0 / 237.0, 1, 237);
        test_floating_ctor<std::int64_t>(237.001, 237001, 1000);
        test_floating_ctor<std::int64_t>(237.0000000001, 2370294537913, 10001242776);
        test_floating_ctor<std::int64_t>(237.0000000000000000000000000000000000001L, 237, 1);
        test_floating_ctor<std::int64_t>(236.9999999999999999999999999999999999999L, 237, 1);
        test_floating_ctor<std::int64_t>(double{23 / 7}, 3, 1);  // NOLINT(bugprone-integer-division)
    }

#if defined(CNL_DEBUG)
    TEST(fraction_ctor, float_ctor_int64_double_inv22)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int64_t>(1e-22), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int64_double_inv30)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int64_t>(1e-30), "");  // NOLINT
    }
#endif

    TEST(fraction, float_ctor_int32)  // NOLINT
    {
        test_floating_ctor<std::int32_t>(.1, 1, 10);
        test_floating_ctor<std::int32_t>(0.999, 999, 1000);
        test_floating_ctor<std::int32_t>(1., 1, 1);
        test_floating_ctor<std::int32_t>(1.001, 1001, 1000);
        test_floating_ctor<std::int32_t>(1.001001001001001001001001001001001001001, 1000, 999);
        test_floating_ctor<std::int32_t>(10., 10, 1);
        test_floating_ctor<std::int32_t>(10.25, 41, 4);
        test_floating_ctor<std::int32_t>(1e-9, 1, 1000000000);
        test_floating_ctor<std::int32_t>(1e9, 1000000000, 1);
        test_floating_ctor<std::int32_t>(1.0 / 3.0, 1, 3);
        test_floating_ctor<std::int32_t>(1.0 / 237.0, 1, 237);
        test_floating_ctor<std::int32_t>(237.001, 237001, 1000);
        test_floating_ctor<std::int32_t>(237.000001, 237000001, 1000000);
        test_floating_ctor<std::int32_t>(double{23 / 7}, 3, 1);  // NOLINT(bugprone-integer-division)
    }

#if defined(CNL_DEBUG)
    TEST(fraction_ctor, float_ctor_int32_double_inv15)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int32_t>(1e-15), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int32_double_inv22)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int32_t>(1e-22), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int32_double_inv30)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int32_t>(1e-30), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int32_double_15)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int32_t>(1e15), "");  // NOLINT
    }
#endif

    TEST(fraction, float_ctor_int16)  // NOLINT
    {
        test_floating_ctor<std::int16_t>(.1, 1, 10);
        test_floating_ctor<std::int16_t>(0.999, 999, 1000);
        test_floating_ctor<std::int16_t>(1., 1, 1);
        test_floating_ctor<std::int16_t>(1.001, 1001, 1000);
        test_floating_ctor<std::int16_t>(1.001001001001001001001001001001001001001, 1000, 999);
        test_floating_ctor<std::int16_t>(10., 10, 1);
        test_floating_ctor<std::int16_t>(10.25, 41, 4);
        test_floating_ctor<std::int16_t>(1.0 / 3.0, 1, 3);
        test_floating_ctor<std::int16_t>(1.0 / 237.0, 1, 237);
        test_floating_ctor<std::int16_t>(237.01, 23701, 100);
        test_floating_ctor<std::int16_t>(237.001, 32470, 137);
        test_floating_ctor<std::int16_t>(double{23 / 7}, 3, 1);  // NOLINT(bugprone-integer-division)
    }

#if defined(CNL_DEBUG)
    TEST(fraction_ctor, float_ctor_int16_double_inv9)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int16_t>(1e-9), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int16_double_inv15)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int16_t>(1e-15), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int16_double_inv22)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int16_t>(1e-22), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int16_double_inv30)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int16_t>(1e-30), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int16_double_9)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int16_t>(1e9), "");  // NOLINT
    }

    TEST(fraction_ctor, float_ctor_int16_double_15)  // NOLINT
    {
        ASSERT_DEATH(cnl::fraction<std::int16_t>(1e15), "");  // NOLINT
    }
#endif
}
