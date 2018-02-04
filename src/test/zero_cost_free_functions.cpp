
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Demonstration of selectively applying functions to numbers.

#include <cnl/rounding_integer.h>
#include <cnl/overflow_integer.h>

using namespace cnl;

// rounding safe integer
template<
        class Rep = int,
        class RoundingTag = closest_rounding_tag,
        class OverflowTag = throwing_overflow_tag>
using rounding_overflow_int = overflow_integer<rounding_integer<Rep, RoundingTag>, OverflowTag>;

static_assert(identical(
        rounding_overflow_int<>{2} * rounding_overflow_int<>{3},
        rounding_overflow_int<>{6}), "");

static_assert(identical(
        multiply(saturated_overflow, rounding_overflow_int<>{INT_MAX}, rounding_overflow_int<>{INT_MAX}),
        rounding_overflow_int<>{INT_MAX}), "");

int bare_saturate(int a, int b) {
    return multiply(saturated_overflow, a, b);
}

rounding_overflow_int<> psi_saturate(rounding_overflow_int<> a, rounding_overflow_int<> b) {
    return multiply(saturated_overflow, a, b);
}
