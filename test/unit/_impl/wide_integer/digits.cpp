
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
            identical(
                    cnl::digits<cnl::int8>::value,
                    cnl::digits<cnl::wide_integer<7>>::value),
            "cnl::digits<cnl::wide_integer>");
}
