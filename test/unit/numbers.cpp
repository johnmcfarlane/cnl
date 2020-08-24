
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/numbers.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/constant.h>

#include <gtest/gtest.h>

#include <cmath>

namespace {
#if defined(__cpp_inline_variables)

    namespace test_math_constants {
        template<typename T>
        void test_type()
        {
            auto epsilon = std::numeric_limits<T>::epsilon();

// Google Test's lack of support for long double results in unneccessary floating-point conversions.
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_EXPECT_NEAR(expected, actual, epsilon) EXPECT_NEAR(double(expected), double(actual), double(epsilon))

            CNL_EXPECT_NEAR(std::exp(T{1}), cnl::numbers::e_v<T>, epsilon);
            EXPECT_EQ(std::log2(cnl::numbers::e_v<T>), cnl::numbers::log2e_v<T>);
            CNL_EXPECT_NEAR(std::log10(cnl::numbers::e_v<T>), cnl::numbers::log10e_v<T>, epsilon);
            CNL_EXPECT_NEAR(std::atan(T{1})*T{4}, cnl::numbers::pi_v<T>, epsilon);
            CNL_EXPECT_NEAR(T{1}/cnl::numbers::pi_v<T>, cnl::numbers::inv_pi_v<T>, epsilon);
            CNL_EXPECT_NEAR(T{1}/std::sqrt(cnl::numbers::pi_v<T>), cnl::numbers::inv_sqrtpi_v<T>, epsilon);
            CNL_EXPECT_NEAR(std::log(T{2}), cnl::numbers::ln2_v<T>, epsilon);
            CNL_EXPECT_NEAR(std::log(T{10}), cnl::numbers::ln10_v<T>, epsilon);
            CNL_EXPECT_NEAR(std::sqrt(T{2}), cnl::numbers::sqrt2_v<T>, epsilon);
            CNL_EXPECT_NEAR(std::sqrt(T{3}), cnl::numbers::sqrt3_v<T>, epsilon);
            CNL_EXPECT_NEAR(T{1}/cnl::numbers::sqrt3_v<T>, cnl::numbers::inv_sqrt3_v<T>, epsilon);
            CNL_EXPECT_NEAR(0.5772156649015328606065120900824024L, cnl::numbers::egamma_v<T>, epsilon);
            CNL_EXPECT_NEAR(1.6180339887498948482045868343656381L, cnl::numbers::phi_v<T>, epsilon);
        }

        TEST(numeric, float_constants)  // NOLINT
        {
            test_type<float>();
        }

        TEST(numeric, double_constants)  // NOLINT
        {
            test_type<double>();
        }

        TEST(numeric, long_double_constants)  // NOLINT
        {
            test_type<long double>();
        }
    }

#endif  // defined(__cpp_inline_variables)
}
