
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
#include <cnl/auxiliary/elastic_fixed_point.h>
#include "safft.h"


TEST(safft, fft_double)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc = 15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    std::vector<std::complex<double>> vec1(fftSize,std::complex<double>(0.0,0.0));
    vec1[impulseLoc] = std::complex<double>(1.0,0.0);
    std::vector<std::complex<double>> vec2(fftSize,std::complex<double>(0.0,0.0));
    Algorithms::SaFFT<double> double_engine(fftSize);
    unsigned int ret = double_engine.fft(vec1, vec2);

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

TEST(safft, fft_fixed_point)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc =15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    using elastic_fixed_point = cnl::elastic_fixed_point<14, 16>;
    using complex = std::complex<elastic_fixed_point>;
    elastic_fixed_point zero = 0;
    elastic_fixed_point one = 1;
    complex czero = complex (zero,zero);
    complex cone = complex (one,zero);

    std::vector <complex> fix_vec1(fftSize, czero);
    fix_vec1[impulseLoc] = cone;
    std::vector <complex> fix_vec2(fftSize, czero);

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    unsigned int fix_ret = fix_engine.fft(fix_vec1, fix_vec2);
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

TEST(safft, fft_elastic_fixed_point)
{
    unsigned int fftSize = 1<<14;
    unsigned int impulseLoc = 15; //Some index smaller than fftSize
    double twopi = M_PI*2.0;

    using elastic_fixed_point = cnl::elastic_fixed_point<14, 16>;
    elastic_fixed_point zero = static_cast<elastic_fixed_point>(0);
    elastic_fixed_point one = static_cast<elastic_fixed_point>(1);
    std::complex<elastic_fixed_point > czero = std::complex<elastic_fixed_point> (zero,zero);
    std::complex<elastic_fixed_point > cone = std::complex<elastic_fixed_point> (one,zero);

    std::vector <std::complex<elastic_fixed_point > > vec1(fftSize, czero);
    vec1[impulseLoc] = cone;
    std::vector <std::complex<elastic_fixed_point > > vec2(fftSize, czero);

    Algorithms::SaFFT<elastic_fixed_point> fix_engine(fftSize);

    unsigned int ret = fix_engine.fft(vec1, vec2);
    auto ptr = (ret == 0) ? &vec1[0] : &vec2[0];

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
        ASSERT_LT(std::abs((double)ptr[i].real()-ref.real()), 0.0005);
        ASSERT_LT(std::abs((double)ptr[i].imag()-ref.imag()), 0.0005);
    }
}
