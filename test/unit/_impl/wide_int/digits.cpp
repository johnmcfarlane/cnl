
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/digits.h>

#include <cnl/_impl/wide_integer/digits.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    static_assert(
            identical(cnl::digits_v<std::int8_t>, cnl::digits_v<cnl::wide_integer<7>>),
            "cnl::digits_v<cnl::wide_integer>");
}
