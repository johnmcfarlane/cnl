
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief test of `cnl/cmath.h`

#include <cnl/cmath.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

static_assert(identical(2, cnl::sqrt(4)));

TEST(cmake, sqrt_float)  // NOLINT
{
    auto const expected{new double{3.5}};
    auto const actual{cnl::sqrt(12.25)};
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    ASSERT_EQ(*expected, actual);
    if (actual > 42) {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        delete expected;
    }
}
