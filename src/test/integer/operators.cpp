
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/number/operators.h>

#include <cnl/_impl/integer.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_equals {
        static_assert(
                identical(
                        true,
                        cnl::comparison_operator<
                                cnl::_impl::equal_op,
                                cnl::_impl::integer<short>,
                                cnl::_impl::integer<long>>{}(
                                        cnl::_impl::integer<short>{-3},
                                        cnl::_impl::integer<long>{-3})),
                "cnl::_impl::integer equality");
        static_assert(
                identical(
                        true,
                        cnl::_impl::integer<unsigned long>{3}==cnl::_impl::integer<unsigned short>{3}),
                "cnl::_impl::integer equality");
    }

    namespace test_minus {
        static_assert(
                identical(
                        cnl::_impl::integer<>{-3},
                        -cnl::_impl::integer<>{3}),
                "cnl::_impl::integer minus");
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::_impl::integer<>{123+234},
                        cnl::_impl::integer<>{123}+cnl::_impl::integer<>{234}),
                "cnl::_impl::integer add");
        static_assert(
                identical(
                        cnl::_impl::integer<>{123+234},
                        cnl::_impl::integer<>{123}+234),
                "cnl::_impl::integer add");
    }

    namespace test_right_shift {
        static_assert(
                identical(
                        cnl::_impl::integer<int64_t>{0xAA<<1},
                        cnl::_impl::integer<int64_t>{0xAA}<<1),
                "cnl::_impl::integer right shift");
    }
}
