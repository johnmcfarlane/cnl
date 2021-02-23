
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/scaled_integer.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_multiply {
        static_assert(
                cnl::_impl::identical(
                        cnl::_impl::wrapper<int, cnl::nearest_rounding_tag>{54},
                        cnl::custom_operator<
                                cnl::_impl::multiply_op,
                                cnl::op_value<cnl::_impl::wrapper<int, cnl::nearest_rounding_tag>>,
                                cnl::op_value<int>>{}(
                                cnl::_impl::wrapper<int, cnl::nearest_rounding_tag>{6}, 9)));

        static_assert(
                identical(
                        cnl::_impl::wrapper<
                                cnl::elastic_integer<13, int>, cnl::nearest_rounding_tag>{42},
                        cnl::from_value<
                                cnl::_impl::wrapper<
                                        cnl::elastic_integer<24, int>, cnl::nearest_rounding_tag>,
                                cnl::elastic_integer<13, int>>{}(cnl::elastic_integer<13, int>{
                                42})));
    }
}
