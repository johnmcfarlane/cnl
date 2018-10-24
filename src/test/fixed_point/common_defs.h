
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// included from fixed_point_common.h;
// contains definitions that aid testing and which depend upon definition of test_int

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

////////////////////////////////////////////////////////////////////////////////
// test name generation

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

////////////////////////////////////////////////////////////////////////////////
// imports from std

using std::is_same;
using std::declval;

////////////////////////////////////////////////////////////////////////////////
// integer definitions
//
// depends upon test_signed and test_unsigned defined in including source file

static_assert(sizeof(int)==4, "warning: many of the tests in this file assume a 4-byte integer!z");

using test_signed = test_int;
using test_unsigned = cnl::make_unsigned_t<test_signed>;

using int8 = cnl::set_digits_t<test_signed, 7>;
using uint8 = cnl::set_digits_t<test_unsigned, 8>;
using int16 = cnl::set_digits_t<test_signed, 15>;
using uint16 = cnl::set_digits_t<test_unsigned, 16>;
using int32 = cnl::set_digits_t<test_signed, 31>;
using uint32 = cnl::set_digits_t<test_unsigned, 32>;
using int64 = cnl::set_digits_t<test_signed, 63>;
using uint64 = cnl::set_digits_t<test_unsigned, 64>;

#if defined(CNL_INT128_ENABLED) || defined(TEST_WIDE_INTEGER)
using int128 = cnl::set_digits_t<test_signed, 127>;
using uint128 = cnl::set_digits_t<test_unsigned, 128>;
#endif

////////////////////////////////////////////////////////////////////////////////
// imports from cnl

template <typename Rep=test_int, int Exponent=0>
using fixed_point = cnl::fixed_point<Rep, Exponent>;

using cnl::_impl::identical;
