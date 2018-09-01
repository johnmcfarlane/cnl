
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

#include <cinttypes>

#define TEST_NATIVE_INTEGER
#define TEST_LABEL rounding_integer_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = cnl::rounding_integer<int, cnl::native_rounding_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"
#include <cnl/constant.h>

#if defined(__cpp_deduction_guides)

namespace test_literals_initialization {
    using cnl::_impl::identical;
    using cnl::fixed_point;

    using namespace cnl::literals;

    static_assert(identical(fixed_point<int, 0>{1}, fixed_point(1_c)));
    static_assert(identical(fixed_point<int, 1>{2}, fixed_point(2_c)));
    static_assert(identical(fixed_point<int, 0>{3}, fixed_point(3_c)));
    static_assert(identical(fixed_point<int, 2>{500}, fixed_point(500_c)));
    static_assert(identical(fixed_point<int32_t, 32>{INT64_C(0x7fffffff00000000)}, fixed_point(0x7fffffff00000000_c)));
}

namespace test_literals_constant {
    using cnl::_impl::identical;
    using cnl::fixed_point;

    using namespace cnl::literals;

    constexpr auto kibi = fixed_point(1024_c);
    constexpr auto mebi = kibi * 1024_c;
    constexpr auto tebi = mebi * mebi;
    constexpr auto exbi = tebi * mebi;
    constexpr auto yobi = exbi * mebi;
    static_assert(yobi == 1208925819614629174706176.L);
}

#endif
