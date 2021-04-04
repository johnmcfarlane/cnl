//          Copyright Heikki Berg 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/rounding/convert.h>

#include <cnl/_impl/rounding/convert_operator.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_scaled_integer.h>
#include <cnl/scaled_integer.h>

using cnl::_impl::identical;

namespace test_convert_nearest_rounding_native_datatypes {

    static_assert(identical(
            0.123F,
            cnl::convert<cnl::nearest_rounding_tag, float>{}(0.123F)));

    static_assert(identical(
            0.125,
            cnl::convert<cnl::nearest_rounding_tag, double>{}(0.125F)));

    static_assert(identical(
            0,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(-0.125F)));

    static_assert(identical(
            1,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(0.5F)));

    static_assert(identical(
            0,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(0.125F)));

    static_assert(identical(
            -1,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(-0.5F)));

    static_assert(identical(
            1,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(0.725F)));

    static_assert(identical(
            -1,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(-0.725F)));

    static_assert(identical(
            3,
            cnl::convert<cnl::nearest_rounding_tag, int>{}(3)));

    static_assert(identical(
            3L,
            cnl::convert<cnl::nearest_rounding_tag, long>{}(3)));

    static_assert(identical(
            3.0,
            cnl::convert<cnl::nearest_rounding_tag, double>{}(3)));
}

namespace test_convert_nearest_rounding_elastic_scaled_integer {
    static constexpr auto a = cnl::elastic_scaled_integer<8, -4>{0.3125};
    static constexpr auto b = cnl::convert<
            cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, -1>>{}(a);
    static_assert(identical(cnl::elastic_scaled_integer<4, -1>{0.5}, b));

    static constexpr auto c = cnl::convert<
            cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, -2>>{}(a);
    static_assert(identical(cnl::elastic_scaled_integer<4, -2>{0.25}, c));
}

namespace test_convert_nearest_rounding_scaled_integer {
    static constexpr auto a = cnl::scaled_integer<int, cnl::power<-4>>{0.3125};
    static constexpr auto b = cnl::convert<
            cnl::nearest_rounding_tag, cnl::scaled_integer<int, cnl::power<-1>>>{}(a);
    static_assert(identical(cnl::scaled_integer<int, cnl::power<-1>>{0.5}, b));

    static constexpr auto c = cnl::convert<
            cnl::nearest_rounding_tag, cnl::scaled_integer<int, cnl::power<-2>>>{}(a);
    static_assert(identical(cnl::scaled_integer<int, cnl::power<-2>>{0.25}, c));
}

namespace test_convert_tie_to_pos_inf_rounding_native_datatypes {

    static_assert(identical(
            0.123F,
            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, float>{}(0.123F)));

    static_assert(identical(
            0.125, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, double>{}(0.125F)));

    static_assert(identical(
            0, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(0.125F)));

    static_assert(identical(
            0, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(-0.125F)));

    static_assert(identical(
            1, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(0.5F)));

    static_assert(identical(
            1, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(0.725F)));

    static_assert(identical(
            0, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(-0.5F)));

    static_assert(identical(
            -1, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(-0.725F)));

    static_assert(identical(
            3, cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(3)));

    static_assert(identical(
            3L,
            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, long>{}(3)));

    static_assert(identical(
            3.0,
            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, double>{}(3)));
}

namespace test_convert_native_rounding {
    static_assert(
            cnl::_impl::identical(
                    short{3},
                    cnl::custom_operator<
                            cnl::_impl::convert_op,
                            cnl::op_value<long, cnl::native_rounding_tag>,
                            cnl::op_value<short, cnl::native_rounding_tag>>{}(3L)),
            "custom_operator<_impl::convert_op, native_rounding_tag, native_rounding_tag, short, long>");
}
