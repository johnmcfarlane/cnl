
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/assert.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/scaled_integer.h>

#include <gtest/gtest.h>

namespace {

#if defined(__cpp_inline_variables)

    using cnl::_impl::identical;

    ////////////////////////////////////////////////////////////////////////////////
    // simple one-off tests

    // 8-bit pi
    static_assert(
            identical(
                    cnl::scaled_integer<cnl::uint8, cnl::power<-6>>{3.140625},
                    cnl::math::pi_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>),
            "cnl::math::pi_v test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // precision tests

    // where greater of inputs is max and lesser is min, returns 1-max/min
    template<class Rep, int Exponent>
    long double get_error(cnl::scaled_integer<Rep, cnl::power<Exponent>> fp_constant, long double ld_constant)
    {
        CNL_ASSERT(fp_constant > 0.);
        CNL_ASSERT(ld_constant > 0.);

        auto ratio = static_cast<long double>(fp_constant) / ld_constant;
        auto error = ((ratio>1)?ratio:(1.L/ratio))-1.L;
        return error;
    }

    TEST(scaled_integer_constants, e) {
        EXPECT_LT(get_error(cnl::math::e_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::e_v<long double>), .006L);
        EXPECT_LT(get_error(cnl::math::e_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::e_v<long double>), .015L);
        EXPECT_LT(get_error(cnl::math::e_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::e_v<long double>), .000002L);
#if !defined(__arm__)
        EXPECT_LT(get_error(cnl::math::e_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::e_v<long double>),
                .000000001L);
#endif
    }

    TEST(scaled_integer_constants, log2e) {
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::log2e_v<long double>),
                .006L);
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::log2e_v<long double>),
                .015L);
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::log2e_v<long double>),
                .000005L);
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::log2e_v<long double>),
                .0000000000000000002L);
    }

    TEST(scaled_integer_constants, log10e) {
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::log10e_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::log10e_v<long double>),
                .16L);
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::log10e_v<long double>),
                .0000325L);
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::log10e_v<long double>),
                .0000000000000000004L);
    }

    TEST(scaled_integer_constants, pi) {
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::pi_v<long double>), .006L);
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::pi_v<long double>), .015L);
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::pi_v<long double>),
                .0000021L);
#if !defined(__arm__)
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::pi_v<long double>),
                .000000001L);
#endif
    }

    TEST(scaled_integer_constants, invpi) {
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::inv_pi_v<long double>),
                .02L);
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::inv_pi_v<long double>),
                .02L);
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::inv_pi_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::inv_pi_v<long double>),
                .0000000000000000003L);
    }

    TEST(scaled_integer_constants, inv_sqrtpi)
    {
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>,
                cnl::math::inv_sqrtpi_v<long double>), .02L);
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>,
                cnl::math::inv_sqrtpi_v<long double>), .02L);
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>,
                cnl::math::inv_sqrtpi_v<long double>), .00005L);
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>,
                cnl::math::inv_sqrtpi_v<long double>), .0000000000000000003L);
    }

    TEST(scaled_integer_constants, ln2) {
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::ln2_v<long double>), .01L);
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::ln2_v<long double>), .015L);
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::ln2_v<long double>),
                .0000021L);
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::ln2_v<long double>),
                .0000000000000000002L);
    }

    TEST(scaled_integer_constants, ln10) {
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::ln10_v<long double>),
                .006L);
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::ln10_v<long double>),
                .025L);
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::ln10_v<long double>),
                .0000021L);
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::ln10_v<long double>),
                .0000000000000000002L);
    }

    TEST(scaled_integer_constants, sqrt2) {
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::sqrt2_v<long double>),
                .006L);
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::sqrt2_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::sqrt2_v<long double>),
                .00001L);
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::sqrt2_v<long double>),
                .0000000000000000002L);
    }

    TEST(scaled_integer_constants, sqrt3) {
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::sqrt3_v<long double>),
                .02L);
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::sqrt3_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::sqrt3_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::sqrt3_v<long double>),
                .0000000000000000003L);
    }

    TEST(scaled_integer_constants, invsqrt3) {
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>,
                cnl::math::inv_sqrt3_v<long double>), .03L);
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>,
                cnl::math::inv_sqrt3_v<long double>), .03L);
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>,
                cnl::math::inv_sqrt3_v<long double>), .00005L);
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>,
                cnl::math::inv_sqrt3_v<long double>), .0000000000000000003L);
    }

    TEST(scaled_integer_constants, egamma) {
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::egamma_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::egamma_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::egamma_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::egamma_v<long double>),
                .0000000000000000003L);
    }

    TEST(scaled_integer_constants, phi) {
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::scaled_integer<cnl::uint8, cnl::power<-6>>>, cnl::math::phi_v<long double>), .02L);
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::scaled_integer<cnl::int16, cnl::power<-4>>>, cnl::math::phi_v<long double>), .04L);
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::scaled_integer<cnl::int32, cnl::power<-16>>>, cnl::math::phi_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::scaled_integer<cnl::uint64, cnl::power<-62>>>, cnl::math::phi_v<long double>),
                .0000000000000000003L);
    }

#endif  // defined(__cpp_inline_variables)
}
