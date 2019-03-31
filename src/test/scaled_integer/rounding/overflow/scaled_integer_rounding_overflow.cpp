
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/scaled_integer.h>
#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

#include <cinttypes>

#define TEST_NATIVE_INTEGER
#define TEST_LABEL static_integer_

////////////////////////////////////////////////////////////////////////////////
// integer types used as scaled_integer Rep type

using test_int = cnl::rounding_integer<cnl::overflow_integer<int, cnl::native_overflow_tag>, cnl::native_rounding_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform scaled_integer tests with this type of scaled_integer specialization

#include <scaled_integer/scaled_integer_common.h>
#include <cnl/constant.h>

#if defined(__cpp_deduction_guides)

namespace test_literals_initialization {
    using cnl::_impl::identical;
    using cnl::scaled_integer;

    using namespace cnl::literals;

    static_assert(identical(scaled_integer<int, 0>{1}, scaled_integer(1_c)));
    static_assert(identical(scaled_integer<int, 1>{2}, scaled_integer(2_c)));
    static_assert(identical(scaled_integer<int, 0>{3}, scaled_integer(3_c)));
    static_assert(identical(scaled_integer<int, 2>{500}, scaled_integer(500_c)));
    static_assert(identical(scaled_integer<int32_t, 32>{INT64_C(0x7fffffff00000000)}, scaled_integer(0x7fffffff00000000_c)));
}

namespace test_literals_constant {
    using cnl::_impl::identical;
    using cnl::scaled_integer;

    using namespace cnl::literals;

    constexpr auto kibi = scaled_integer(1024_c);
    constexpr auto mebi = kibi * 1024_c;
    constexpr auto tebi = mebi * mebi;
    constexpr auto exbi = tebi * mebi;
    constexpr auto yobi = exbi * mebi;
    static_assert(yobi == 1208925819614629174706176.L);
}

#endif
