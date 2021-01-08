
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for `cnl::set_rep` of `cnl::_impl::wrapper`

#include <cnl/_impl/wrapper/set_rep.h>

#include <cnl/_impl/type_traits/assert_same.h>

using cnl::_impl::assert_same;

namespace {
    static_assert(
            assert_same<
                    cnl::_impl::wrapper<long long>,
                    cnl::_impl::set_rep_t<cnl::_impl::wrapper<>, long long>>::value);
    static_assert(
            assert_same<cnl::_impl::wrapper<>, cnl::_impl::set_rep_t<cnl::_impl::wrapper<>, int>>::
                    value);
}
