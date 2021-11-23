
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// example code presented on the front page of the Doxygen documentation
// which exhibits undefined behavior that can be trapped by UBSan

#include <cnl/all.h>

#include <gtest/gtest.h>

#include <iostream>

#if defined(CNL_IOSTREAMS_ENABLED)
////////////////////////////////////////////////////////////////////////////////
//! [scaled_integer overflow example 1]
void overflow_example1()
{
    // fixed-point value of 65,536 * 2^-16
    auto num = cnl::scaled_integer<int, cnl::power<-16>>{1};

    // fixed-point value of 4,294,967,296 * 2^-32
    auto square = num * num;

    std::cout << square;
}
//! [scaled_integer overflow example 1]

//! [scaled_integer overflow example 2]
void overflow_example2()
{
    // integer variable with value 65,536
    auto num = 65536;

    // integer variable with value 4,294,967,296
    auto square = num * num;

    std::cout << square;
}
//! [scaled_integer overflow example 2]

TEST(index, faq_overflow_example)  // NOLINT
{
#if defined(CNL_DEBUG)
    ASSERT_DEATH(overflow_example1(), "");  // NOLINT
    ASSERT_DEATH(overflow_example2(), "");  // NOLINT
#endif
}
#endif
