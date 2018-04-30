//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <complex>
#include <iostream>
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "dsp/complex.h"
#include "dsp/complex_vector.h"
#include "dsp/fft.h"
#include "dsp/dsp_types.h"
#include "dsp/dsp_math.h"

#define FFT_DOUBLE_ACCURACY 0.00000000001
#define FFT_Q4_20_ACCURACY 0.00001
#define FFT_Q8_40_ACCURACY 0.000000001

TEST(fft, forward_double)
{
    unsigned int fftSize = 256;
    double const twoPi {
        cnl::dsp::math::c_2_pi
    };
    using complex = cnl::dsp::complex<double>;
    using complex_vector = cnl::dsp::complex_vector<double>;
    complex_vector vec(fftSize);

    for (unsigned int impulseLoc = 0; impulseLoc < fftSize; ++impulseLoc)
    {
        vec.fill(complex(0.0, 0.0));
        vec.set_at(impulseLoc, complex(1.0, 0.0));
        cnl::dsp::fft::fft<double>(vec);
        for (unsigned int i = 0; i < fftSize; ++i)
        {
            // For given impulse location FFT result can be calculated easily
            unsigned int index = (i * (impulseLoc)) % fftSize;
            double ref_angle = twoPi * (double)index / (double)fftSize;
            auto ref = complex(std::cos(ref_angle), -std::sin(ref_angle));
            EXPECT_NEAR(vec[i].real(), ref.real(), FFT_DOUBLE_ACCURACY)
                            << " at index: " << i << std::endl;
            EXPECT_NEAR(vec[i].imag(), ref.imag(), FFT_DOUBLE_ACCURACY)
                            << " at index: " << i << std::endl;
        }
    }
}

 TEST(fft, reverse_double)
{
    unsigned int fftSize = 256;
    double const twoPi {
        cnl::dsp::math::c_2_pi
    };
    using complex = cnl::dsp::complex<double>;
    using complex_vector = cnl::dsp::complex_vector<double>;
    complex_vector vec(fftSize);

    for (unsigned int impulseLoc = 0; impulseLoc < fftSize; ++impulseLoc)
    {
        vec.fill(complex(0.0, 0.0));
        vec.set_at(impulseLoc, complex(1.0, 0.0));
        cnl::dsp::fft::ifft<double>(vec);
        for (unsigned int i = 0; i < fftSize; ++i)
        {
            // For given impulse location FFT result can be calculated easily
            unsigned int index = (i * (impulseLoc)) % fftSize;
            double ref_angle = twoPi * (double)index / (double)fftSize;
            auto ref = complex(std::cos(ref_angle), std::sin(ref_angle));
            EXPECT_NEAR(vec[i].real(), ref.real(), FFT_DOUBLE_ACCURACY)
                            << " at index: " << i << std::endl;
            EXPECT_NEAR(vec[i].imag(), ref.imag(), FFT_DOUBLE_ACCURACY)
                            << " at index: " << i << std::endl;
        }
    }
}

TEST(fft, forward_q4_20)
{
    unsigned int fftSize = 256;
    double const twoPi {
        cnl::dsp::math::c_2_pi
    };
    using complex = cnl::dsp::complex<q4_20>;
    using complex_vector = cnl::dsp::complex_vector<q4_20>;
    complex_vector vec(fftSize);

    for (unsigned int impulseLoc = 0; impulseLoc < fftSize; ++impulseLoc)
    {
        vec.fill(complex(0.0, 0.0));
        vec.set_at(impulseLoc, complex(1.0, 0.0));
        int exponent = cnl::dsp::fft::fft<q4_20>(vec);
        auto scale = pow(2.0, exponent);
        for (unsigned int i = 0; i < fftSize; ++i)
        {
            // For given impulse location FFT result can be calculated easily
            unsigned int index = (i * (impulseLoc)) % fftSize;
            double ref_angle = twoPi * (double)index / (double)fftSize;
            auto ref = cnl::dsp::complex<double>(std::cos(ref_angle), -std::sin(ref_angle));
            EXPECT_NEAR(static_cast<double>(vec[i].real()) * scale,
                        ref.real(), FFT_Q4_20_ACCURACY)
                            << " at index: " << i << std::endl;
            EXPECT_NEAR(static_cast<double>(vec[i].imag()) * scale,
                        ref.imag(), FFT_Q4_20_ACCURACY)
                            << " at index: " << i << std::endl;
        }
    }
}

TEST(fft, reverse_q4_20)
{
    unsigned int fftSize = 256;
    double const twoPi {
        cnl::dsp::math::c_2_pi
    };
    using complex = cnl::dsp::complex<q4_20>;
    using complex_vector = cnl::dsp::complex_vector<q4_20>;
    complex_vector vec(fftSize);

    for (unsigned int impulseLoc = 0; impulseLoc < fftSize; ++impulseLoc)
    {
        vec.fill(complex(0.0, 0.0));
        vec.set_at(impulseLoc, complex(1.0, 0.0));
        int exponent = cnl::dsp::fft::ifft<q4_20>(vec);
        auto scale = pow(2.0, exponent);
        for (unsigned int i = 0; i < fftSize; ++i)
        {
            // For given impulse location FFT result can be calculated easily
            unsigned int index = (i * (impulseLoc)) % fftSize;
            double ref_angle = twoPi * (double)index / (double)fftSize;
            auto ref = cnl::dsp::complex<double>(std::cos(ref_angle), std::sin(ref_angle));
            EXPECT_NEAR(static_cast<double>(vec[i].real()) * scale,
                        ref.real(), FFT_Q4_20_ACCURACY)
                            << " at index: " << i << std::endl;
            EXPECT_NEAR(static_cast<double>(vec[i].imag()) * scale,
                        ref.imag(), FFT_Q4_20_ACCURACY)
                            << " at index: " << i << std::endl;
        }
    }
}

TEST(fft, real_forward_double)
{
    unsigned int fftSize = 256;
    double const twoPi {
            cnl::dsp::math::c_2_pi
    };
    using complex = cnl::dsp::complex<double>;
    using complex_vector = cnl::dsp::complex_vector<double>;
    std::vector<double> in(fftSize);
    complex_vector out(1 + fftSize / 2);

    for (unsigned int impulseLoc = 0; impulseLoc < fftSize; ++impulseLoc)
    {
        std::fill(in.begin(), in.end(), 0.0);
        in[impulseLoc] = 1.0;
        cnl::dsp::fft::real_fft<double>(in, out);
        for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
        {
            // For given impulse location FFT result can be calculated easily
            unsigned int index = (i * (impulseLoc)) % fftSize;
            double ref_angle = twoPi * (double)index / (double)fftSize;
            auto ref = complex(std::cos(ref_angle), -std::sin(ref_angle));
            EXPECT_NEAR(out[i].real(), ref.real(), FFT_DOUBLE_ACCURACY)
                                << " at index: " << i << std::endl;
            EXPECT_NEAR(out[i].imag(), ref.imag(), FFT_DOUBLE_ACCURACY)
                                << " at index: " << i << std::endl;
        }
    }
}

TEST(fft, real_forward_q4_20)
{
    unsigned int fftSize = 256;
    double const twoPi {
        cnl::dsp::math::c_2_pi
    };
    using complex_vector = cnl::dsp::complex_vector<q4_20>;
    std::vector<q4_20> in(fftSize);
    complex_vector out(1 + fftSize / 2);

    for (unsigned int impulseLoc = 0; impulseLoc < fftSize; ++impulseLoc)
    {
        std::fill(in.begin(), in.end(), q4_20(0));
        in[impulseLoc] = q4_20(1.0);
        int exponent = cnl::dsp::fft::real_fft<q4_20>(in, out);
        auto scale = pow(2.0, exponent);
        for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
        {
            // For given impulse location FFT result can be calculated easily
            unsigned int index = (i * (impulseLoc)) % fftSize;
            double ref_angle = twoPi * (double)index / (double)fftSize;
            auto ref = cnl::dsp::complex<double>(std::cos(ref_angle), -std::sin(ref_angle));
            EXPECT_NEAR(static_cast<double>(out[i].real()) * scale,
                        ref.real(), FFT_Q4_20_ACCURACY)
                            << " at index: " << i << std::endl;
            EXPECT_NEAR(static_cast<double>(out[i].imag()) * scale,
                        ref.imag(), FFT_Q4_20_ACCURACY)
                            << " at index: " << i << std::endl;
        }
    }
}

TEST(fft, forward_random_q4_20)
{
    unsigned int fftSize = 2048;

    std::mt19937 mt(91);
    std::uniform_real_distribution<double> fdist(-0.5, 0.5);

    std::vector<double>     input(fftSize);
    std::vector<q4_20>      input_fix(fftSize);
    cnl::dsp::complex_vector<double>     vec_double(fftSize);
    cnl::dsp::complex_vector<double>     vec_double_real(1 + fftSize / 2);
    cnl::dsp::complex_vector<q4_20> vec_fix(fftSize);
    cnl::dsp::complex_vector<q4_20> vec_fix_real(1 + fftSize / 2);
    std::vector<double>& re_double = vec_double.real_data();
    std::vector<q4_20>& re_fix = vec_fix.real_data();

    std::generate(input.begin(), input.end(),
                  [&] ()
                  {
                      double val = fdist(mt);
                      q4_20 val_fix = val;
                      return static_cast<double>(val_fix);
                  });
    std::transform(input.begin(),
                   input.end(),
                   input_fix.begin(),
                   [] (double& val)
                   {
                       return val;
                   });
    std::transform(input.begin(),
                   input.end(),
                   re_double.begin(),
                   [] (double& val)
                   {
                       return val;
                   });
    std::transform(input_fix.begin(),
                   input_fix.end(),
                   re_fix.begin(),
                   [] (q4_20 & val)
                   {
                       return val;
                   });
    // Check that inputs are ~ bit exact
    for (unsigned int i = 0; i < fftSize; ++i)
    {
        EXPECT_NEAR(vec_double[i].real(),
                    static_cast<double>(vec_fix[i].real()), FFT_Q4_20_ACCURACY);
        EXPECT_NEAR(vec_double[i].imag(),
                    static_cast<double>(vec_fix[i].imag()), FFT_Q4_20_ACCURACY);
    }

    cnl::dsp::fft::fft<double>(vec_double);
    cnl::dsp::fft::real_fft<double>(input, vec_double_real);

    int exponent_fix = cnl::dsp::fft::fft<q4_20>(vec_fix);
    int exponent_fix_real = cnl::dsp::fft::real_fft<q4_20>(input_fix, vec_fix_real);

    auto scale = pow(2.0, exponent_fix);
    auto scale_real = pow(2.0, exponent_fix_real);

    for (unsigned int i = 0; i < fftSize; ++i)
    {
        EXPECT_NEAR(vec_double[i].real(),
                    static_cast<double>(vec_fix[i].real()) * scale, 42*FFT_Q4_20_ACCURACY);
        EXPECT_NEAR(vec_double[i].imag(),
                    static_cast<double>(vec_fix[i].imag()) * scale, 42*FFT_Q4_20_ACCURACY);
    }

    for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
    {
        EXPECT_NEAR(vec_double_real[i].real(),
                    static_cast<double>(vec_fix_real[i].real()) * scale_real, 42*FFT_Q4_20_ACCURACY);
        EXPECT_NEAR(vec_double_real[i].imag(),
                    static_cast<double>(vec_fix_real[i].imag()) * scale_real, 42*FFT_Q4_20_ACCURACY);
    }
}

#if defined(CNL_INT128_ENABLED)
TEST(fft, forward_random_q8_40)
{
    unsigned int fftSize = 2048;

    std::mt19937 mt(91);
    std::uniform_real_distribution<double> fdist(-0.5, 0.5);

    std::vector<double>     input(fftSize);
    std::vector<q8_40>      input_fix(fftSize);
    cnl::dsp::complex_vector<double>     vec_double(fftSize);
    cnl::dsp::complex_vector<double>     vec_double_real(1 + fftSize / 2);
    cnl::dsp::complex_vector<q8_40> vec_fix(fftSize);
    cnl::dsp::complex_vector<q8_40> vec_fix_real(1 + fftSize / 2);
    std::vector<double>& re_double = vec_double.real_data();
    std::vector<q8_40>& re_fix = vec_fix.real_data();

    std::generate(input.begin(), input.end(),
                  [&] ()
                  {
                      double val = fdist(mt);
                      q8_40 val_fix = val;
                      return static_cast<double>(val_fix);
                  });
    std::transform(input.begin(),
                   input.end(),
                   input_fix.begin(),
                   [] (double& val)
                   {
                       return val;
                   });
    std::transform(input.begin(),
                   input.end(),
                   re_double.begin(),
                   [] (double& val)
                   {
                       return val;
                   });
    std::transform(input_fix.begin(),
                   input_fix.end(),
                   re_fix.begin(),
                   [] (q8_40 & val)
                   {
                       return val;
                   });
    // Check that inputs are ~ bit exact
    for (unsigned int i = 0; i < fftSize; ++i)
    {
        EXPECT_NEAR(vec_double[i].real(),
                    static_cast<double>(vec_fix[i].real()), FFT_Q8_40_ACCURACY);
        EXPECT_NEAR(vec_double[i].imag(),
                    static_cast<double>(vec_fix[i].imag()), FFT_Q8_40_ACCURACY);
    }

    cnl::dsp::fft::fft<double>(vec_double);
    cnl::dsp::fft::real_fft<double>(input, vec_double_real);

    int exponent_fix = cnl::dsp::fft::fft<q8_40>(vec_fix);
    int exponent_fix_real = cnl::dsp::fft::real_fft<q8_40>(input_fix, vec_fix_real);

    auto scale = pow(2.0, exponent_fix);
    auto scale_real = pow(2.0, exponent_fix_real);

    for (unsigned int i = 0; i < fftSize; ++i)
    {
        EXPECT_NEAR(vec_double[i].real(),
                    static_cast<double>(vec_fix[i].real()) * scale, FFT_Q8_40_ACCURACY);
        EXPECT_NEAR(vec_double[i].imag(),
                    static_cast<double>(vec_fix[i].imag()) * scale, FFT_Q8_40_ACCURACY);
    }

    for (unsigned int i = 0; i < 1 + fftSize / 2; ++i)
    {
        EXPECT_NEAR(vec_double_real[i].real(),
                    static_cast<double>(vec_fix_real[i].real()) * scale_real, FFT_Q8_40_ACCURACY);
        EXPECT_NEAR(vec_double_real[i].imag(),
                    static_cast<double>(vec_fix_real[i].imag()) * scale_real, FFT_Q8_40_ACCURACY);
    }
}
#endif
