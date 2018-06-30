
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for cnl/limits.h

#include <cnl/limits.h>
#include <cnl/cnlint.h>

namespace {
    // Sanity tests: if any of these fail, please proceed with caution!
    // The author would be very interested to learn more.
    
    static_assert(CHAR_BIT == 8, "untested platform");
    static_assert(std::is_same<signed char, std::int8_t>::value, "untested platform");
    static_assert(std::is_same<unsigned char, std::uint8_t>::value, "untested platform");
    static_assert(std::is_same<short, std::int16_t>::value, "untested platform");
    static_assert(std::is_same<unsigned short, std::uint16_t>::value, "untested platform");
    static_assert(std::is_same<int, std::int32_t>::value, "untested platform");
    static_assert(std::is_same<unsigned int, std::uint32_t>::value, "untested platform");
    static_assert(sizeof(long long) == 8, "untested platform");
    static_assert(cnl::numeric_limits<int8_t>::lowest()==-128, "");

#if defined(CNL_INT128_ENABLED)
    namespace test_cnl_numeric_limits_int128 {
        using numeric_limits = cnl::numeric_limits<cnl::int128>;
        static_assert(numeric_limits::is_specialized, "cnl::numeric_limits<cnl::int128> test failed");
        static_assert(numeric_limits::is_integer, "cnl::numeric_limits<cnl::int128> test failed");
        static_assert(numeric_limits::is_signed, "cnl::numeric_limits<cnl::int128> test failed");
    }

    namespace test_cnl_numeric_limits_uint128 {
        using numeric_limits = cnl::numeric_limits<cnl::uint128>;
        static_assert(numeric_limits::is_specialized, "cnl::numeric_limits<cnl::uint128> test failed");
        static_assert(numeric_limits::is_integer, "cnl::numeric_limits<cnl::uint128> test failed");
        static_assert(!numeric_limits::is_signed, "cnl::numeric_limits<cnl::uint128> test failed");
    }
#endif
}
