//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <complex>
#include <vector>

#include <gtest/gtest.h>

#include "dsp/dsp_types.h"
#include "dsp/dsp_math.h"

#include "dsp/complex_vector.h"
#include "dsp/stft.h"

#define STFT_DOUBLE_ACCURACY 0.00000000001
#define STFT_Q4_20_ACCURACY 0.0006

TEST(stft, boxcar_double)
{
    using complex_vector = cnl::dsp::complex_vector<double>;
    using complex = cnl::dsp::complex<double>;

    unsigned int const fftSize = 256;

    double const twoPi {
        cnl::dsp::math::c_2_pi
    };

    std::vector<double> vec(fftSize / 2, 0.0);
    std::vector<std::vector<double> > input;
    input.push_back(vec);

    std::vector<complex_vector> output;
    output.resize(1);
    output[0].resize(1 + fftSize / 2);

    std::vector<int> exponents(1);
    std::vector<double> window(fftSize, 1.0);

    cnl::dsp::stft<double> myKernel(fftSize, 1);
    myKernel.set_window(window);

    for (unsigned int impulseLocation = 0;
         impulseLocation < (fftSize / 2);
         impulseLocation++)
    {
        input[0][impulseLocation] = 1.0;
        myKernel.transform(input, output, exponents);
        auto scale = pow(2.0, exponents[0]);
        // Impulse manifests in location impulseLocation+fftSize/2 due to 50% overlap
        // and delayed buffer
        for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
        {
            unsigned int index =
                (i * (impulseLocation + fftSize / 2)) % fftSize;
            double refAngle = twoPi * (double)index / (double)fftSize;
            auto ref = complex(std::cos(refAngle), -std::sin(refAngle));
            EXPECT_NEAR(output[0][i].real() * scale, ref.real(), STFT_DOUBLE_ACCURACY);
            EXPECT_NEAR(output[0][i].imag() * scale, ref.imag(), STFT_DOUBLE_ACCURACY);
        }
        input[0][impulseLocation] = 0.0;
        myKernel.transform(input, output, exponents);
        scale = pow(2.0, exponents[0]);
        // Feed in all zero vector
        // Impulse manifests in location impulseLocation due to delayed buffer
        for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
        {
            unsigned int index = (i * (impulseLocation)) % fftSize;
            double refAngle = twoPi * (double)index / (double)fftSize;
            auto ref = complex(std::cos(refAngle), -std::sin(refAngle));
            EXPECT_NEAR(output[0][i].real() * scale, ref.real(), STFT_DOUBLE_ACCURACY);
            EXPECT_NEAR(output[0][i].imag() * scale, ref.imag(), STFT_DOUBLE_ACCURACY);
        }
    }
}


TEST(stft, hann_double)
{
    using complex_vector = cnl::dsp::complex_vector<double>;
    using complex = cnl::dsp::complex<double>;

    unsigned int const fftSize = 256;

    double const twoPi {
        cnl::dsp::math::c_2_pi
    };

    double const pi {
        cnl::dsp::math::c_pi
    };

    std::vector<double> vec(fftSize / 2, 0.0);
    std::vector<std::vector<double> > input;
    input.push_back(vec);

    std::vector<complex_vector> output;
    output.resize(1);
    output[0].resize(1 + fftSize / 2);

    std::vector<int> exponents(1);

    cnl::dsp::stft<double> myKernel(fftSize, 1);

    for (unsigned int impulseLocation = 0;
         impulseLocation < (fftSize / 2);
         impulseLocation++)
    {
        input[0][impulseLocation] = 1.0;
        myKernel.transform(input, output, exponents);
        auto scale = pow(2.0, exponents[0]);

        // Impulse manifests in location impulseLocation+fftSize/2 due to 50% overlap
        // and delayed buffer
        for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
        {
            unsigned int index =
                (i * (impulseLocation + fftSize / 2)) % fftSize;
            double refAngle = twoPi * (double)index / (double)fftSize;
            double magnitude = std::sin(pi * (double)(impulseLocation + fftSize / 2) / (double)fftSize);
            auto ref = complex(std::cos(refAngle) * magnitude, -std::sin(refAngle) * magnitude);
            EXPECT_NEAR(output[0][i].real() * scale, ref.real(), STFT_DOUBLE_ACCURACY);
            EXPECT_NEAR(output[0][i].imag() * scale, ref.imag(), STFT_DOUBLE_ACCURACY);
        }
        input[0][impulseLocation] = 0.0;
        myKernel.transform(input, output, exponents);
        // Feed in all zero vector
        // Impulse manifests in location impulseLocation due to delayed buffer
        for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
        {
            unsigned int index = (i * (impulseLocation)) % fftSize;
            double refAngle = twoPi * (double)index / (double)fftSize;
            double magnitude = std::sin(pi * (double)impulseLocation / (double)fftSize);
            auto ref = complex(std::cos(refAngle) * magnitude, -std::sin(refAngle) * magnitude);
            EXPECT_NEAR(output[0][i].real() * scale, ref.real(), STFT_DOUBLE_ACCURACY);
            EXPECT_NEAR(output[0][i].imag() * scale, ref.imag(), STFT_DOUBLE_ACCURACY);
        }
    }
}

TEST(stft, random_hann_16_double_q4_20)
{
    unsigned int const fftSize = 16;

    // If these test vectors would be changed in future, remember to
    // --write-data of these test cases for DSP test inputs
    std::vector<double> vec1 = {
        0.537667139546100,
        1.833885014595086,
        -2.258846861003648,
        0.862173320368121,
        0.318765239858981,
        -1.307688296305273,
        -0.433592022305684,
        0.342624466538650
    };

    std::vector<double> vec2 = {
        3.578396939725760,
        2.769437029884877,
        -1.349886940156521,
        3.034923466331855,
        0.725404224946106,
        -0.063054873189656,
        0.714742903826096,
        -0.204966058299775
    };

    std::vector<std::complex<double> > ref1 {
        std::complex<double>(0.366085707659550, 0.000000000000000),
        std::complex<double>(-1.331671389227205, -0.662606585626785),
        std::complex<double>(1.638184573684681, -0.324223293065444),
        std::complex<double>(-1.225221207720604, -0.091215133070358),
        std::complex<double>(3.015899067557640, -0.288421102839888),
        std::complex<double>(-2.566780411759879, 3.545570955792287),
        std::complex<double>(-1.013652420014165, -4.166171090298601),
        std::complex<double>(2.973004450523289, 2.072575252392494),
        std::complex<double>(-3.345611033747062, 0.000000000000000),
        std::complex<double>(2.973004450523289, -2.072575252392494),
        std::complex<double>(-1.013652420014165, 4.166171090298601),
        std::complex<double>(-2.566780411759879, -3.545570955792287),
        std::complex<double>(3.015899067557640, +0.288421102839888),
        std::complex<double>(-1.225221207720604, 0.091215133070358),
        std::complex<double>(1.638184573684681, 0.324223293065444),
        std::complex<double>(-1.331671389227205, 0.662606585626785)
    };
    std::vector<std::complex<double> > ref2 {
        std::complex<double>(7.328273946863045, 0.000000000000000),
        std::complex<double>(-5.737132487240891, 4.113222966764005),
        std::complex<double>(3.893604627653774, -2.896471250728718),
        std::complex<double>(-4.455466396382844, 0.564967329428880),
        std::complex<double>(6.555358961341885, 1.346836808969967),
        std::complex<double>(-4.195892004213137, 0.727943517536565),
        std::complex<double>(1.786510633254537, -6.865450566723212),
        std::complex<double>(0.074903128933831, 3.126050419472004),
        std::complex<double>(-3.172046873557353, 0.000000000000000),
        std::complex<double>(0.074903128933831, -3.126050419472004),
        std::complex<double>(1.786510633254537, 6.865450566723212),
        std::complex<double>(-4.195892004213137, -0.727943517536565),
        std::complex<double>(6.555358961341885, -1.346836808969967),
        std::complex<double>(-4.455466396382844, -0.564967329428880),
        std::complex<double>(3.893604627653774, 2.896471250728718),
        std::complex<double>(-5.737132487240891, -4.113222966764005)
    };

    std::vector<std::vector<double> > input1;
    std::vector<std::vector<double> > input2;
    input1.push_back(vec1);
    input2.push_back(vec2);

    std::vector<std::vector<q4_20> > input1_fix;
    std::vector<std::vector<q4_20> > input2_fix;
    input1_fix.resize(input1.size());
    input2_fix.resize(input1.size());
    input1_fix[0].resize(input1[0].size());
    input2_fix[0].resize(input1[0].size());

    std::transform(input1[0].begin(),
                   input1[0].end(),
                   input1_fix[0].begin(),
                   [] (double x)
                   {
                       return static_cast<q4_20>(x);
                   });
    std::transform(input2[0].begin(),
                   input2[0].end(),
                   input2_fix[0].begin(),
                   [] (double x)
                   {
                       return static_cast<q4_20>(x);
                   });

    std::vector<std::vector<std::complex<double> > > output_ref1;
    std::vector<std::vector<std::complex<double> > > output_ref2;
    output_ref1.push_back(ref1);
    output_ref2.push_back(ref2);

    std::vector<cnl::dsp::complex_vector<double> > output;
    std::vector<cnl::dsp::complex_vector<q4_20> > output_fix;
    output.resize(1);
    output[0].resize(1 + fftSize / 2);
    output_fix.resize(1);
    output_fix[0].resize(1 + fftSize / 2);

    std::vector<int> exponents(1);
    std::vector<int> exponents_fix(1);

    cnl::dsp::stft<double> myKernel(fftSize, 1);
    cnl::dsp::stft<q4_20>  myKernel_fix(fftSize, 1);

    // Shoot in the first half
    myKernel.transform(input1, output, exponents);
    myKernel_fix.transform(input1_fix, output_fix, exponents_fix);

    for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
    {
        EXPECT_NEAR(output[0][i].real(), output_ref1[0][i].real(), STFT_DOUBLE_ACCURACY);
        EXPECT_NEAR(output[0][i].imag(), output_ref1[0][i].imag(), STFT_DOUBLE_ACCURACY);
    }
    // Shoot in the first and the second half
    myKernel.transform(input2, output, exponents);
    myKernel_fix.transform(input2_fix, output_fix, exponents_fix);
    double scale_fix = pow(2.0, exponents_fix[0]);
    for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
    {
        EXPECT_NEAR(output[0][i].real(), output_ref2[0][i].real(), STFT_DOUBLE_ACCURACY);
        EXPECT_NEAR(output[0][i].imag(), output_ref2[0][i].imag(), STFT_DOUBLE_ACCURACY);
        EXPECT_NEAR(output[0][i].real(), scale_fix * static_cast<double>(output_fix[0][i].real()), STFT_Q4_20_ACCURACY);
        EXPECT_NEAR(output[0][i].imag(), scale_fix * static_cast<double>(output_fix[0][i].imag()), STFT_Q4_20_ACCURACY);
    }
}
