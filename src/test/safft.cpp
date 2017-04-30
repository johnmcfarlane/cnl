
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
#include <sg14/auxiliary/elastic_fixed_point.h>
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

    fixed_point<int32_t,-16> zero = static_cast<fixed_point<int32_t,-16>>(0);
    fixed_point<int32_t,-16> one = static_cast<fixed_point<int32_t,-16>>(1);
    std::complex<fixed_point<int32_t,-16> > czero = std::complex<fixed_point<int32_t,-16>> (zero,zero);
    std::complex<fixed_point<int32_t,-16> > cone = std::complex<fixed_point<int32_t,-16>> (one,zero);

    std::vector <std::complex<fixed_point<int32_t,-16> > > fix_vec1(fftSize, czero);
    fix_vec1[impulseLoc] = cone;
    std::vector <std::complex<fixed_point<int32_t,-16> > > fix_vec2(fftSize, czero);

    Algorithms::SaFFT<fixed_point<int32_t,-16>> fix_engine(fftSize);

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

    elastic_fixed_point<16,16> zero = static_cast<elastic_fixed_point<16,16>>(0);
    elastic_fixed_point<16,16> one = static_cast<elastic_fixed_point<16,16>>(1);
    std::complex<elastic_fixed_point<16,16> > czero = std::complex<elastic_fixed_point<16,16>> (zero,zero);
    std::complex<elastic_fixed_point<16,16> > cone = std::complex<elastic_fixed_point<16,16>> (one,zero);

    std::vector <std::complex<elastic_fixed_point<16,16> > > vec1(fftSize, czero);
    vec1[impulseLoc] = cone;
    std::vector <std::complex<elastic_fixed_point<16,16> > > vec2(fftSize, czero);

    Algorithms::SaFFT<elastic_fixed_point<16,16>> fix_engine(fftSize);

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
