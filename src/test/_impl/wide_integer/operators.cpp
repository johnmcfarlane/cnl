
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/operators.h>

#include <cnl/_impl/wide_integer/operators.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_equals {
        static_assert(
                identical(
                        true,
                        cnl::_impl::binary_operator<
                                cnl::_impl::equal_op,
                                cnl::_impl::wide_integer<15>,
                                cnl::_impl::wide_integer<63>>{}(
                                        cnl::_impl::wide_integer<15>{-3},
                                        cnl::_impl::wide_integer<63>{-3})),
                "cnl::_impl::wide_integer equality");
        static_assert(
                identical(
                        true,
                        cnl::_impl::wide_integer<64, unsigned>{3}==cnl::_impl::wide_integer<16, unsigned>{3}),
                "cnl::_impl::wide_integer equality");
    }

    namespace test_minus {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<>{-3},
                        -cnl::_impl::wide_integer<>{3}),
                "cnl::_impl::wide_integer minus");
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<>{123+234},
                        cnl::_impl::wide_integer<>{123}+cnl::_impl::wide_integer<>{234}),
                "cnl::_impl::wide_integer add");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<>{123+234},
                        cnl::_impl::wide_integer<>{123}+234),
                "cnl::_impl::wide_integer add");
        static_assert(
                identical(
                        cnl::_impl::wide_integer<8, signed>{5},
                        cnl::_impl::wide_integer<8, unsigned>{2}+cnl::_impl::wide_integer<7, signed>{3}),
                "cnl::_impl::wide_integer add");
    }

    namespace test_right_shift {
        static_assert(
                identical(
                        cnl::_impl::wide_integer<63>{0xAA<<1},
                        cnl::_impl::wide_integer<63>{0xAA}<<1),
                "cnl::_impl::wide_integer right shift");
    }
}
