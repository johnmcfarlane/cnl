
//          Copyright Heikki Berg 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define _USE_MATH_DEFINES // define M_PI in <cmath>

#include <iostream>
#include <iomanip>
#include <complex>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include <cnl/fixed_point.h>
#include "safft.h"


TEST(fft, safft_double)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc = 15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    std::vector<std::complex<double>> vec1(fftSize,std::complex<double>(0.0,0.0));
    vec1[impulseLoc] = std::complex<double>(1.0,0.0);
    std::vector<std::complex<double>> vec2(fftSize,std::complex<double>(0.0,0.0));
    Algorithms::SaFFT<double> double_engine(fftSize);
    unsigned int ret = double_engine.sa_fft(vec1, vec2);

    auto ptr = (ret == 0) ? &vec1[0] : &vec2[0];

    std::complex<double> ref;
    unsigned int index;
    double ref_angle;
    for (unsigned int i=0;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        index = (i*(impulseLoc))%fftSize;
        ref_angle = twopi*(double)index/(double)fftSize;
        ref = std::complex<double>(cos(ref_angle),-sin(ref_angle));
        //This will be pretty accurate...
        ASSERT_LT(std::abs(ptr[i]-ref), 0.00000000000001);
    }
}

TEST(fft, safft_fixed_point)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc =15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    using elastic_fixed_point = cnl::elastic_fixed_point<30, -16>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1;
    complex czero = complex (zero,zero);
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, czero);
    fix_vec1[impulseLoc] = cone;
    std::vector <complex> fix_vec2(fftSize, czero);

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    unsigned int fix_ret = fix_engine.sa_fft(fix_vec1, fix_vec2);
    auto fix_ptr = (fix_ret == 0) ? &fix_vec1[0] : &fix_vec2[0];

    std::complex<double> ref;
    unsigned int index;
    double ref_angle;
    for (unsigned int i=0;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        index = (i*(impulseLoc))%fftSize;
        ref_angle = twopi*(double)index/(double)fftSize;
        ref = std::complex<double>(cos(ref_angle),-sin(ref_angle));
        //Accuracy vs. double will be smaller
        //TODO: Acceptable FFT accuracy
        ASSERT_LT(std::abs((double)fix_ptr[i].real()-ref.real()), 0.0005);
        ASSERT_LT(std::abs((double)fix_ptr[i].imag()-ref.imag()), 0.0005);
    }
}

TEST(fft, saifft_fixed_point)
{
    unsigned int fftSize = 1<<14;

    using elastic_fixed_point = cnl::elastic_fixed_point<30, -16>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1;
    complex czero = complex (zero,zero);
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, cone);
    std::vector <complex> fix_vec2(fftSize, czero);

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    // Stockham autosort FFT using two buffers
    unsigned int fix_ret = fix_engine.sa_ifft(fix_vec1, fix_vec2);
    auto fix_ptr = (fix_ret == 0) ? &fix_vec1[0] : &fix_vec2[0];

    std::complex<double> refdc = fftSize;
    std::complex<double> ref = 0;
    ASSERT_LT(std::abs((double)fix_ptr[0].real()-refdc.real()), 0.0005);
    ASSERT_LT(std::abs((double)fix_ptr[0].imag()-refdc.imag()), 0.0005);
    for (unsigned int i=1;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        //TODO: Acceptable FFT accuracy
        ASSERT_LT(std::abs((double)fix_ptr[i].real()-ref.real()), 0.0005);
        ASSERT_LT(std::abs((double)fix_ptr[i].imag()-ref.imag()), 0.0005);
    }
}

TEST(fft, ctfft_fixed_point)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc =15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    using elastic_fixed_point = cnl::elastic_fixed_point<30, -16>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1;
    complex czero = complex (zero,zero);
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, czero);
    fix_vec1[impulseLoc] = cone;

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    // In-place FFT
    fix_engine.ct_fft(fix_vec1);

    std::complex<double> ref;
    unsigned int index;
    double ref_angle;
    for (unsigned int i=0;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        index = (i*(impulseLoc))%fftSize;
        ref_angle = twopi*(double)index/(double)fftSize;
        ref = std::complex<double>(cos(ref_angle),-sin(ref_angle));
        //Accuracy vs. double will be smaller
        //TODO: Acceptable FFT accuracy
        ASSERT_LT(std::abs((double)fix_vec1[i].real()-ref.real()), 0.0005);
        ASSERT_LT(std::abs((double)fix_vec1[i].imag()-ref.imag()), 0.0005);
    }
}

TEST(fft, ctifft_fixed_point)
{
    unsigned int fftSize = 1<<14;

    using elastic_fixed_point = cnl::elastic_fixed_point<30, -16>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1;
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, cone);

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    fix_engine.ct_ifft(fix_vec1);

    std::complex<double> refdc = fftSize;
    std::complex<double> ref = 0;
    ASSERT_LT(std::abs((double)fix_vec1[0].real()-refdc.real()), 0.0005);
    ASSERT_LT(std::abs((double)fix_vec1[0].imag()-refdc.imag()), 0.0005);
    for (unsigned int i=1;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        //TODO: Acceptable FFT accuracy
        ASSERT_LT(std::abs((double)fix_vec1[i].real()-ref.real()), 0.0005);
        ASSERT_LT(std::abs((double)fix_vec1[i].imag()-ref.imag()), 0.0005);
    }
}

TEST(fft, blockfft_fixed_point)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc =15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    using elastic_fixed_point = cnl::elastic_fixed_point<24, -23>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1.0;
    complex czero = complex (zero,zero);
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, czero);
    fix_vec1[impulseLoc] = cone;

    Algorithms::SaFFT<elastic_fixed_point> fft_engine(fftSize);

    // In-place FFT
    int norm = fft_engine.bf_fft(fix_vec1);
    double scale = pow(2.0,(double)-norm);

    std::complex<double> ref;
    unsigned int index;
    double ref_angle;
    for (unsigned int i=0;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        index = (i*(impulseLoc))%fftSize;
        ref_angle = twopi*(double)index/(double)fftSize;
        ref = std::complex<double>(cos(ref_angle),-sin(ref_angle));
        //Accuracy vs. double will be smaller
        //TODO: Acceptable FFT accuracy
        ASSERT_LT(std::abs(((double)fix_vec1[i].real())*scale-ref.real()), 0.00001);
        ASSERT_LT(std::abs(((double)fix_vec1[i].imag())*scale-ref.imag()), 0.00001);
    }
}

TEST(fft, blockifft_fixed_point)
{
    unsigned int fftSize = 1<<14;

    using elastic_fixed_point = cnl::elastic_fixed_point<24, -23>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1;
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, cone);

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    int norm = fix_engine.bf_ifft(fix_vec1);
    double scale = pow(2.0,(double)-norm);

    std::complex<double> refdc = fftSize;
    std::complex<double> ref = 0;
    ASSERT_LT(std::abs((double)fix_vec1[0].real()*scale-refdc.real()), 0.00001);
    ASSERT_LT(std::abs((double)fix_vec1[0].imag()*scale-refdc.imag()), 0.00001);
    for (unsigned int i=1;i < fftSize;++i) {
        //For given impulse location FFT result can be calculated easily
        //TODO: Acceptable FFT accuracy
        ASSERT_LT(std::abs((double)fix_vec1[i].real()*scale-ref.real()), 0.00001);
        ASSERT_LT(std::abs((double)fix_vec1[i].imag()*scale-ref.imag()), 0.00001);
    }
    std::cout << "Total normalization: " << norm << std::endl;
}

