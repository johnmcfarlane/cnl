
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <integer.h>
#include <fixed_point.h>

#define TEST_NATIVE_OVERFLOW
#define TEST_LABEL native_integer_

#if defined(_MSC_VER)
#define TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE
#define TEST_IGNORE_MSVC_INTERNAL_ERRORS
#endif

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Repr type

using test_int = sg14::native_integer<>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"
