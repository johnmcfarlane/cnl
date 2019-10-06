//          Copyright Heikki Berg 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/rounding/convert.h>

#include <cnl/_impl/rounding/convert_operator.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_scaled_integer.h>
#include <cnl/scaled_integer.h>


using cnl::_impl::assert_same;
using cnl::_impl::identical;


namespace  test_convert_nearest_rounding_native_datatypes
{

    static_assert(
            cnl::_impl::identical(0.123F, cnl::convert<cnl::nearest_rounding_tag, float, float>(0.123F)),
            "cnl::convert<nearest_rounding_tag, float, float>");

    static_assert(
            cnl::_impl::identical(0.125, cnl::convert<cnl::nearest_rounding_tag, double, float>(0.125F)),
            "cnl::convert<nearest_rounding_tag, double, float>");

    static_assert(
            cnl::_impl::identical(0, cnl::convert<cnl::nearest_rounding_tag, int, float>(0.125F)),
            "cnl::convert<nearest_rounding_tag, int, float>");

    static_assert(
            cnl::_impl::identical(1, cnl::convert<cnl::nearest_rounding_tag, int, float>(0.725F)),
            "cnl::convert<nearest_rounding_tag, int, float>");

    static_assert(
            cnl::_impl::identical(-1, cnl::convert<cnl::nearest_rounding_tag, int, float>(-0.725F)),
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

namespace test_convert_nearest_rounding_elastic_scaled_integer
{
    static constexpr auto a = cnl::elastic_scaled_integer<8, -4>{0.3125};
    static constexpr auto b =
            cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, -1>, cnl::elastic_scaled_integer<8, -4>>(a);
    static_assert(identical(cnl::elastic_scaled_integer<4, -1>{0.5}, b),
            "cnl::convert<nearest_rounding_tag, elastic_scaled_integer, elastic_scaled_integer>");

    static constexpr auto c =
            cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, -2>, cnl::elastic_scaled_integer<8, -4>>(a);
    static_assert(identical(cnl::elastic_scaled_integer<4, -2>{0.25}, c),
            "cnl::convert<nearest_rounding_tag, elastic_scaled_integer, elastic_scaled_integer>");
}

namespace test_convert_nearest_rounding_scaled_integer
{
    static constexpr auto a = cnl::scaled_integer<int, cnl::power<-4>>{0.3125};
    static constexpr auto b =
            cnl::convert<
                    cnl::nearest_rounding_tag,
                    cnl::scaled_integer<int, cnl::power<-1>>,
                    cnl::scaled_integer<int, cnl::power<-4>>>(a);
    static_assert(identical(cnl::scaled_integer<int, cnl::power<-1>>{0.5}, b),
            "cnl::convert<nearest_rounding_tag, scaled_integer, scaled_integer>");

    static constexpr auto c =
            cnl::convert<
                    cnl::nearest_rounding_tag,
                    cnl::scaled_integer<int, cnl::power<-2>>,
                    cnl::scaled_integer<int, cnl::power<-4>>>(a);
    static_assert(identical(cnl::scaled_integer<int, cnl::power<-2>>{0.25}, c),
            "cnl::convert<nearest_rounding_tag, scaled_integer, scaled_integer>");
}
