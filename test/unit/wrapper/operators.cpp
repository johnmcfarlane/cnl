
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/operators.h>

#include <cnl/_impl/wrapper/operators.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/wrapper.h>

using cnl::_impl::identical;

namespace {
    namespace test_equals {
        static_assert(
                identical(
                        true,
                        cnl::comparison_operator<
                                cnl::_impl::equal_op, cnl::_impl::wrapper<short>,
                                cnl::_impl::wrapper<long>>{}(
                                cnl::_impl::wrapper<short>{-3}, cnl::_impl::wrapper<long>{-3})));
        static_assert(
                identical(
                        true, cnl::_impl::wrapper<unsigned long>{3}
                                      == cnl::_impl::wrapper<unsigned short>{3}));
    }

    namespace test_minus {
        static_assert(
                identical(cnl::_impl::wrapper<>{-3}, -cnl::_impl::wrapper<>{3}));
    }

    namespace test_add {
        static_assert(cnl::_impl::is_wrapper<cnl::_impl::wrapper<>>);
        static_assert(
                identical(
                        cnl::_impl::wrapper<>{123 + 234},
                        cnl::binary_operator<
                                cnl::_impl::add_op, cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::_impl::wrapper<>, cnl::_impl::wrapper<>>{}(
                                cnl::_impl::wrapper<>{123}, cnl::_impl::wrapper<>{234})));
        static_assert(
                identical(
                        cnl::_impl::wrapper<>{123 + 234},
                        cnl::_impl::wrapper<>{123} + cnl::_impl::wrapper<>{234}));
        static_assert(
                identical(cnl::_impl::wrapper<>{123 + 234}, cnl::_impl::wrapper<>{123} + 234));
    }

    namespace test_right_shift {
        static_assert(
                identical(
                        cnl::_impl::wrapper<int64_t>{0xAA << 1},
                        cnl::_impl::wrapper<int64_t>{0xAA} << 1));
    }
}
