
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/numeric_limits.h>

#include <cnl/_impl/wide_integer/numeric_limits.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/wide_integer.h>

#include <limits>

using cnl::_impl::identical;

namespace {
    static_assert(
            std::numeric_limits<cnl::wide_integer<>>::is_specialized,
            "std::numeric_limits<cnl::wide_integer>::is_specialized");
    static_assert(
            identical(
                    cnl::wide_integer<8, unsigned>{255},
                    std::numeric_limits<cnl::wide_integer<8, unsigned>>::max()),
            "std::numeric_limits<cnl::wide_integer>::max");
    static_assert(
            identical(
                    cnl::wide_integer<10>{1023}, std::numeric_limits<cnl::wide_integer<10>>::max()));
    static_assert(
            identical(
                    cnl::wide_integer<6>{-64}, std::numeric_limits<cnl::wide_integer<6>>::lowest()));
    static_assert(
            std::numeric_limits<cnl::wide_integer<100>>::lowest()
            < -std::numeric_limits<cnl::wide_integer<100>>::max());
}
