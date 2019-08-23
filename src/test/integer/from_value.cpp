
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/number/from_value.h>

#include <cnl/_impl/integer.h>

#include <cnl/_impl/number/operators.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    static_assert(
            identical(
                    cnl::_impl::integer<unsigned long long>{654},
                    cnl::from_value<cnl::_impl::integer<>, unsigned long long>{}(654ULL)),
            "cnl::from_value<cnl::_impl::integer>");
    static_assert(
            identical(
                    cnl::_impl::integer<unsigned long long>{654},
                    cnl::_impl::from_value<cnl::_impl::integer<>>(654ULL)),
            "cnl::from_value<cnl::_impl::integer>");
}
