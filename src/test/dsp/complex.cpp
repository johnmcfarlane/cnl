//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <gtest/gtest.h>

#include <dsp/dsp_types.h>
#include <dsp/complex.h>

TEST(complex, basic)
{
    {
        cnl::dsp::complex<float> num1;
        EXPECT_EQ(num1.real(),0.0f);
        EXPECT_EQ(num1.imag(),0.0f);
        cnl::dsp::complex<float> num2(3.0f);
        EXPECT_EQ(num2.real(),3.0f);
        EXPECT_EQ(num2.imag(),0.0f);
        cnl::dsp::complex<float> num3(1.125f,2.5f);
        EXPECT_EQ(num3.real(),1.125f);
        EXPECT_EQ(num3.imag(),2.5f);
        cnl::dsp::complex<float> num4(num3);
        EXPECT_EQ(num4.real(),1.125f);
        EXPECT_EQ(num4.imag(),2.5f);
        cnl::dsp::complex<float> num5(std::move(num4));
        EXPECT_EQ(num5.real(),1.125f);
        EXPECT_EQ(num5.imag(),2.5f);
    }
    {
        cnl::dsp::complex<q4_20> num1;
        EXPECT_EQ(num1.real(),0.0f);
        EXPECT_EQ(num1.imag(),0.0f);
        cnl::dsp::complex<q4_20> num2(3.0f);
        EXPECT_EQ(num2.real(),3.0f);
        EXPECT_EQ(num2.imag(),0.0f);
        cnl::dsp::complex<q4_20> num3(1.125f,2.5f);
        EXPECT_EQ(num3.real(),1.125f);
        EXPECT_EQ(num3.imag(),2.5f);
        cnl::dsp::complex<q4_20> num4(num3);
        EXPECT_EQ(num4.real(),1.125f);
        EXPECT_EQ(num4.imag(),2.5f);
        cnl::dsp::complex<q4_20> num5(std::move(num4));
        EXPECT_EQ(num5.real(),1.125f);
        EXPECT_EQ(num5.imag(),2.5f);
    }
}

TEST(complex, operations)
{
    {
        cnl::dsp::complex<float> num1(1.0f, 1.0f);
        cnl::dsp::complex<float> num2(1.0f, 2.0f);
        auto sum = num1 + num2;
        EXPECT_EQ(sum.real(), 2.0f);
        EXPECT_EQ(sum.imag(), 3.0f);

        sum += cnl::dsp::complex<float>(1.0f, 0.0f);
        EXPECT_EQ(sum.real(), 3.0f);
        EXPECT_EQ(sum.imag(), 3.0f);

        auto diff = num1 - num2;
        EXPECT_EQ(diff.real(), 0.0f);
        EXPECT_EQ(diff.imag(), -1.0f);

        diff -= cnl::dsp::complex<float>(2.0f, 0.0f);
        EXPECT_EQ(diff.real(), -2.0f);
        EXPECT_EQ(diff.imag(), -1.0f);

        auto prod = num1 * num2;
        EXPECT_EQ(prod.real(), -1.0f);
        EXPECT_EQ(prod.imag(), 3.0f);

        prod *= cnl::dsp::complex<float>(0.5f, 0.0f);
        EXPECT_EQ(prod.real(), -0.5f);
        EXPECT_EQ(prod.imag(), 1.5f);

        auto div = num1 / cnl::dsp::complex<float>(2.0f, 0.0f);
        EXPECT_EQ(div.real(), 0.5f);
        EXPECT_EQ(div.imag(), 0.5f);

        auto abs = cnl::dsp::abs(num1);
        EXPECT_EQ(abs, 1.414213562373095f);

        auto con = cnl::dsp::conj(num2);
        EXPECT_EQ(con.real(), 1.0f);
        EXPECT_EQ(con.imag(), -2.0f);
    }
    {
        cnl::dsp::complex<q4_20> num1(1.0f, 1.0f);
        cnl::dsp::complex<q4_20> num2(1.0f, 2.0f);
        auto sum = num1 + num2;
        EXPECT_EQ(sum.real(), 2.0f);
        EXPECT_EQ(sum.imag(), 3.0f);

        sum += cnl::dsp::complex<q4_20>(1.0f, 0.0f);
        EXPECT_EQ(sum.real(), 3.0f);
        EXPECT_EQ(sum.imag(), 3.0f);

        auto diff = num1 - num2;
        EXPECT_EQ(diff.real(), 0.0f);
        EXPECT_EQ(diff.imag(), -1.0f);

        diff -= cnl::dsp::complex<q4_20>(2.0f, 0.0f);
        EXPECT_EQ(diff.real(), -2.0f);
        EXPECT_EQ(diff.imag(), -1.0f);

        auto prod = num1 * num2;
        EXPECT_EQ(prod.real(), -1.0f);
        EXPECT_EQ(prod.imag(), 3.0f);

        prod *= cnl::dsp::complex<q4_20>(0.5f, 0.0f);
        EXPECT_EQ(prod.real(), -0.5f);
        EXPECT_EQ(prod.imag(), 1.5f);

        auto div = num1 / cnl::dsp::complex<q4_20>(2.0f, 0.0f);
        EXPECT_EQ(div.real(), 0.5f);
        EXPECT_EQ(div.imag(), 0.5f);

        auto abs = cnl::dsp::abs(num1);
        EXPECT_NEAR(static_cast<float>(abs), 1.41421f, 1e-5f);

        auto con = cnl::dsp::conj(num2);
        EXPECT_EQ(con.real(), 1.0f);
        EXPECT_EQ(con.imag(), -2.0f);
    }
}
