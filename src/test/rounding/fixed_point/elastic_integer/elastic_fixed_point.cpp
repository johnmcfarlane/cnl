
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_fixed_point.h>
#include <cnl/rounding.h>

using cnl::_impl::identical;

namespace {
    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::elastic_fixed_point<4, -1>{0.5};
        static constexpr auto actual = cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_fixed_point<4, -1>>(
                cnl::elastic_fixed_point<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_fixed_point, cnl::elastic_fixed_point>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::elastic_fixed_point<4, -2>{0.25};
        static constexpr auto actual = cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_fixed_point<4, -2>>(
                cnl::elastic_fixed_point<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_fixed_point, cnl::elastic_fixed_point>");
    }

    namespace elastic_fixed_point_tie_to_pos_inf_rounding {

        // Positive
        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::elastic_fixed_point<16, 0>>
                        (cnl::elastic_fixed_point<16, -4>{5.25}),
                cnl::elastic_fixed_point<16, 0>{5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{5.25}),
                84),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{5.0}),
                5),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::elastic_fixed_point<16, 0>>
                        (cnl::elastic_fixed_point<16, -4>{5.5}),
                cnl::elastic_fixed_point<16, 0>{6.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{5.5}),
                88),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{6.0}),
                6),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::elastic_fixed_point<16, -1>>
                        (cnl::elastic_fixed_point<16, -4>{5.25}),
                cnl::elastic_fixed_point<16, -1>{5.5}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{5.25}),
                84),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -1>{5.5}),
                11),
                "");

        // Negative
        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::elastic_fixed_point<16, 0>>
                        (cnl::elastic_fixed_point<16, -4>{-5.25}),
                cnl::elastic_fixed_point<16, 0>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{-5.0}),
                -5),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::elastic_fixed_point<16, 0>>
                        (cnl::elastic_fixed_point<16, -4>{-5.5}),
                cnl::elastic_fixed_point<16, 0>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.5}),
                -88),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{-5.0}),
                -5),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::elastic_fixed_point<16, -1>>
                        (cnl::elastic_fixed_point<16, -4>{-5.25}),
                cnl::elastic_fixed_point<16, -1>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -1>{-5.0}),
                -10),
                "");
    }
}
