
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/operators/overloads.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/overflow.h>

namespace {
    using cnl::_impl::identical;

    namespace test_arithmetic {
        static_assert(
                cnl::_impl::is_homogeneous_operator_tag<cnl::undefined_overflow_tag>::value, "");
        static_assert(
                cnl::_impl::enable_binary<cnl::undefined_overflow_tag, cnl::undefined_overflow_tag>::value, "");
        static_assert(
                identical(
                        true,
                        cnl::comparison_operator<
                                cnl::_impl::equal_op,
                                cnl::undefined_overflow_tag, cnl::undefined_overflow_tag>{}(
                                cnl::undefined_overflow_tag{}, cnl::undefined_overflow_tag{})),
                "cnl::convert test failed");
        static_assert(
                identical(
                        cnl::undefined_overflow_tag{},
                        cnl::binary_operator<
                                cnl::_impl::add_op,
                                cnl::_impl::native_tag, cnl::_impl::native_tag,
                                cnl::undefined_overflow_tag, cnl::undefined_overflow_tag>{}(
                                        cnl::undefined_overflow_tag{}, cnl::undefined_overflow_tag{})),
                "cnl::convert test failed");
        static_assert(
                identical(
                        cnl::undefined_overflow_tag{},
                        cnl::undefined_overflow_tag{}+cnl::undefined_overflow_tag{}),
                "cnl::convert test failed");
    }
}
