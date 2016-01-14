
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <integer.h>

#define TEST_NATIVE_OVERFLOW
#define TEST_LABEL native_integer_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Repr type

using test_unsigned = sg14::native_integer<unsigned>;
using test_signed = sg14::native_integer<signed>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"
