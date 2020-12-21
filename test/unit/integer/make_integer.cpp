
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/number/make_number.h>

#include <cnl/_impl/integer.h>

#include <cnl/_impl/number/operators.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    static_assert(
            identical(cnl::_impl::integer<long>{1}, cnl::_impl::make_integer(1L)),
            "cnl::make_integer<cnl::_impl::integer>");
}
