//          Copyright Heikki Berg 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/rounding/convert.h>

#include <cnl/_impl/rounding/tagged_convert_operator.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_number.h>
#include <cnl/scaled_integer.h>


using cnl::_impl::assert_same;
using cnl::_impl::identical;


namespace  test_convert_nearest_rounding_native_datatypes
{

    static_assert(
            cnl::_impl::identical(0.123f, cnl::convert<cnl::nearest_rounding_tag, float, float>(0.123f)),
            "cnl::convert<nearest_rounding_tag, float, float>");

    static_assert(
            cnl::_impl::identical(0.125, cnl::convert<cnl::nearest_rounding_tag, double, float>(0.125f)),
            "cnl::convert<nearest_rounding_tag, double, float>");

    static_assert(
            cnl::_impl::identical(0, cnl::convert<cnl::nearest_rounding_tag, int, float>(0.125f)),
            "cnl::convert<nearest_rounding_tag, int, float>");

    static_assert(
            cnl::_impl::identical(1, cnl::convert<cnl::nearest_rounding_tag, int, float>(0.725f)),
            "cnl::convert<nearest_rounding_tag, int, float>");

    static_assert(
            cnl::_impl::identical(-1, cnl::convert<cnl::nearest_rounding_tag, int, float>(-0.725f)),
            "cnl::convert<nearest_rounding_tag, int, float>");

    static_assert(
            cnl::_impl::identical(3, cnl::convert<cnl::nearest_rounding_tag, int, int>(3)),
            "cnl::convert<nearest_rounding_tag, int, int>");

    static_assert(
            cnl::_impl::identical(3L, cnl::convert<cnl::nearest_rounding_tag, long, int>(3)),
            "cnl::convert<nearest_rounding_tag, long, int>");

    static_assert(
            cnl::_impl::identical(3.0, cnl::convert<cnl::nearest_rounding_tag, double, int>(3)),
            "cnl::convert<nearest_rounding_tag, double, int>");
}

namespace test_convert_nearest_rounding_elastic_number
{
    static constexpr auto a = cnl::elastic_number<8, -4>{0.3125};
    static constexpr auto b =
            cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_number<4, -1>, cnl::elastic_number<8, -4>>(a);
    static_assert(identical(cnl::elastic_number<4, -1>{0.5}, b),
            "cnl::convert<nearest_rounding_tag, elastic_number, elastic_number>");

    static constexpr auto c =
            cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_number<4, -2>, cnl::elastic_number<8, -4>>(a);
    static_assert(identical(cnl::elastic_number<4, -2>{0.25}, c),
            "cnl::convert<nearest_rounding_tag, elastic_number, elastic_number>");
}

namespace test_convert_nearest_rounding_fixed_point
{
    static constexpr auto a = cnl::fixed_point<int, -4>{0.3125};
    static constexpr auto b =
            cnl::convert<cnl::nearest_rounding_tag, cnl::fixed_point<int, -1>, cnl::fixed_point<int, -4>>(a);
    static_assert(identical(cnl::fixed_point<int, -1>{0.5}, b),
            "cnl::convert<nearest_rounding_tag, fixed_point, fixed_point>");

    static constexpr auto c =
            cnl::convert<cnl::nearest_rounding_tag, cnl::fixed_point<int, -2>, cnl::fixed_point<int, -4>>(a);
    static_assert(identical(cnl::fixed_point<int, -2>{0.25}, c),
            "cnl::convert<nearest_rounding_tag, fixed_point, fixed_point>");
}
