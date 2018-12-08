
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/_impl/assert.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {

#if defined(__cpp_inline_variables)

    using cnl::_impl::identical;
    using cnl::fixed_point;

    ////////////////////////////////////////////////////////////////////////////////
    // simple one-off tests

    // 8-bit pi
    static_assert(
            identical(fixed_point<cnl::uint8, -6>{3.140625}, cnl::pi<fixed_point<cnl::uint8, -6>>),
            "cnl::pi test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // precision tests

    // where greater of inputs is max and lesser is min, returns 1-max/min
    template<class Rep, int Exponent>
    long double get_error(fixed_point<Rep, Exponent> fp_constant, long double ld_constant) {
        CNL_ASSERT(fp_constant > 0.);
        CNL_ASSERT(ld_constant > 0.);

        auto ratio = static_cast<long double>(fp_constant) / ld_constant;
        auto error = ((ratio>1)?ratio:(1.L/ratio))-1.L;
        return error;
    }

    TEST(fixed_point_constants, e) {
        EXPECT_LT(get_error(cnl::e<fixed_point<cnl::uint8, -6>>, cnl::e<long double>), .006L);
        EXPECT_LT(get_error(cnl::e<fixed_point<cnl::int16, -4>>, cnl::e<long double>), .015L);
        EXPECT_LT(get_error(cnl::e<fixed_point<cnl::int32, -16>>, cnl::e<long double>), .000002L);
        EXPECT_LT(get_error(cnl::e<fixed_point<cnl::uint64, -62>>, cnl::e<long double>), .000000001L);
    }

    TEST(fixed_point_constants, log2e) {
        EXPECT_LT(get_error(cnl::log2e<fixed_point<cnl::uint8, -6>>, cnl::log2e<long double>), .006L);
        EXPECT_LT(get_error(cnl::log2e<fixed_point<cnl::int16, -4>>, cnl::log2e<long double>), .015L);
        EXPECT_LT(get_error(cnl::log2e<fixed_point<cnl::int32, -16>>, cnl::log2e<long double>), .000005L);
        EXPECT_LT(get_error(cnl::log2e<fixed_point<cnl::uint64, -62>>, cnl::log2e<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, log10e) {
        EXPECT_LT(get_error(cnl::log10e<fixed_point<cnl::uint8, -6>>, cnl::log10e<long double>), .03L);
        EXPECT_LT(get_error(cnl::log10e<fixed_point<cnl::int16, -4>>, cnl::log10e<long double>), .16L);
        EXPECT_LT(get_error(cnl::log10e<fixed_point<cnl::int32, -16>>, cnl::log10e<long double>), .0000325L);
        EXPECT_LT(get_error(cnl::log10e<fixed_point<cnl::uint64, -62>>, cnl::log10e<long double>), .0000000000000000004L);
    }

    TEST(fixed_point_constants, pi) {
        EXPECT_LT(get_error(cnl::pi<fixed_point<cnl::uint8, -6>>, cnl::pi<long double>), .006L);
        EXPECT_LT(get_error(cnl::pi<fixed_point<cnl::int16, -4>>, cnl::pi<long double>), .015L);
        EXPECT_LT(get_error(cnl::pi<fixed_point<cnl::int32, -16>>, cnl::pi<long double>), .0000021L);
        EXPECT_LT(get_error(cnl::pi<fixed_point<cnl::uint64, -62>>, cnl::pi<long double>), .000000001L);
    }

    TEST(fixed_point_constants, invpi) {
        EXPECT_LT(get_error(cnl::invpi<fixed_point<cnl::uint8, -6>>, cnl::invpi<long double>), .02L);
        EXPECT_LT(get_error(cnl::invpi<fixed_point<cnl::int16, -4>>, cnl::invpi<long double>), .02L);
        EXPECT_LT(get_error(cnl::invpi<fixed_point<cnl::int32, -16>>, cnl::invpi<long double>), .00005L);
        EXPECT_LT(get_error(cnl::invpi<fixed_point<cnl::uint64, -62>>, cnl::invpi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrtpi) {
        EXPECT_LT(get_error(cnl::invsqrtpi<fixed_point<cnl::uint8, -6>>, cnl::invsqrtpi<long double>), .02L);
        EXPECT_LT(get_error(cnl::invsqrtpi<fixed_point<cnl::int16, -4>>, cnl::invsqrtpi<long double>), .02L);
        EXPECT_LT(get_error(cnl::invsqrtpi<fixed_point<cnl::int32, -16>>, cnl::invsqrtpi<long double>), .00005L);
        EXPECT_LT(get_error(cnl::invsqrtpi<fixed_point<cnl::uint64, -62>>, cnl::invsqrtpi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, ln2) {
        EXPECT_LT(get_error(cnl::ln2<fixed_point<cnl::uint8, -6>>, cnl::ln2<long double>), .01L);
        EXPECT_LT(get_error(cnl::ln2<fixed_point<cnl::int16, -4>>, cnl::ln2<long double>), .015L);
        EXPECT_LT(get_error(cnl::ln2<fixed_point<cnl::int32, -16>>, cnl::ln2<long double>), .0000021L);
        EXPECT_LT(get_error(cnl::ln2<fixed_point<cnl::uint64, -62>>, cnl::ln2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, ln10) {
        EXPECT_LT(get_error(cnl::ln10<fixed_point<cnl::uint8, -6>>, cnl::ln10<long double>), .006L);
        EXPECT_LT(get_error(cnl::ln10<fixed_point<cnl::int16, -4>>, cnl::ln10<long double>), .025L);
        EXPECT_LT(get_error(cnl::ln10<fixed_point<cnl::int32, -16>>, cnl::ln10<long double>), .0000021L);
        EXPECT_LT(get_error(cnl::ln10<fixed_point<cnl::uint64, -62>>, cnl::ln10<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt2) {
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<cnl::uint8, -6>>, cnl::sqrt2<long double>), .006L);
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<cnl::int16, -4>>, cnl::sqrt2<long double>), .03L);
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<cnl::int32, -16>>, cnl::sqrt2<long double>), .00001L);
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<cnl::uint64, -62>>, cnl::sqrt2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt3) {
        EXPECT_LT(get_error(cnl::sqrt3<fixed_point<cnl::uint8, -6>>, cnl::sqrt3<long double>), .02L);
        EXPECT_LT(get_error(cnl::sqrt3<fixed_point<cnl::int16, -4>>, cnl::sqrt3<long double>), .03L);
        EXPECT_LT(get_error(cnl::sqrt3<fixed_point<cnl::int32, -16>>, cnl::sqrt3<long double>), .00005L);
        EXPECT_LT(get_error(cnl::sqrt3<fixed_point<cnl::uint64, -62>>, cnl::sqrt3<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrt2) {
        EXPECT_LT(get_error(cnl::invsqrt2<fixed_point<cnl::uint8, -6>>, cnl::invsqrt2<long double>), .02L);
        EXPECT_LT(get_error(cnl::invsqrt2<fixed_point<cnl::int16, -4>>, cnl::invsqrt2<long double>), .03L);
        EXPECT_LT(get_error(cnl::invsqrt2<fixed_point<cnl::int32, -16>>, cnl::invsqrt2<long double>), .00005L);
        EXPECT_LT(get_error(cnl::invsqrt2<fixed_point<cnl::uint64, -62>>, cnl::invsqrt2<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrt3) {
        EXPECT_LT(get_error(cnl::invsqrt3<fixed_point<cnl::uint8, -6>>, cnl::invsqrt3<long double>), .03L);
        EXPECT_LT(get_error(cnl::invsqrt3<fixed_point<cnl::int16, -4>>, cnl::invsqrt3<long double>), .03L);
        EXPECT_LT(get_error(cnl::invsqrt3<fixed_point<cnl::int32, -16>>, cnl::invsqrt3<long double>), .00005L);
        EXPECT_LT(get_error(cnl::invsqrt3<fixed_point<cnl::uint64, -62>>, cnl::invsqrt3<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, radian) {
        EXPECT_LT(get_error(cnl::radian<fixed_point<cnl::uint8, -1>>, cnl::radian<long double>), .02L);
        EXPECT_LT(get_error(cnl::radian<fixed_point<cnl::int16, -4>>, cnl::radian<long double>), .02L);
        EXPECT_LT(get_error(cnl::radian<fixed_point<cnl::int32, -16>>, cnl::radian<long double>), .00005L);
        EXPECT_LT(get_error(cnl::radian<fixed_point<cnl::uint64, -58>>, cnl::radian<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, egamma) {
        EXPECT_LT(get_error(cnl::egamma<fixed_point<cnl::uint8, -6>>, cnl::egamma<long double>), .03L);
        EXPECT_LT(get_error(cnl::egamma<fixed_point<cnl::int16, -4>>, cnl::egamma<long double>), .03L);
        EXPECT_LT(get_error(cnl::egamma<fixed_point<cnl::int32, -16>>, cnl::egamma<long double>), .00005L);
        EXPECT_LT(get_error(cnl::egamma<fixed_point<cnl::uint64, -62>>, cnl::egamma<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, phi) {
        EXPECT_LT(get_error(cnl::phi<fixed_point<cnl::uint8, -6>>, cnl::phi<long double>), .02L);
        EXPECT_LT(get_error(cnl::phi<fixed_point<cnl::int16, -4>>, cnl::phi<long double>), .04L);
        EXPECT_LT(get_error(cnl::phi<fixed_point<cnl::int32, -16>>, cnl::phi<long double>), .00005L);
        EXPECT_LT(get_error(cnl::phi<fixed_point<cnl::uint64, -62>>, cnl::phi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, catalan) {
        EXPECT_LT(get_error(cnl::catalan<fixed_point<cnl::uint8, -6>>, cnl::catalan<long double>), .02L);
        EXPECT_LT(get_error(cnl::catalan<fixed_point<cnl::int16, -4>>, cnl::catalan<long double>), .05L);
        EXPECT_LT(get_error(cnl::catalan<fixed_point<cnl::int32, -16>>, cnl::catalan<long double>), .00005L);
        EXPECT_LT(get_error(cnl::catalan<fixed_point<cnl::uint64, -62>>, cnl::catalan<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, apery) {
        EXPECT_LT(get_error(cnl::apery<fixed_point<cnl::uint8, -6>>, cnl::apery<long double>), .02L);
        EXPECT_LT(get_error(cnl::apery<fixed_point<cnl::int16, -4>>, cnl::apery<long double>), .02L);
        EXPECT_LT(get_error(cnl::apery<fixed_point<cnl::int32, -16>>, cnl::apery<long double>), .00005L);
        EXPECT_LT(get_error(cnl::apery<fixed_point<cnl::uint64, -62>>, cnl::apery<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, glaisher) {
        EXPECT_LT(get_error(cnl::glaisher<fixed_point<cnl::uint8, -6>>, cnl::glaisher<long double>), .02L);
        EXPECT_LT(get_error(cnl::glaisher<fixed_point<cnl::int16, -4>>, cnl::glaisher<long double>), .03L);
        EXPECT_LT(get_error(cnl::glaisher<fixed_point<cnl::int32, -16>>, cnl::glaisher<long double>), .00005L);
        EXPECT_LT(get_error(cnl::glaisher<fixed_point<cnl::uint64, -62>>, cnl::glaisher<long double>), .0000000000000000003L);
    }

#endif  // defined(__cpp_inline_variables)
}
