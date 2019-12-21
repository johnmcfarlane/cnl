
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/fixed_point.h>
#include <cnl/rounding.h>

using cnl::_impl::identical;

namespace {
    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::fixed_point<int, -1>{0.5};
        static constexpr auto actual = cnl::convert<
                cnl::power<>,
                cnl::nearest_rounding_tag,
                cnl::fixed_point<int, -1>>(cnl::fixed_point<int, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point, cnl::fixed_point>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::fixed_point<int, -2>{-0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::power<>,
                cnl::fixed_point<int, -2>>(cnl::fixed_point<int, -4>{-0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point, cnl::fixed_point>");
    }

    namespace test_nearest_round_up_float {
        static constexpr auto expected = cnl::fixed_point<int, -2>{-0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::power<>,
                cnl::fixed_point<int, -2>>(-0.3125F);

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point, cnl::fixed_point>");
    }

    namespace test_truncate_round_up {
        static constexpr auto expected = cnl::fixed_point<int, 0>{1};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::power<>,
                cnl::fixed_point<int, 0>>(cnl::fixed_point<int, -2>{0.75});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag>"); //Fails gives 0.75 as result.
    }

    namespace test_nearest_int {
        static_assert(
                identical(
                        3,
                        cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int>(
                                cnl::fixed_point<int, -2>{2.5})),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int, cnl::fixed_point>");
        static_assert(
                identical(
                        2,
                        cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int>(
                                cnl::fixed_point<int, -2>{2.4375})),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int, cnl::fixed_point>");
    }

    namespace test_nearest_from_int {
        static_assert(
                identical(
                        cnl::fixed_point<int, 2>{4},
                        cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point<int, 2>>(5)),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point, int>");
        static_assert(
                identical(
                        cnl::fixed_point<int, 2>{8},
                        cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point<int, 2>>(6)),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::fixed_point, int>");
    }

    namespace test_native_int {
        static_assert(
                identical(
                        2,
                        cnl::convert_operator<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                int,
                                cnl::fixed_point<int, -2>>{}(2.5)),
                "cnl::convert<cnl::native_rounding_tag, int, cnl::fixed_point>");
        static_assert(
                identical(
                        2,
                        cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int>(
                                cnl::fixed_point<int, -2>{2.4375})),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int, cnl::fixed_point>");
    }

    namespace test_native_from_int {
        static_assert(
                identical(
                        cnl::fixed_point<int, 2>{4},
                        cnl::convert<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                cnl::fixed_point<int, 2>>(5)),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point, int>");
        static_assert(
                identical(
                        cnl::fixed_point<int, 2>{4},
                        cnl::convert<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                cnl::fixed_point<int, 2>>(6)),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point, int>");
    }

    namespace fixed_point_nearest_rounding {

        // Positive
        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{5.25}),
                cnl::fixed_point<int32_t, 0>{5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{5.25}),
                84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{5.0}),
                5),
                "");

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{5.5}),
                cnl::fixed_point<int32_t, 0>{6.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{5.5}),
                88),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{6.0}),
                6),
                "");

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, -1>>
                        (cnl::fixed_point<int32_t, -4>{5.25}),
                cnl::fixed_point<int32_t, -1>{5.5}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{5.25}),
                84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -1>{5.5}),
                11),
                "");

        // Negative
        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{-5.25}),
                cnl::fixed_point<int32_t, 0>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{-5.0}),
                -5),
                "");

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{-5.5}),
                cnl::fixed_point<int32_t, 0>{-6.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{-5.5}),
                -88),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{-6.0}),
                -6),
                "");

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, -1>>
                        (cnl::fixed_point<int32_t, -4>{-5.25}),
                cnl::fixed_point<int32_t, -1>{-5.5}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -1>{-5.5}),
                -11),
                "");
    }

    namespace fixed_point_tie_to_pos_inf_rounding {

        // Positive
        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{5.25}),
                cnl::fixed_point<int32_t, 0>{5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{5.25}),
                84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{5.0}),
                5),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{5.5}),
                cnl::fixed_point<int32_t, 0>{6.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{5.5}),
                88),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{6.0}),
                6),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, -1>>
                        (cnl::fixed_point<int32_t, -4>{5.25}),
                cnl::fixed_point<int32_t, -1>{5.5}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{5.25}),
                84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -1>{5.5}),
                11),
                "");

        // Negative
        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{-5.25}),
                cnl::fixed_point<int32_t, 0>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{-5.0}),
                -5),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, 0>>
                        (cnl::fixed_point<int32_t, -4>{-5.5}),
                cnl::fixed_point<int32_t, 0>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{-5.5}),
                -88),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, 0>{-5.0}),
                -5),
                "");

        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::fixed_point<int32_t, -1>>
                        (cnl::fixed_point<int32_t, -4>{-5.25}),
                cnl::fixed_point<int32_t, -1>{-5.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::fixed_point<int32_t, -1>{-5.0}),
                -10),
                "");
    }

}
