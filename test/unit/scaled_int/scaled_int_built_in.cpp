
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/scaled_integer.h>

#include <cinttypes>

#define TEST_NATIVE_INTEGER
#define TEST_LABEL built_in_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// integer types used as scaled_integer Rep type

using test_int = int;

////////////////////////////////////////////////////////////////////////////////
// perform scaled_integer tests with this type of scaled_integer specialization

#include "scaled_int_common.h"

namespace test_literals_initialization {
    using cnl::scaled_integer;
    using cnl::_impl::identical;

    using namespace cnl::literals;

    static_assert(identical(scaled_integer<int, cnl::power<0>>{1}, cnl::make_scaled_integer(1_c)));
    static_assert(identical(scaled_integer<int, cnl::power<1>>{2}, cnl::make_scaled_integer(2_c)));
    static_assert(identical(scaled_integer<int, cnl::power<0>>{3}, cnl::make_scaled_integer(3_c)));
    static_assert(identical(scaled_integer<int, cnl::power<2>>{500}, cnl::make_scaled_integer(500_c)));
    static_assert(identical(
            scaled_integer<int32_t, cnl::power<32>>{INT64_C(0x7fffffff00000000)},
            cnl::make_scaled_integer(0x7fffffff00000000_c)));
}

namespace test_literals_constant {
    using cnl::scaled_integer;  // NOLINT(misc-unused-using-decls)

    using namespace cnl::literals;

    constexpr auto kibi = cnl::make_scaled_integer(1024_c);
    constexpr auto mebi = kibi * 1024_c;
    constexpr auto tebi = mebi * mebi;
    constexpr auto exbi = tebi * mebi;
    constexpr auto yobi = exbi * mebi;
    static_assert(yobi == 1208925819614629174706176.L);
}
