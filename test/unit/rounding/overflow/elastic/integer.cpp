
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

template<int Digits>
using saturated_elastic_integer = cnl::rounding_integer<
        cnl::overflow_integer<cnl::elastic_integer<Digits, int>, cnl::saturated_overflow_tag>,
        cnl::native_rounding_tag>;

namespace test_saturated_scale {
    static_assert(
            identical(
                    saturated_elastic_integer<2>{2},
                    cnl::_impl::scale<1>(saturated_elastic_integer<1>{1})),
            "");
}
