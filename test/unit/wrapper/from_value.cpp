
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/from_value.h>

#include <cnl/_impl/wrapper/from_value.h>

#include <cnl/_impl/wrapper/operators.h>
#include <test.h>

namespace {
    static_assert(
            identical(
                    cnl::_impl::wrapper<unsigned long long>{654},
                    cnl::from_value<cnl::_impl::wrapper<>, unsigned long long>{}(654ULL)));
    static_assert(
            identical(
                    cnl::_impl::wrapper<unsigned long long>{654},
                    cnl::_impl::from_value<cnl::_impl::wrapper<>>(654ULL)));
}
