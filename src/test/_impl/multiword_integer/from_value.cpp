
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/multiword_integer/from_value.h>

#include <cnl/_impl/multiword_integer/from_value.h>
#include <cnl/_impl/multiword_integer/operators.h>
#include <cnl/_impl/multiword_integer/type.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_from_value {
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long, 4>{4},
                        cnl::_impl::make_number<cnl::_impl::multiword_integer<long, 4>>(4)), "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long unsigned, 2>{123},
                        cnl::_impl::make_number<cnl::_impl::multiword_integer<long unsigned, 2>>(123)),
                "");
    }
}
