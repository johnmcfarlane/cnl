
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/multiword_integer/from_rep.h>

#include <cnl/_impl/multiword_integer/from_rep.h>
#include <cnl/_impl/multiword_integer/operators.h>
#include <cnl/_impl/multiword_integer/type.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_from_rep {
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long, 4>{4},
                        cnl::_impl::from_rep<cnl::_impl::multiword_integer<long, 4>>(4)), "");
    }
}
