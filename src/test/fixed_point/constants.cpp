
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/assert.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

namespace {

#if defined(__cpp_inline_variables)

    using cnl::_impl::identical;

    ////////////////////////////////////////////////////////////////////////////////
    // simple one-off tests

    // 8-bit pi
    static_assert(
            identical(cnl::fixed_point<cnl::uint8, -6>{3.140625}, cnl::math::pi_v<cnl::fixed_point<cnl::uint8, -6>>),
            "cnl::math::pi_v test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // precision tests

    // where greater of inputs is max and lesser is min, returns 1-max/min
    template<class Rep, int Exponent>
    long double get_error(cnl::fixed_point<Rep, Exponent> fp_constant, long double ld_constant)
    {
        CNL_ASSERT(fp_constant > 0.);
        CNL_ASSERT(ld_constant > 0.);

        auto ratio = static_cast<long double>(fp_constant) / ld_constant;
        auto error = ((ratio>1)?ratio:(1.L/ratio))-1.L;
        return error;
    }

    TEST(fixed_point_constants, e) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::e_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::e_v<long double>), .006L);
        EXPECT_LT(get_error(cnl::math::e_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::e_v<long double>), .015L);
        EXPECT_LT(get_error(cnl::math::e_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::e_v<long double>), .000002L);
#if !defined(__arm__)
        EXPECT_LT(get_error(cnl::math::e_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::e_v<long double>),
                .000000001L);
#endif
    }

    TEST(fixed_point_constants, log2e) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::log2e_v<long double>),
                .006L);
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::log2e_v<long double>),
                .015L);
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::log2e_v<long double>),
                .000005L);
        EXPECT_LT(get_error(cnl::math::log2e_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::log2e_v<long double>),
                .0000000000000000002L);
    }

    TEST(fixed_point_constants, log10e) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::log10e_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::log10e_v<long double>),
                .16L);
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::log10e_v<long double>),
                .0000325L);
        EXPECT_LT(get_error(cnl::math::log10e_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::log10e_v<long double>),
                .0000000000000000004L);
    }

    TEST(fixed_point_constants, pi) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::pi_v<long double>), .006L);
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::pi_v<long double>), .015L);
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::pi_v<long double>),
                .0000021L);
#if !defined(__arm__)
        EXPECT_LT(get_error(cnl::math::pi_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::pi_v<long double>),
                .000000001L);
#endif
    }

    TEST(fixed_point_constants, invpi) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::inv_pi_v<long double>),
                .02L);
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::inv_pi_v<long double>),
                .02L);
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::inv_pi_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::inv_pi_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::inv_pi_v<long double>),
                .0000000000000000003L);
    }

    TEST(fixed_point_constants, inv_sqrtpi)  // NOLINT
    {
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::fixed_point<cnl::uint8, -6>>,
                cnl::math::inv_sqrtpi_v<long double>), .02L);
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::fixed_point<cnl::int16, -4>>,
                cnl::math::inv_sqrtpi_v<long double>), .02L);
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::fixed_point<cnl::int32, -16>>,
                cnl::math::inv_sqrtpi_v<long double>), .00005L);
        EXPECT_LT(get_error(cnl::math::inv_sqrtpi_v<cnl::fixed_point<cnl::uint64, -62>>,
                cnl::math::inv_sqrtpi_v<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, ln2) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::ln2_v<long double>), .01L);
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::ln2_v<long double>), .015L);
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::ln2_v<long double>),
                .0000021L);
        EXPECT_LT(get_error(cnl::math::ln2_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::ln2_v<long double>),
                .0000000000000000002L);
    }

    TEST(fixed_point_constants, ln10) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::ln10_v<long double>),
                .006L);
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::ln10_v<long double>),
                .025L);
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::ln10_v<long double>),
                .0000021L);
        EXPECT_LT(get_error(cnl::math::ln10_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::ln10_v<long double>),
                .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt2) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::sqrt2_v<long double>),
                .006L);
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::sqrt2_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::sqrt2_v<long double>),
                .00001L);
        EXPECT_LT(get_error(cnl::math::sqrt2_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::sqrt2_v<long double>),
                .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt3) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::sqrt3_v<long double>),
                .02L);
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::sqrt3_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::sqrt3_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::sqrt3_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::sqrt3_v<long double>),
                .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrt3) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::fixed_point<cnl::uint8, -6>>,
                cnl::math::inv_sqrt3_v<long double>), .03L);
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::fixed_point<cnl::int16, -4>>,
                cnl::math::inv_sqrt3_v<long double>), .03L);
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::fixed_point<cnl::int32, -16>>,
                cnl::math::inv_sqrt3_v<long double>), .00005L);
        EXPECT_LT(get_error(cnl::math::inv_sqrt3_v<cnl::fixed_point<cnl::uint64, -62>>,
                cnl::math::inv_sqrt3_v<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, egamma) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::egamma_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::egamma_v<long double>),
                .03L);
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::egamma_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::egamma_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::egamma_v<long double>),
                .0000000000000000003L);
    }

    TEST(fixed_point_constants, phi) {  // NOLINT
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::fixed_point<cnl::uint8, -6>>, cnl::math::phi_v<long double>), .02L);
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::fixed_point<cnl::int16, -4>>, cnl::math::phi_v<long double>), .04L);
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::fixed_point<cnl::int32, -16>>, cnl::math::phi_v<long double>),
                .00005L);
        EXPECT_LT(get_error(cnl::math::phi_v<cnl::fixed_point<cnl::uint64, -62>>, cnl::math::phi_v<long double>),
                .0000000000000000003L);
    }

#endif  // defined(__cpp_inline_variables)
}
