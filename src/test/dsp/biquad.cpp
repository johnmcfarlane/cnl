//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <gtest/gtest.h>
#include "dsp/dsp_types.h"
#include "dsp/biquad.h"

static constexpr double biquad_double_accuracy = 0.0000001;
static constexpr double biquad_fix_accuracy = 0.00001;

TEST(biquad, impulse_default)
{
    // Default impulse response is unit impulse response
    cnl::dsp::biquad<double> biquad;

    EXPECT_NEAR(biquad.filter(1.0), 1.0, biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.0, biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(1.0), 1.0, biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.0, biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.0, biquad_double_accuracy);
}

TEST(biquad, impulse)
{
    // Matlab:
    // [zlo,plo,klo] = butter(2,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);
    std::array<double, 5> coeff { { 0.274726851035635, 0.549453702071270,
                                    0.274726851035635, -0.073623846384979,
                                    0.172531250527518 } };

    cnl::dsp::biquad<double> biquad(coeff);

    EXPECT_NEAR(biquad.filter(1.0), 0.274726851035635,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.569680149549746,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.269269927691992,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), -0.078462940810032,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), -0.052234220855265,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.009691625045734,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), 0.009725570158077,
                biquad_double_accuracy);
    EXPECT_NEAR(biquad.filter(0.0), -0.000956074305460,
                biquad_double_accuracy);
}

TEST(biquad, impulse_q4_20)
{
    // Matlab:
    // [zlo,plo,klo] = butter(2,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);
    std::array<q4_20, 5> coeff { { 0.274726851035635,  0.549453702071270,
                                   0.274726851035635, -0.073623846384979,
                                   0.172531250527518 } };

    cnl::dsp::biquad<q4_20> biquad(coeff);

    q4_20 one = 1.0;
    q4_20 zero = 0.0;

    EXPECT_NEAR(static_cast<double>(biquad.filter(one)), 0.274726851035635,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), 0.569680149549746,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), 0.269269927691992,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), -0.078462940810032,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), -0.052234220855265,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), 0.009691625045734,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), 0.009725570158077,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquad.filter(zero)), -0.000956074305460,
                biquad_fix_accuracy);
}
