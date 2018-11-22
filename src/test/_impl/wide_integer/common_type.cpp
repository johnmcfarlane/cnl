
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/common_type.h>

#include <cnl/_impl/wide_integer/common_type.h>

#include <cnl/_impl/type_traits/assert_same.h>

using cnl::_impl::assert_same;

namespace {
    static_assert(
            assert_same<
                    cnl::_impl::wide_integer<16, int>,
                    typename std::common_type<
                            cnl::_impl::wide_integer<16, unsigned char>,
                            cnl::_impl::wide_integer<4, signed char>>::type>::value,
            "cnl::common_type<cnl::_impl::wide_integer>");
}
