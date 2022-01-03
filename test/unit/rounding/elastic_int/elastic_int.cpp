
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/wrapper/from_rep.h>

#include <gtest/gtest.h>

#include <type_traits>

namespace {
    namespace test_shift_right_nearest {
        static_assert(
                identical(
                        cnl::elastic_integer<53>{7000 >> 9},
                        cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(
                                cnl::elastic_integer<62>{7000}, cnl::constant<9>{})));

#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::elastic_integer<117>{7000 >> 9},
                        cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(
                                cnl::elastic_integer<126>{7000}, cnl::constant<9>{})));
#endif
    }

    namespace test_rounding {
        static_assert(
                std::is_same_v<
                        cnl::native_rounding_tag,
                        typename cnl::rounding<cnl::elastic_integer<>>::type>);
    }

    namespace test_multiply {
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
