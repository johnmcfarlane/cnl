//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <gtest/gtest.h>

#include "dsp/dsp_types.h"
#include "dsp/dsp_math.h"
#include "dsp/trig.h"

static constexpr double trig_accuracy = 1e-15;
static constexpr double trig_interpolation_accuracy = 1e-7;
static constexpr double trig_accuracy_q4_20 = 1e-6;
static constexpr double trig_interpolation_accuracy_q4_20 = 2e-6;

TEST(trig, cos_idx_double) {
    cnl::dsp::trig<double>& myTrig = cnl::dsp::trig<double>::instance();
    std::size_t twoPiIndex = myTrig.get_twopi_index();
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= 8 * twoPiIndex; ++i)
    {
        double val = myTrig.cos_at(i);
        // Let's do the modulo here for the reference higher accuracy
        std::size_t k = i % twoPiIndex;
        //Allow error due to PI rounding
        EXPECT_NEAR(std::cos(twoPi * double(k) / double(twoPiIndex)),
                    val, trig_accuracy)  << "At index : " << i << std::endl;
    }
}

 TEST(trig, sin_idx_double) {
    cnl::dsp::trig<double>& myTrig = cnl::dsp::trig<double>::instance();
    std::size_t twoPiIndex = myTrig.get_twopi_index();
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= 8 * twoPiIndex; ++i)
    {
        double val = myTrig.sin_at(i);
        std::size_t k = i % twoPiIndex;
        EXPECT_NEAR(std::sin(twoPi * double(k) / double(twoPiIndex)),
                    val, trig_accuracy) << "At index : " << i << std::endl;

    }
}

TEST(trig, cos_turn_double) {
    cnl::dsp::trig<double>& myTrig = cnl::dsp::trig<double>::instance();
    std::size_t tests = 1 << 18;
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= tests; ++i)
    {
        double turn = (double)i / (double)tests;
        double radians = turn * twoPi;
        EXPECT_NEAR(std::cos(radians), myTrig.cos_turn(turn),
                    trig_interpolation_accuracy);
    }
}


TEST(trig, sin_turn_double) {
    cnl::dsp::trig<double>& myTrig = cnl::dsp::trig<double>::instance();
    std::size_t tests = 1 << 18;
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= tests; ++i)
    {
        double turn = (double)i / (double)tests;
        double radians = turn * twoPi;
        EXPECT_NEAR(std::sin(radians), myTrig.sin_turn(turn),
                    trig_interpolation_accuracy);
    }
}

TEST(trig, cos_idx_q4_20) {
    cnl::dsp::trig<q4_20>& myTrig = cnl::dsp::trig<q4_20>::instance();
    std::size_t twoPiIndex = myTrig.get_twopi_index();
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= 8 * twoPiIndex; ++i)
    {
        q4_20 val = myTrig.cos_at(i);
        std::size_t k = i % twoPiIndex;
        EXPECT_NEAR(std::cos(twoPi * double(k) / double(twoPiIndex)),
                    static_cast<double>(val), trig_accuracy_q4_20)
                        << "At index : " << i << std::endl;
    }
}


TEST(trig, sin_idx_q4_20) {
    cnl::dsp::trig<q4_20>& myTrig = cnl::dsp::trig<q4_20>::instance();
    std::size_t twoPiIndex = myTrig.get_twopi_index();
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= 8 * twoPiIndex; ++i)
    {
        q4_20 val = myTrig.sin_at(i);
        std::size_t k = i % twoPiIndex;
        EXPECT_NEAR(std::sin(twoPi * double(k) / double(twoPiIndex)),
                    static_cast<double>(val), trig_accuracy_q4_20)
                        << "At index : " << i << std::endl;

    }
}

TEST(trig, cos_turn_q4_20) {
    cnl::dsp::trig<q4_20>& myTrig = cnl::dsp::trig<q4_20>::instance();
    std::size_t tests = 1 << 18;
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= tests; ++i)
    {
        double turn = (double)i / (double)tests;
        double radians = turn * twoPi;
        EXPECT_NEAR(std::cos(radians),
                    static_cast<double>(myTrig.cos_turn(static_cast<q4_20>(turn))),
                    trig_interpolation_accuracy_q4_20);
    }
}


TEST(trig, sin_turn_q4_20) {
    cnl::dsp::trig<q4_20>& myTrig = cnl::dsp::trig<q4_20>::instance();
    std::size_t tests = 1 << 18;
    double const twoPi {cnl::dsp::math::c_2_pi};

    for (std::size_t i = 0; i <= tests; ++i)
    {
        double turn = (double)i / (double)tests;
        double radians = turn * twoPi;
        EXPECT_NEAR(std::sin(radians),
                    static_cast<double>(myTrig.sin_turn(static_cast<q4_20>(turn))),
                    trig_interpolation_accuracy_q4_20);
    }
}
