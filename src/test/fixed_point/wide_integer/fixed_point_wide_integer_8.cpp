
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/num_traits/digits.h>
#include <cnl/fixed_point.h>
#include <cnl/wide_integer.h>

#include <cinttypes>

// TODO: Every `#if !defined(TEST_WIDE_INTEGER)` is a TODO
#define TEST_WIDE_INTEGER
#define TEST_WIDE_INTEGER_8
#define TEST_LABEL wide_integer_8_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// wide_integer type used as fixed_point Rep type

using test_int = cnl::wide_integer<cnl::digits<int>::value, cnl::int8>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"
