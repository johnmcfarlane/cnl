
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
                        cnl::_impl::number<int, cnl::nearest_rounding_tag>{54},
                        cnl::binary_operator<
                                cnl::_impl::multiply_op,
                                cnl::_impl::native_tag,
                                cnl::_impl::native_tag,
                                cnl::_impl::number<int, cnl::nearest_rounding_tag>,
                                int>{}(cnl::_impl::number<int, cnl::nearest_rounding_tag>{6}, 9)),
                "binary_operator<multiply_op, native_tag, native_tag, nearest_rounding_integer, int>");


        static_assert(
                identical(
                        cnl::_impl::number<cnl::elastic_integer<13, int>, cnl::nearest_rounding_tag>{42},
                        cnl::from_value<
                                cnl::_impl::number<cnl::elastic_integer<24, int>, cnl::nearest_rounding_tag>,
                                cnl::elastic_integer<13, int>>{}(
                                cnl::elastic_integer<13, int>{42})),
                "");
    }

    namespace acme {
        template<typename T>
        class number
        {};
    }
    namespace test_convert_operator {
        static_assert(
                identical(0,
                        cnl::convert_operator<
                                cnl::power<-20, 2>,
                                cnl::power<0, 2>,
                                cnl::_impl::number<cnl::elastic_integer<24, int>, cnl::nearest_rounding_tag>,
                                acme::number<
                                        cnl::scaled_integer<
                                                cnl::_impl::number<cnl::elastic_integer<24, int>, cnl::nearest_rounding_tag>,
                                                cnl::power<-20, 2>>>>{}(0)),
                "");
    }
}
