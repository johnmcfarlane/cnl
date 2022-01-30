
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief test of `cnl/cmath.h`

#include <cnl/cmath.h>

#include <test.h>

static_assert(identical(2, cnl::sqrt(4)));

TEST(cmake, sqrt_float)  // NOLINT
{
    constexpr auto expected{3.5};
    auto const actual{cnl::sqrt(12.25)};
    ASSERT_EQ(expected, actual);
}
