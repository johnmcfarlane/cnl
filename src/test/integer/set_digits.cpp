
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/integer/set_digits.h>

#include <cnl/_impl/integer/set_digits.h>

#include <cnl/_impl/type_traits/assert_same.h>

using cnl::_impl::assert_same;

namespace {
    static_assert(
            assert_same<
                    cnl::_impl::integer<int8_t>,
                    typename cnl::set_digits<cnl::_impl::integer<>, 7>::type>::value,
            "cnl::set_digits<cnl::_impl::integer>");
    static_assert(
            assert_same<
                    cnl::_impl::integer<int16_t>,
                    typename cnl::set_digits<cnl::_impl::integer<>, 8>::type>::value,
            "cnl::set_digits<cnl::_impl::integer>");
    static_assert(
            assert_same<
                    cnl::_impl::integer<int16_t>,
                    typename cnl::set_digits<cnl::_impl::integer<>, 8>::type>::value,
            "cnl::set_digits<cnl::_impl::integer>");
}
