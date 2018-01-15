
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Demonstration of selectively applying functions to numbers.

#include <cnl/precise_integer.h>
#include <cnl/overflow_integer.h>

using namespace cnl;

// precise safe integer
template<
        class Rep = int,
        class RoundingTag = closest_rounding_tag,
        class OverflowTag = throwing_overflow_tag>
using precise_overflow_int = overflow_integer<precise_integer<Rep, RoundingTag>, OverflowTag>;

static_assert(identical(
        precise_overflow_int<>{2} * precise_overflow_int<>{3},
        precise_overflow_int<>{6}), "");

static_assert(identical(
        multiply(saturated_overflow, precise_overflow_int<>{INT_MAX}, precise_overflow_int<>{INT_MAX}),
        precise_overflow_int<>{INT_MAX}), "");

int bare_saturate(int a, int b) {
    return multiply(saturated_overflow, a, b);
}

precise_overflow_int<> psi_saturate(precise_overflow_int<> a, precise_overflow_int<> b) {
    return multiply(saturated_overflow, a, b);
}
