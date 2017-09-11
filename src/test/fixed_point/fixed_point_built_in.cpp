
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>

#include <cinttypes>

#define TEST_NATIVE_OVERFLOW
#define TEST_LABEL built_in_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = int;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"

#if defined(__cpp_deduction_guides)
namespace test_literals_initialization {
    using cnl::_impl::identical;
    using cnl::fixed_point;

    using namespace cnl::literals;

    static_assert(identical(fixed_point<int, 0>{1}, fixed_point(1_c)));
    static_assert(identical(fixed_point<int, 1>{2}, fixed_point(2_c)));
    static_assert(identical(fixed_point<int, 0>{3}, fixed_point(3_c)));
    static_assert(identical(fixed_point<int, 2>{500}, fixed_point(500_c)));
}
#endif
