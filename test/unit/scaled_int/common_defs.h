
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// included from scaled_int_common.h;
// contains definitions that aid testing and which depend upon definition of test_int

#if !defined(TEST_SCALED_INTEGER_COMMON_DEFS_H)
#define TEST_SCALED_INTEGER_COMMON_DEFS_H

#include <cnl/_impl/num_traits/adopt_width.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
// test name generation

#define TOKENPASTE(x, y) x##y  // NOLINT(cppcoreguidelines-macro-usage)
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// imports from std

using std::declval;  // NOLINT(google-global-names-in-headers)
using std::is_same;  // NOLINT(google-global-names-in-headers)

////////////////////////////////////////////////////////////////////////////////
// integer definitions
//
// depends upon test_signed and test_unsigned defined in including source file

static_assert(
        sizeof(int) == 4, "warning: many of the tests in this file assume a 4-byte integer!");

using test_signed = test_int;
using test_unsigned = cnl::numbers::set_signedness_t<test_signed, false>;

using int8 = cnl::set_digits_t<test_signed, 7>;
using uint8 = cnl::set_digits_t<test_unsigned, 8>;
using int16 = cnl::set_digits_t<test_signed, 15>;
using uint16 = cnl::set_digits_t<test_unsigned, 16>;
using int32 = cnl::set_digits_t<test_signed, 31>;
using uint32 = cnl::set_digits_t<test_unsigned, 32>;
using int64 = cnl::set_digits_t<test_signed, 63>;
using uint64 = cnl::set_digits_t<test_unsigned, 64>;

#if defined(CNL_INT128_ENABLED) || defined(TEST_WIDE_INTEGER)
using int128_t = cnl::set_digits_t<test_signed, 127>;
using uint128_t = cnl::set_digits_t<test_unsigned, 128>;
#endif

////////////////////////////////////////////////////////////////////////////////
// imports from cnl

template<typename Rep = test_int, class Scale = cnl::power<>>
using scaled_integer = cnl::scaled_integer<Rep, Scale>;

using cnl::_impl::assert_same;  // NOLINT(google-global-names-in-headers)
using cnl::_impl::identical;  // NOLINT(google-global-names-in-headers)

#endif
