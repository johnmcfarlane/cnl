
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>

#include <cinttypes>

#define TEST_NATIVE_INTEGER
#define TEST_LABEL built_in_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = int;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"
#include <cnl/constant.h>

#if defined(__cpp_deduction_guides) && defined(CNL_P1021)

namespace test_literals_initialization {
    using cnl::_impl::identical;
    using cnl::fixed_point;

    using namespace cnl::literals;

    static_assert(identical(fixed_point<int, 0>{1}, cnl::fixed_point(1_c)));
    static_assert(identical(fixed_point<int, 1>{2}, cnl::fixed_point(2_c)));
    static_assert(identical(fixed_point<int, 0>{3}, cnl::fixed_point(3_c)));
    static_assert(identical(fixed_point<int, 2>{500}, cnl::fixed_point(500_c)));
    static_assert(identical(
            fixed_point<int32_t, 32>{INT64_C(0x7fffffff00000000)},
            cnl::fixed_point(0x7fffffff00000000_c)));
}

namespace test_literals_constant {
    using cnl::_impl::identical;
    using cnl::fixed_point;

    using namespace cnl::literals;

    constexpr auto kibi = cnl::fixed_point(1024_c);
    constexpr auto mebi = kibi * 1024_c;
    constexpr auto tebi = mebi * mebi;
    constexpr auto exbi = tebi * mebi;
    constexpr auto yobi = exbi * mebi;
    static_assert(yobi == 1208925819614629174706176.L);
}

#endif
