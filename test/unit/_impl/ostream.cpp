
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/ostream.h>

#include <cnl/_impl/ostream.h>

#include <gtest/gtest.h>

namespace {
#if defined(CNL_INT128_ENABLED) && defined(CNL_IOSTREAMS_ENABLED)
    TEST(ostream, int128_42)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::int128{42};
        ASSERT_EQ("42", s.str());  // NOLINT
    }

    TEST(ostream, int128_max)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::numeric_limits<cnl::int128>::max();
        ASSERT_EQ("170141183460469231731687303715884105727", s.str());  // NOLINT
    }

    TEST(ostream, int128_min_plus_one)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::numeric_limits<cnl::int128>::min() + 1;
        ASSERT_EQ("-170141183460469231731687303715884105727", s.str());  // NOLINT
    }

    TEST(ostream, uint128_42)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::uint128{42};
        ASSERT_EQ("42", s.str());  // NOLINT
    }

    TEST(ostream, uint128_max)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::numeric_limits<cnl::uint128>::max();
        ASSERT_EQ("340282366920938463463374607431768211455", s.str());  // NOLINT
    }

    TEST(ostream, uint128_min)  // NOLINT
    {
        using cnl::operator<<;
        std::stringstream s;
        s << cnl::numeric_limits<cnl::uint128>::min();
        ASSERT_EQ("0", s.str());  // NOLINT
    }
#endif
}
