
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/set_digits.h>

#include <cnl/_impl/wide_integer/set_digits.h>

#include <cnl/_impl/type_traits/assert_same.h>

using cnl::_impl::assert_same;

namespace {
    static_assert(
            assert_same<
                    cnl::_impl::wide_integer<7>,
                    typename cnl::set_digits<cnl::_impl::wide_integer<>, 7>::type>::value,
            "cnl::set_digits<cnl::_impl::wide_integer>");
    static_assert(
            assert_same<
                    cnl::_impl::wide_integer<8>,
                    typename cnl::set_digits<cnl::_impl::wide_integer<>, 8>::type>::value,
            "cnl::set_digits<cnl::_impl::wide_integer>");
}
