//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <gtest/gtest.h>

#include <dsp/dsp_types.h>
#include <dsp/dsp_math.h>

TEST(basic_math, floor)
{
    std::vector<float> nbrs {
        0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f,
        1.7f, 1.8f, 1.9f, 2.0f, 2.1f, 2.2f, 2.3f, 2.4f,
        -0.9f, -1.0f, -1.1f, -1.2f, -1.3f, -1.4f, -1.5f, -1.6f,
        -1.7f, -1.8f, -1.9f, -2.0f, -2.1f, -2.2f, -2.3f, -2.4f
    };
    std::vector<float> res {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f,
        -1.0f, -1.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f,
        -2.0f, -2.0f, -2.0f, -2.0f, -3.0f, -3.0f, -3.0f, -3.0f
    };

    {
        for (std::size_t index = 0; index < nbrs.size(); ++index)
        {
            float a = nbrs[index];
            float expected = res[index];
            EXPECT_EQ(cnl::dsp::math::floor(a), expected)
            << "index: " << index << std::endl;
        }
    }
    {
        for (std::size_t index = 0; index < nbrs.size(); ++index)
        {
            q4_20 a = nbrs[index];
            q4_20 expected = res[index];
            EXPECT_EQ(static_cast<float>(cnl::dsp::math::floor(a)), res[index])
            << "index: " << index << std::endl;
            EXPECT_EQ(cnl::dsp::math::floor(a), expected)
            << "index: " << index << std::endl;
        }
    }


}

TEST(basic_math, div)
{

    q4_20 a(1.0);
    q4_20 b(2.0);
    q4_20 c1 = cnl::make_fixed_point(cnl::make_fractional(b,a));
    q4_20 c2 = cnl::make_fixed_point(cnl::make_fractional(a,b));

    EXPECT_EQ(static_cast<float>(c1), 2.0);
    EXPECT_EQ(static_cast<float>(c2), 0.5);
}


/*
TEST(basic_math, round)
{

    using q4_4 = rsfp<4, 4>;
    using q4_1 = rsfp<4, 1>;
    q4_4 a = 0.4375;
    q4_1 b = a;
    EXPECT_EQ(a, 0.4375);
    EXPECT_EQ(b, 0.5);

}
*/
