
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

    ////////////////////////////////////////////////////////////////////////////////
    // simple one-off tests

    // 8-bit pi
    static_assert(identical(fixed_point<std::uint8_t, -6>{3.140625}, cnl::pi<fixed_point<std::uint8_t, -6>>), "");
    static_assert(identical(fixed_point<std::int8_t, -6>{1.5625}, cnl::pi_2<fixed_point<std::int8_t, -6>>), "");

    ////////////////////////////////////////////////////////////////////////////////
    // precision tests

    // where greater of inputs is max and lesser is min, returns 1-max/min
    template<class Rep, int Exponent>
    long double get_error(fixed_point<Rep, Exponent> fp_constant, long double ld_constant) {
        assert(fp_constant > 0);
        assert(ld_constant > 0);

        auto ratio = static_cast<long double>(fp_constant) / ld_constant;
        auto error = ((ratio>1)?ratio:(1.L/ratio))-1.L;
        return error;
    }

    TEST(fixed_point_constants, e) {
        EXPECT_LT(get_error(cnl::e<fixed_point<std::uint8_t, -6>>, cnl::e<long double>), .006L);
        EXPECT_LT(get_error(cnl::e<fixed_point<std::int16_t, -4>>, cnl::e<long double>), .015L);
        EXPECT_LT(get_error(cnl::e<fixed_point<std::int32_t, -16>>, cnl::e<long double>), .000002L);
        EXPECT_LT(get_error(cnl::e<fixed_point<std::uint64_t, -62>>, cnl::e<long double>), .0000000000000000000000000000002L);
    }

    TEST(fixed_point_constants, log2e) {
        EXPECT_LT(get_error(cnl::log2e<fixed_point<std::uint8_t, -6>>, cnl::log2e<long double>), .006L);
        EXPECT_LT(get_error(cnl::log2e<fixed_point<std::int16_t, -4>>, cnl::log2e<long double>), .015L);
        EXPECT_LT(get_error(cnl::log2e<fixed_point<std::int32_t, -16>>, cnl::log2e<long double>), .000005L);
        EXPECT_LT(get_error(cnl::log2e<fixed_point<std::uint64_t, -62>>, cnl::log2e<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, log10e) {
        EXPECT_LT(get_error(cnl::log10e<fixed_point<std::uint8_t, -6>>, cnl::log10e<long double>), .03L);
        EXPECT_LT(get_error(cnl::log10e<fixed_point<std::int16_t, -4>>, cnl::log10e<long double>), .16L);
        EXPECT_LT(get_error(cnl::log10e<fixed_point<std::int32_t, -16>>, cnl::log10e<long double>), .0000325L);
        EXPECT_LT(get_error(cnl::log10e<fixed_point<std::uint64_t, -62>>, cnl::log10e<long double>), .0000000000000000004L);
    }

    TEST(fixed_point_constants, ln2) {
        EXPECT_LT(get_error(cnl::ln2<fixed_point<std::uint8_t, -6>>, cnl::ln2<long double>), .01L);
        EXPECT_LT(get_error(cnl::ln2<fixed_point<std::int16_t, -4>>, cnl::ln2<long double>), .015L);
        EXPECT_LT(get_error(cnl::ln2<fixed_point<std::int32_t, -16>>, cnl::ln2<long double>), .0000021L);
        EXPECT_LT(get_error(cnl::ln2<fixed_point<std::uint64_t, -62>>, cnl::ln2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, ln10) {
        EXPECT_LT(get_error(cnl::ln10<fixed_point<std::uint8_t, -6>>, cnl::ln10<long double>), .006L);
        EXPECT_LT(get_error(cnl::ln10<fixed_point<std::int16_t, -4>>, cnl::ln10<long double>), .025L);
        EXPECT_LT(get_error(cnl::ln10<fixed_point<std::int32_t, -16>>, cnl::ln10<long double>), .0000021L);
        EXPECT_LT(get_error(cnl::ln10<fixed_point<std::uint64_t, -62>>, cnl::ln10<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, pi) {
        EXPECT_LT(get_error(cnl::pi<fixed_point<std::uint8_t, -6>>, cnl::pi<long double>), .006L);
        EXPECT_LT(get_error(cnl::pi<fixed_point<std::int16_t, -4>>, cnl::pi<long double>), .015L);
        EXPECT_LT(get_error(cnl::pi<fixed_point<std::int32_t, -16>>, cnl::pi<long double>), .0000021L);
        EXPECT_LT(get_error(cnl::pi<fixed_point<std::uint64_t, -62>>, cnl::pi<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, pi_2) {
        EXPECT_LT(get_error(cnl::pi_2<fixed_point<std::uint8_t, -6>>, cnl::pi_2<long double>), .006L);
        EXPECT_LT(get_error(cnl::pi_2<fixed_point<std::int16_t, -4>>, cnl::pi_2<long double>), .015L);
        EXPECT_LT(get_error(cnl::pi_2<fixed_point<std::int32_t, -16>>, cnl::pi_2<long double>), .00005L);
        EXPECT_LT(get_error(cnl::pi_2<fixed_point<std::uint64_t, -62>>, cnl::pi_2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, pi_4) {
        EXPECT_LT(get_error(cnl::pi_4<fixed_point<std::uint8_t, -6>>, cnl::pi_4<long double>), .006L);
        EXPECT_LT(get_error(cnl::pi_4<fixed_point<std::int16_t, -4>>, cnl::pi_4<long double>), .05L);
        EXPECT_LT(get_error(cnl::pi_4<fixed_point<std::int32_t, -16>>, cnl::pi_4<long double>), .00002L);
        EXPECT_LT(get_error(cnl::pi_4<fixed_point<std::uint64_t, -62>>, cnl::pi_4<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, one_pi) {
        EXPECT_LT(get_error(cnl::one_pi<fixed_point<std::uint8_t, -6>>, cnl::one_pi<long double>), .02L);
        EXPECT_LT(get_error(cnl::one_pi<fixed_point<std::int16_t, -4>>, cnl::one_pi<long double>), .02L);
        EXPECT_LT(get_error(cnl::one_pi<fixed_point<std::int32_t, -16>>, cnl::one_pi<long double>), .00005L);
        EXPECT_LT(get_error(cnl::one_pi<fixed_point<std::uint64_t, -62>>, cnl::one_pi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, two_pi) {
        EXPECT_LT(get_error(cnl::two_pi<fixed_point<std::uint8_t, -6>>, cnl::two_pi<long double>), .02L);
        EXPECT_LT(get_error(cnl::two_pi<fixed_point<std::int16_t, -4>>, cnl::two_pi<long double>), .02L);
        EXPECT_LT(get_error(cnl::two_pi<fixed_point<std::int32_t, -16>>, cnl::two_pi<long double>), .00002L);
        EXPECT_LT(get_error(cnl::two_pi<fixed_point<std::uint64_t, -62>>, cnl::two_pi<long double>), .0000000000000000003L);
    }

    TEST(fixed_point_constants, two_sqrtpi) {
        EXPECT_LT(get_error(cnl::two_sqrtpi<fixed_point<std::uint8_t, -6>>, cnl::two_sqrtpi<long double>), .006L);
        EXPECT_LT(get_error(cnl::two_sqrtpi<fixed_point<std::int16_t, -4>>, cnl::two_sqrtpi<long double>), .015L);
        EXPECT_LT(get_error(cnl::two_sqrtpi<fixed_point<std::int32_t, -16>>, cnl::two_sqrtpi<long double>), .000007L);
        EXPECT_LT(get_error(cnl::two_sqrtpi<fixed_point<std::uint64_t, -62>>, cnl::two_sqrtpi<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, sqrt2) {
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<std::uint8_t, -6>>, cnl::sqrt2<long double>), .006L);
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<std::int16_t, -4>>, cnl::sqrt2<long double>), .03L);
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<std::int32_t, -16>>, cnl::sqrt2<long double>), .00001L);
        EXPECT_LT(get_error(cnl::sqrt2<fixed_point<std::uint64_t, -62>>, cnl::sqrt2<long double>), .0000000000000000002L);
    }

    TEST(fixed_point_constants, one_sqrt2) {
        EXPECT_LT(get_error(cnl::one_sqrt2<fixed_point<std::uint8_t, -6>>, cnl::one_sqrt2<long double>), .006L);
        EXPECT_LT(get_error(cnl::one_sqrt2<fixed_point<std::int16_t, -4>>, cnl::one_sqrt2<long double>), .03L);
        EXPECT_LT(get_error(cnl::one_sqrt2<fixed_point<std::int32_t, -16>>, cnl::one_sqrt2<long double>), .00003L);
        EXPECT_LT(get_error(cnl::one_sqrt2<fixed_point<std::uint64_t, -62>>, cnl::one_sqrt2<long double>), .0000000000000000002L);
    }

#endif  // defined(__cpp_inline_variables)
}
