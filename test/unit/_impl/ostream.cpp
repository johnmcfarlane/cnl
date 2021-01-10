
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/ostream.h>

#include <cnl/_impl/ostream.h>

#include <gtest/gtest.h>

namespace {
#if defined(CNL_INT128_ENABLED)
    TEST(ostream, int128)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::int128{15};
        ASSERT_EQ("15", s.str());  // NOLINT
    }

    TEST(ostream, uint128)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::uint128{15};
        ASSERT_EQ("15", s.str());  // NOLINT
    }
#endif
}
