
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/overflow_integer.h>

#define TEST_UNDEFINED_OVERFLOW_INTEGER
#define TEST_LABEL undefined_overflow_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = cnl::overflow_integer<int, cnl::undefined_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"
