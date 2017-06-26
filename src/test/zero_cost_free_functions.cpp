
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Demonstration of selectively applying functions to numbers.

#include <sg14/auxiliary/precise_integer.h>
#include <sg14/auxiliary/safe_integer.h>

using namespace sg14;

// precise safe integer
template<
        class Rep = int,
        class RoundingTag = closest_rounding_policy,
        class OverflowTag = throwing_overflow_tag>
using precise_safe_integer = safe_integer<precise_integer<Rep, RoundingTag>, OverflowTag>;

static_assert(identical(
        precise_safe_integer<>{2} * precise_safe_integer<>{3},
        precise_safe_integer<>{6}), "");

static_assert(identical(
        multiply(saturated_overflow, precise_safe_integer<>{INT_MAX}, precise_safe_integer<>{INT_MAX}),
        precise_safe_integer<>{INT_MAX}), "");

int bare_saturate(int a, int b) {
    return multiply(saturated_overflow, a, b);
}

precise_safe_integer<> psi_saturate(precise_safe_integer<> a, precise_safe_integer<> b) {
    return multiply(saturated_overflow, a, b);
}
