
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Demonstration of selectively applying functions to numbers.

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

using namespace cnl;
using cnl::_impl::identical;

// rounding safe integer
template<
        class Rep = int,
        class RoundingTag = nearest_rounding_tag,
        class OverflowTag = undefined_overflow_tag>
using rounding_overflow_int = rounding_integer<overflow_integer<Rep, OverflowTag>, RoundingTag>;

static_assert(identical(
        rounding_overflow_int<>{2} * rounding_overflow_int<>{3},
        rounding_overflow_int<>{6}), "");

static_assert(
        identical(
                INT_MAX,
                cnl::multiply<cnl::saturated_overflow_tag>(INT_MAX, INT_MAX)),
        "");

int bare_saturate(int a, int b) {
    return cnl::multiply<cnl::saturated_overflow_tag>(a, b);
}

rounding_overflow_int<> psi_saturate(rounding_overflow_int<> a, rounding_overflow_int<> b) {
    return cnl::multiply<cnl::saturated_overflow_tag>(a, b);
}
