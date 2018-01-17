
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

namespace {

#if defined(__cpp_inline_variables)

    using cnl::_impl::identical;
    using cnl::fixed_point;
    using namespace cnl::math_constants;

    ////////////////////////////////////////////////////////////////////////////////
    // simple one-off tests

    // 8-bit pi
    static_assert(
            identical(fixed_point<cnl::uint8, -6>{3.140625}, pi<fixed_point<cnl::uint8, -6>>),
            "cnl::math_constants::pi test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // precision tests

    // where greater of inputs is max and lesser is min, returns 1-max/min
    template<class Rep, int Exponent>
    long double get_error(fixed_point<Rep, Exponent> fp_constant, long double ld_constant) {
        assert(fp_constant > 0.);
        assert(ld_constant > 0.);

        auto ratio = static_cast<long double>(fp_constant) / ld_constant;
        auto error = ((ratio>1)?ratio:(1.L/ratio))-1.L;
        return error;
    }

    TEST(fixed_point_constants, e) {
        EXPECT_LT(get_error(e<fixed_point<cnl::uint8, -6>>, e<long double>), .006L);
        EXPECT_LT(get_error(e<fixed_point<cnl::int16, -4>>, e<long double>), .015L);
        EXPECT_LT(get_error(e<fixed_point<cnl::int32, -16>>, e<long double>), .000002L);
        EXPECT_LT(get_error(e<fixed_point<cnl::uint64, -62>>, e<long double>), .0000000000000000000000000000002L);
    }

    TEST(fixed_point_constants, log2e) {
        EXPECT_LT(get_error(log2e<fixed_point<cnl::uint8, -6>>, log2e<long double>), .006L);
        EXPECT_LT(get_error(log2e<fixed_point<cnl::int16, -4>>, log2e<long double>), .015L);
        EXPECT_LT(get_error(log2e<fixed_point<cnl::int32, -16>>, log2e<long double>), .000005L);
        EXPECT_LT(get_error(log2e<fixed_point<cnl::uint64, -62>>, log2e<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, log10e) {
        EXPECT_LT(get_error(log10e<fixed_point<cnl::uint8, -6>>, log10e<long double>), .03L);
        EXPECT_LT(get_error(log10e<fixed_point<cnl::int16, -4>>, log10e<long double>), .16L);
        EXPECT_LT(get_error(log10e<fixed_point<cnl::int32, -16>>, log10e<long double>), .0000325L);
        EXPECT_LT(get_error(log10e<fixed_point<cnl::uint64, -62>>, log10e<long double>), .0000000000000000004L);
    }

    TEST(fixed_point_constants, pi) {
        EXPECT_LT(get_error(pi<fixed_point<cnl::uint8, -6>>, pi<long double>), .006L);
        EXPECT_LT(get_error(pi<fixed_point<cnl::int16, -4>>, pi<long double>), .015L);
        EXPECT_LT(get_error(pi<fixed_point<cnl::int32, -16>>, pi<long double>), .0000021L);
        EXPECT_LT(get_error(pi<fixed_point<cnl::uint64, -62>>, pi<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, invpi) {
        EXPECT_LT(get_error(invpi<fixed_point<cnl::uint8, -6>>, invpi<long double>), .02L);
        EXPECT_LT(get_error(invpi<fixed_point<cnl::int16, -4>>, invpi<long double>), .02L);
        EXPECT_LT(get_error(invpi<fixed_point<cnl::int32, -16>>, invpi<long double>), .00005L);
        EXPECT_LT(get_error(invpi<fixed_point<cnl::uint64, -62>>, invpi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrtpi) {
        EXPECT_LT(get_error(invsqrtpi<fixed_point<cnl::uint8, -6>>, invsqrtpi<long double>), .02L);
        EXPECT_LT(get_error(invsqrtpi<fixed_point<cnl::int16, -4>>, invsqrtpi<long double>), .02L);
        EXPECT_LT(get_error(invsqrtpi<fixed_point<cnl::int32, -16>>, invsqrtpi<long double>), .00005L);
        EXPECT_LT(get_error(invsqrtpi<fixed_point<cnl::uint64, -62>>, invsqrtpi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, ln2) {
        EXPECT_LT(get_error(ln2<fixed_point<cnl::uint8, -6>>, ln2<long double>), .01L);
        EXPECT_LT(get_error(ln2<fixed_point<cnl::int16, -4>>, ln2<long double>), .015L);
        EXPECT_LT(get_error(ln2<fixed_point<cnl::int32, -16>>, ln2<long double>), .0000021L);
        EXPECT_LT(get_error(ln2<fixed_point<cnl::uint64, -62>>, ln2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, ln10) {
        EXPECT_LT(get_error(ln10<fixed_point<cnl::uint8, -6>>, ln10<long double>), .006L);
        EXPECT_LT(get_error(ln10<fixed_point<cnl::int16, -4>>, ln10<long double>), .025L);
        EXPECT_LT(get_error(ln10<fixed_point<cnl::int32, -16>>, ln10<long double>), .0000021L);
        EXPECT_LT(get_error(ln10<fixed_point<cnl::uint64, -62>>, ln10<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt2) {
        EXPECT_LT(get_error(sqrt2<fixed_point<cnl::uint8, -6>>, sqrt2<long double>), .006L);
        EXPECT_LT(get_error(sqrt2<fixed_point<cnl::int16, -4>>, sqrt2<long double>), .03L);
        EXPECT_LT(get_error(sqrt2<fixed_point<cnl::int32, -16>>, sqrt2<long double>), .00001L);
        EXPECT_LT(get_error(sqrt2<fixed_point<cnl::uint64, -62>>, sqrt2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt3) {
        EXPECT_LT(get_error(sqrt3<fixed_point<cnl::uint8, -6>>, sqrt3<long double>), .02L);
        EXPECT_LT(get_error(sqrt3<fixed_point<cnl::int16, -4>>, sqrt3<long double>), .03L);
        EXPECT_LT(get_error(sqrt3<fixed_point<cnl::int32, -16>>, sqrt3<long double>), .00005L);
        EXPECT_LT(get_error(sqrt3<fixed_point<cnl::uint64, -62>>, sqrt3<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrt2) {
        EXPECT_LT(get_error(invsqrt2<fixed_point<cnl::uint8, -6>>, invsqrt2<long double>), .02L);
        EXPECT_LT(get_error(invsqrt2<fixed_point<cnl::int16, -4>>, invsqrt2<long double>), .03L);
        EXPECT_LT(get_error(invsqrt2<fixed_point<cnl::int32, -16>>, invsqrt2<long double>), .00005L);
        EXPECT_LT(get_error(invsqrt2<fixed_point<cnl::uint64, -62>>, invsqrt2<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, invsqrt3) {
        EXPECT_LT(get_error(invsqrt3<fixed_point<cnl::uint8, -6>>, invsqrt3<long double>), .03L);
        EXPECT_LT(get_error(invsqrt3<fixed_point<cnl::int16, -4>>, invsqrt3<long double>), .03L);
        EXPECT_LT(get_error(invsqrt3<fixed_point<cnl::int32, -16>>, invsqrt3<long double>), .00005L);
        EXPECT_LT(get_error(invsqrt3<fixed_point<cnl::uint64, -62>>, invsqrt3<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, radian) {
        EXPECT_LT(get_error(radian<fixed_point<cnl::uint8, -1>>, radian<long double>), .02L);
        EXPECT_LT(get_error(radian<fixed_point<cnl::int16, -4>>, radian<long double>), .02L);
        EXPECT_LT(get_error(radian<fixed_point<cnl::int32, -16>>, radian<long double>), .00005L);
        EXPECT_LT(get_error(radian<fixed_point<cnl::uint64, -58>>, radian<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, egamma) {
        EXPECT_LT(get_error(egamma<fixed_point<cnl::uint8, -6>>, egamma<long double>), .03L);
        EXPECT_LT(get_error(egamma<fixed_point<cnl::int16, -4>>, egamma<long double>), .03L);
        EXPECT_LT(get_error(egamma<fixed_point<cnl::int32, -16>>, egamma<long double>), .00005L);
        EXPECT_LT(get_error(egamma<fixed_point<cnl::uint64, -62>>, egamma<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, phi) {
        EXPECT_LT(get_error(phi<fixed_point<cnl::uint8, -6>>, phi<long double>), .02L);
        EXPECT_LT(get_error(phi<fixed_point<cnl::int16, -4>>, phi<long double>), .04L);
        EXPECT_LT(get_error(phi<fixed_point<cnl::int32, -16>>, phi<long double>), .00005L);
        EXPECT_LT(get_error(phi<fixed_point<cnl::uint64, -62>>, phi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, catalan) {
        EXPECT_LT(get_error(catalan<fixed_point<cnl::uint8, -6>>, catalan<long double>), .02L);
        EXPECT_LT(get_error(catalan<fixed_point<cnl::int16, -4>>, catalan<long double>), .05L);
        EXPECT_LT(get_error(catalan<fixed_point<cnl::int32, -16>>, catalan<long double>), .00005L);
        EXPECT_LT(get_error(catalan<fixed_point<cnl::uint64, -62>>, catalan<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, apery) {
        EXPECT_LT(get_error(apery<fixed_point<cnl::uint8, -6>>, apery<long double>), .02L);
        EXPECT_LT(get_error(apery<fixed_point<cnl::int16, -4>>, apery<long double>), .02L);
        EXPECT_LT(get_error(apery<fixed_point<cnl::int32, -16>>, apery<long double>), .00005L);
        EXPECT_LT(get_error(apery<fixed_point<cnl::uint64, -62>>, apery<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, glaisher) {
        EXPECT_LT(get_error(glaisher<fixed_point<cnl::uint8, -6>>, glaisher<long double>), .02L);
        EXPECT_LT(get_error(glaisher<fixed_point<cnl::int16, -4>>, glaisher<long double>), .03L);
        EXPECT_LT(get_error(glaisher<fixed_point<cnl::int32, -16>>, glaisher<long double>), .00005L);
        EXPECT_LT(get_error(glaisher<fixed_point<cnl::uint64, -62>>, glaisher<long double>), .0000000000000000003L);
    }

#endif  // defined(__cpp_inline_variables)
}
