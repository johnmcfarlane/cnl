
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
using test_int = sg14::native_integer<int>;
using int8 = sg14::native_integer<std::int8_t>;
using uint8 = sg14::native_integer<std::uint8_t>;
using int16 = sg14::native_integer<std::int16_t>;
using uint16 = sg14::native_integer<std::uint16_t>;
using int32 = sg14::native_integer<std::int32_t>;
using uint32 = sg14::native_integer<std::uint32_t>;
using int64 = sg14::native_integer<std::int64_t>;
using uint64 = sg14::native_integer<std::uint64_t>;
#if defined(_GLIBCXX_USE_INT128)
using int128 = sg14::native_integer<__int128>;
using uint128 = sg14::native_integer<unsigned __int128>;
#endif

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"
