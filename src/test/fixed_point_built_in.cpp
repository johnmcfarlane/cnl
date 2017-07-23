
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
