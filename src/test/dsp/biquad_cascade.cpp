//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include <dsp/dsp_types.h>
#include <dsp/biquad_cascade.h>

static const double biquad_double_accuracy = 0.0000001;
static const double biquad_fix_accuracy = 0.00001;

TEST(biquad_cascade, impulse_1sos_default)
{
    // Default impulse response is unit impulse
    cnl::dsp::biquad_cascade<double> biquadCascade;
    EXPECT_NEAR(biquadCascade.filter(1.0), 1.0, biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.0, biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(1.0), 1.0, biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.0, biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.0, biquad_double_accuracy);
}

TEST(biquad_cascade, impulse_1sos)
{
    // Matlab:
    // [zlo,plo,klo] = butter(2,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);
    std::array<double, 5> coeff { { 0.274726851035635, 0.549453702071270,
                                    0.274726851035635, -0.073623846384979,
                                    0.172531250527518 } };
    std::vector<std::array<double, 5> > coeffs;
    coeffs.push_back(coeff);

    cnl::dsp::biquad_cascade<double> biquadCascade(coeffs);

    EXPECT_NEAR(biquadCascade.filter(1.0), 0.274726851035635,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.569680149549746,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.269269927691992,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), -0.078462940810032,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), -0.052234220855265,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.009691625045734,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.009725570158077,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), -0.000956074305460,
                biquad_double_accuracy);
}

TEST(biquad_cascade, impulse_2sos)
{
    // Matlab:
    // [zlo,plo,klo] = butter(4,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);
    std::array<double, 5> coeff1 { { 0.082672620462993, 0.165345240925987,
                                     0.082672620462993, -0.065336810440030,
                                     0.040552155481494 } };
    std::array<double, 5> coeff2 { { 1.000000000000000, 2.000000000000000,
                                     1.000000000000000, -0.090873773698255,
                                     0.447253094566770 } };
    std::vector<std::array<double, 5> > coeffs;
    coeffs.push_back(coeff1);
    coeffs.push_back(coeff2);

    cnl::dsp::biquad_cascade<double> biquadCascade(coeffs);

    EXPECT_NEAR(biquadCascade.filter(1.0), 0.082672620462993,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.343604820186741,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.508891433529390,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.243252880539287,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), -0.120782472179578,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), -0.128457608737101,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.038343992553840,
                biquad_double_accuracy);
    EXPECT_NEAR(biquadCascade.filter(0.0), 0.061028226574351,
                biquad_double_accuracy);
}

TEST(biquad_cascade, impulse_2sos_4channel)
{
    unsigned int const nChannels = 4;
    // Matlab:
    // [zlo,plo,klo] = butter(4,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);

    std::array<double, 5> coeff1 { { 0.082672620462993, 0.165345240925987,
                                     0.082672620462993, -0.065336810440030,
                                     0.040552155481494 } };
    std::array<double, 5> coeff2 { { 1.000000000000000, 2.000000000000000,
                                     1.000000000000000, -0.090873773698255,
                                     0.447253094566770 } };
    std::vector<double> golden = { 0.082672620462993, 0.343604820186741,
                                   0.508891433529390, 0.243252880539287,
                                   -0.120782472179578, -0.128457608737101,
                                   0.038343992553840, 0.061028226574351 };

    std::vector<std::array<double, 5> > coeffs;
    coeffs.push_back(coeff1);
    coeffs.push_back(coeff2);
    std::vector<double> one(nChannels, 1.0);
    std::vector<double> zero(nChannels, 0.0);
    std::vector<double> res(nChannels, 0.0);

    cnl::dsp::biquad_cascade<double> biquadCascade(coeffs, nChannels);
    res = biquadCascade.filter_interleaved(one);
    for (unsigned int ch = 0; ch < nChannels; ++ch)
    {
        EXPECT_NEAR(res[ch], golden[0], biquad_double_accuracy);
    }
    for (unsigned int i = 1; i < static_cast<unsigned int>(golden.size())-1; ++i)
    {
        res = biquadCascade.filter_interleaved(zero);
        for (unsigned int ch = 0; ch < nChannels; ++ch)
        {
            EXPECT_NEAR(res[ch], golden[i], biquad_double_accuracy);
        }
    }
}

TEST(biquad_cascade, impulse_2sos_4channel_for_each)
{
    unsigned int const nChannels = 4;
    // Matlab:
    // [zlo,plo,klo] = butter(4,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);

    std::array<double, 5> coeff1 { { 0.082672620462993, 0.165345240925987,
                                     0.082672620462993, -0.065336810440030,
                                     0.040552155481494 } };
    std::array<double, 5> coeff2 { { 1.000000000000000, 2.000000000000000,
                                     1.000000000000000, -0.090873773698255,
                                     0.447253094566770 } };
    std::vector<double> golden = { 0.082672620462993, 0.343604820186741,
                                   0.508891433529390, 0.243252880539287,
                                   -0.120782472179578, -0.128457608737101,
                                   0.038343992553840, 0.061028226574351 };

    std::vector<std::array<double, 5> > coeffs;
    coeffs.push_back(coeff1);
    coeffs.push_back(coeff2);
    std::vector<double> one(nChannels, 1.0);
    std::vector<double> zero(nChannels, 0.0);
    std::vector<std::vector<double> > input;
    input.push_back(one);
    for (unsigned int i = 0; i < static_cast<unsigned int>(golden.size())-1; ++i)
    {
        input.push_back(zero);
    }

    cnl::dsp::biquad_cascade<double> biquadCascade(coeffs, nChannels);

    // filterin INPLACE for parallel nChannels
    std::for_each(input.begin(),
                  input.end(),
                  [&biquadCascade](std::vector<double>&in)
                  {
                      in = biquadCascade.filter_interleaved(in);
                  });

    for (unsigned int i = 0; i < static_cast<unsigned int>(golden.size()); ++i)
    {
        for (unsigned int ch = 0; ch < nChannels; ++ch)
        {
            EXPECT_NEAR(input[i][ch], golden[i], biquad_double_accuracy);
        }
    }
}

TEST(biquad_cascade, impulse_twosos_q4_20)
{
    // Matlab:
    // [zlo,plo,klo] = butter(4,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);
    std::array<q4_20, 5> coeff1 { { 0.082672620462993, 0.165345240925987,
                                    0.082672620462993, -0.065336810440030,
                                    0.040552155481494 } };
    std::array<q4_20, 5> coeff2 { { 1.000000000000000, 2.000000000000000,
                                    1.000000000000000, -0.090873773698255,
                                    0.447253094566770 } };
    std::vector<std::array<q4_20, 5> > coeffs;
    coeffs.push_back(coeff1);
    coeffs.push_back(coeff2);

    cnl::dsp::biquad_cascade<q4_20> biquadCascade(coeffs);
    q4_20 one = 1.0;
    q4_20 zero = 0.0;

    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(one)), 0.082672620462993,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), 0.343604820186741,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), 0.508891433529390,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), 0.243252880539287,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), -0.120782472179578,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), -0.128457608737101,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), 0.038343992553840,
                biquad_fix_accuracy);
    EXPECT_NEAR(static_cast<double>(biquadCascade.filter(zero)), 0.061028226574351,
                biquad_fix_accuracy);
}


TEST(biquad_cascade, impulse_twosos_long_double_vs_q4_20) {

    // Matlab:
    // [zlo,plo,klo] = butter(4,0.48);
    // sos = zp2sos(zlo,plo,klo)
    // x = zeros(8,1); x(1) = 1;
    // y = sosfilt(sos,x);
    std::array<double, 5> coeff1 { { 0.082672620462993, 0.165345240925987,
                                     0.082672620462993, -0.065336810440030,
                                     0.040552155481494 } };
    std::array<double, 5> coeff2 { { 1.000000000000000, 2.000000000000000,
                                     1.000000000000000, -0.090873773698255,
                                     0.447253094566770 } };

    std::vector<std::array<double, 5> > coeffs;
    coeffs.push_back(coeff1);
    coeffs.push_back(coeff2);

    std::array<q4_20, 5> coeff1_fix;
    std::array<q4_20, 5> coeff2_fix;

    std::transform(coeff1.begin(),
                   coeff1.end(),
                   coeff1_fix.begin(),
                   [] (double val)
                   {
                       return static_cast<q4_20>(val);
                   });
    std::transform(coeff2.begin(),
                   coeff2.end(),
                   coeff2_fix.begin(),
                   [] (double val)
                   {
                       return static_cast<q4_20>(val);
                   });

    std::vector<std::array<q4_20, 5> > coeffs_fix;
    coeffs_fix.push_back(coeff1_fix);
    coeffs_fix.push_back(coeff2_fix);

    cnl::dsp::biquad_cascade<double> biquadCascade(coeffs);
    cnl::dsp::biquad_cascade<q4_20>  biquadCascadeFix(coeffs_fix);

    // Impulse response
    for (int i = 0; i < 1; ++i)
    {
        double res = biquadCascade.filter(1.0);
        q4_20 res_fix = biquadCascadeFix.filter(static_cast<q4_20>(1.0));
        EXPECT_NEAR(res, static_cast<double>(res_fix), biquad_fix_accuracy);
    }
    // Impulse response
    for (int i = 0; i < 9999; ++i)
    {
        double res = biquadCascade.filter(0.0);
        q4_20 res_fix = biquadCascadeFix.filter(static_cast<q4_20>(0.0));
        EXPECT_NEAR(res, static_cast<double>(res_fix), biquad_fix_accuracy);
    }
    // Step response
    for (int i = 1; i < 10000; ++i)
    {
        double res = biquadCascade.filter(1.0);
        q4_20 res_fix = biquadCascadeFix.filter(static_cast<q4_20>(1.0));
        EXPECT_NEAR(res, static_cast<double>(res_fix), biquad_fix_accuracy);
    }
}

