
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding.h>
#include <cnl/wide_integer.h>

#include <test.h>

namespace {
    namespace test_convert_native_rounding {
        static_assert(
                identical(
                        cnl::_impl::wrapper<int, cnl::wide_tag<31, int>>{42},
                        cnl::custom_operator<
                                cnl::_impl::convert_op,
                                cnl::op_value<cnl::_impl::wrapper<int, cnl::wide_tag<31, int>>, cnl::native_rounding_tag>,
                                cnl::op_value<cnl::_impl::wrapper<int, cnl::wide_tag<31, int>>, cnl::nearest_rounding_tag>>{}(42)),
                "custom_operator<_impl::convert_op, nearest_rounding_tag, native_rounding_tag, wide_integer, "
                "wide_integer>");
    }
}
