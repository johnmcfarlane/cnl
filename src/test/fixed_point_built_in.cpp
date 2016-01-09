
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cinttypes>

#define TEST_NATIVE_OVERFLOW
#define TEST_LABEL built_in_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Repr type

using test_unsigned = unsigned;
using test_signed = signed;
using test_int = int;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
#if defined(_GLIBCXX_USE_INT128)
using int128 = __int128;
using uint128 = unsigned __int128;
#endif

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"
