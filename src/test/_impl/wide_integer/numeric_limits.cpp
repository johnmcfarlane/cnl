
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/numeric_limits.h>

#include <cnl/_impl/wide_integer/numeric_limits.h>
#include <cnl/_impl/wide_integer/operators.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    static_assert(
            cnl::numeric_limits<cnl::_impl::wide_integer<>>::is_specialized,
            "cnl::numeric_limits<cnl::_impl::wide_integer>::is_specialized");
    static_assert(
            identical(
                    cnl::_impl::wide_integer<8, unsigned>{255},
                    cnl::numeric_limits<cnl::_impl::wide_integer<8, unsigned>>::max()),
            "cnl::numeric_limits<cnl::_impl::wide_integer>::max");
    static_assert(
            identical(
                    cnl::_impl::wide_integer<10>{1023},
                    cnl::numeric_limits<cnl::_impl::wide_integer<10>>::max()),
            "");
    static_assert(
            identical(
                    cnl::_impl::wide_integer<6>{-64},
                    cnl::numeric_limits<cnl::_impl::wide_integer<6>>::lowest()),
            "");
    static_assert(
            cnl::numeric_limits<cnl::_impl::wide_integer<100>>::lowest()
                    <-cnl::numeric_limits<cnl::_impl::wide_integer<100>>::max(),
            "");
}
