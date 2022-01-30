
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/make_integer.h>

#include <cnl/_impl/wide_integer/make_wide_integer.h>

#include <cnl/_impl/wide_integer/operators.h>
#include <test.h>

namespace {
    static_assert(
            identical(cnl::wide_integer<31>{5}, cnl::_impl::make_wide_integer(int{5})),
            "cnl::make_wide_integer");
    static_assert(
            identical(cnl::wide_integer<63>{1}, cnl::_impl::make_wide_integer(INT64_C(1))),
            "cnl::make_wide_integer");
}
