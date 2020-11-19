
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
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::power<>,
                cnl::elastic_fixed_point<4, -1>>(cnl::elastic_fixed_point<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::elastic_fixed_point, cnl::elastic_fixed_point>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::elastic_fixed_point<4, -2>{0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::power<>,
                cnl::elastic_fixed_point<4, -2>>(
                cnl::elastic_fixed_point<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::elastic_fixed_point, cnl::elastic_fixed_point>");
    }

    namespace elastic_fixed_point_nearest_rounding {

        // Positive
        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, -1>>
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
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
                        (cnl::elastic_fixed_point<16, -4>{-5.5}),
                cnl::elastic_fixed_point<16, 0>{-6.0}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.5}),
                -88),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{-6.0}),
                -6),
                "");

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, -1>>
                        (cnl::elastic_fixed_point<16, -4>{-5.25}),
                cnl::elastic_fixed_point<16, -1>{-5.5}),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.25}),
                -84),
                "");
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -1>{-5.5}),
                -11),
                "");
    }

    using T1 = cnl::elastic_fixed_point<16, -8>;
    using T2 = cnl::elastic_fixed_point<8, -4>;
    using T3 = cnl::elastic_fixed_point<5, -1>;

    namespace elastic_fixed_point_implicit_conversions {
        static_assert(identical(T2{0.25}, static_cast<T2>(T1{0.25})),
                "conversion 1 (elastic_fixed_point)");
        static_assert(identical(T2{-0.25}, static_cast<T2>(T1{-0.25})),
                "conversion 2 (elastic_fixed_point)");
        static_assert(identical(T3{0.0}, static_cast<T3>(T2{0.25})),
                "conversion 3 (elastic_fixed_point)");
        static_assert(identical(T3{0.0}, static_cast<T3>(T2{-0.25})),
                "conversion 3 (elastic_fixed_point)");
        static_assert(identical(T3{-0.5}, static_cast<T3>(T2{-0.75})),
                "conversion 3 (elastic_fixed_point)");
        static_assert(identical(T3{0.5}, static_cast<T3>(T2{0.75})),
                "conversion 3 (elastic_fixed_point)");
    }

    namespace elastic_fixed_point_multiply {
        static constexpr auto expected1 = T3{0.0};
        static constexpr T3 result1 = T2{0.5}*T2{0.5};
        static_assert(identical(expected1, result1), "test 1 multiply and round (elastic_fixed_point)");

        static constexpr auto expected2 = T3{0.0};
        static constexpr T3 result2 = T2{-0.5}*T2{0.5};
        static_assert(identical(expected2, result2), "test 2 multiply and round (elastic_fixed_point)");

        static constexpr auto expected3 = T3{-0.5};
        static constexpr T3 result3 = T2{-3.0}*T2{0.25};
        static_assert(identical(expected3, result3), "test 3 multiply and round (elastic_fixed_point)");

        static constexpr auto expected4 = T3{0.5};
        static constexpr T3 result4 = T2{3.0}*T2{0.25};
        static_assert(identical(expected4, result4), "test 4 multiply and round (elastic_fixed_point)");
    }

    namespace elastic_fixed_point_divide {
        static constexpr auto expected1 = T3{1.0};
        static constexpr T3 result1 = cnl::quotient(T2{0.5}, T2{0.5});
        static_assert(identical(expected1, result1), "test 1 divide and round (elastic_fixed_point)");

        static constexpr auto expected2 = T3{-1.0};
        static constexpr T3 result2 = cnl::quotient(T2{-0.5}, T2{0.5});
        static_assert(identical(expected2, result2), "test 2 divide and round (elastic_fixed_point)");

        static constexpr auto expected3 = T3{0.0};
        static constexpr T3 result3 = cnl::quotient(T2{0.5}, T2{2.0});
        static_assert(identical(expected3, result3), "test 3 divide and round (elastic_fixed_point)");

        static constexpr auto expected4 = T3{0.0};
        static constexpr T3 result4 = cnl::quotient(T2{0.5}, T2{1.5});
        static_assert(identical(expected4, result4), "test 4 divide and round (elastic_fixed_point)");

        static constexpr auto expected5 = T3{0.0};
        static constexpr T3 result5 = cnl::quotient(T2{-0.5}, T2{1.5});
        static_assert(identical(expected5, result5), "test 5 divide and round (elastic_fixed_point)");
    }
}
