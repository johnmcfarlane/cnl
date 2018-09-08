
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/from_rep.h>

#include <cnl/_impl/wide_integer/from_rep.h>

#include <cnl/_impl/type_traits/assert_same.h>

using cnl::_impl::assert_same;

namespace {
    static_assert(
            assert_same<
                    cnl::_impl::wide_integer<31, int>,
                    cnl::_impl::from_rep_t<cnl::_impl::wide_integer<>, std::int64_t>>::value,
            "cnl::from_rep_t<cnl::_impl::wide_integer>");
    static_assert(
            assert_same<
                    cnl::_impl::wide_integer<31, unsigned int>,
                    cnl::_impl::from_rep_t<cnl::_impl::wide_integer<>, std::uint32_t>>::value,
            "cnl::from_rep_t<cnl::_impl::wide_integer>");
//    static_assert(
//            assert_same<
//                    cnl::_impl::wide_integer<9>,
//                    cnl::_impl::from_rep_t<cnl::_impl::wide_integer<>, cnl::constant<400>>>::value,
//            "cnl::from_rep_t<cnl::_impl::wide_integer>");
}
