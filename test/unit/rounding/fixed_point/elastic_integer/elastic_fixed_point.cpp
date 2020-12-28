
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_fixed_point.h>
#include <cnl/rounding.h>

using cnl::_impl::identical;

namespace {
    namespace test_native_round_down {
        static constexpr auto expected = cnl::elastic_fixed_point<4, -1>{0.0};
        static constexpr auto actual = cnl::convert<
                cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<4, -1>>(
                cnl::elastic_fixed_point<8, -4>{0.3125});

        static_assert(
                identical(expected, actual), "cnl::convert<cnl::native_rounding_tag, cnl::power<>, "
                                             "cnl::elastic_fixed_point, cnl::elastic_fixed_point>");
    }

    namespace test_native_round_up {
        static constexpr auto expected = cnl::elastic_fixed_point<4, -2>{0.25};
        static constexpr auto actual = cnl::convert<
                cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<4, -2>>(
                cnl::elastic_fixed_point<8, -4>{0.3125});

        static_assert(
                identical(expected, actual), "cnl::convert<cnl::native_rounding_tag, cnl::power<>, "
                                             "cnl::elastic_fixed_point, cnl::elastic_fixed_point>");
    }

    namespace elastic_fixed_point_native_rounding {

        // Positive
        static_assert(
                identical(
                        cnl::convert<
                                cnl::native_rounding_tag, cnl::_impl::native_tag,
                                cnl::elastic_fixed_point<16, 0>>(cnl::elastic_fixed_point<16, -4>{
                                5.25}),
                        cnl::elastic_fixed_point<16, 0>{5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{5.25}), 84));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{5.0}), 5));

        static_assert(
                identical(
                        cnl::convert<
                                cnl::native_rounding_tag, cnl::_impl::native_tag,
                                cnl::elastic_fixed_point<16, 0>>(cnl::elastic_fixed_point<16, -4>{
                                5.5}),
                        cnl::elastic_fixed_point<16, 0>{5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{5.5}), 88));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{5.0}), 5));

        static_assert(
                identical(
                        cnl::convert<
                                cnl::native_rounding_tag, cnl::_impl::native_tag,
                                cnl::elastic_fixed_point<16, -1>>(cnl::elastic_fixed_point<16, -4>{
                                5.25}),
                        cnl::elastic_fixed_point<16, -1>{5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{5.25}), 84));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -1>{5.0}), 10));

        // Negative
        static_assert(
                identical(
                        cnl::convert<
                                cnl::native_rounding_tag, cnl::_impl::native_tag,
                                cnl::elastic_fixed_point<16, 0>>(cnl::elastic_fixed_point<16, -4>{
                                -5.25}),
                        cnl::elastic_fixed_point<16, 0>{-5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.25}), -84));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{-5.0}), -5));

        static_assert(
                identical(
                        cnl::convert<
                                cnl::native_rounding_tag, cnl::_impl::native_tag,
                                cnl::elastic_fixed_point<16, 0>>(cnl::elastic_fixed_point<16, -4>{
                                -5.5}),
                        cnl::elastic_fixed_point<16, 0>{-5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.5}), -88));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, 0>{-5.0}), -5));

        static_assert(
                identical(
                        cnl::convert<
                                cnl::native_rounding_tag, cnl::_impl::native_tag,
                                cnl::elastic_fixed_point<16, -1>>(cnl::elastic_fixed_point<16, -4>{
                                -5.25}),
                        cnl::elastic_fixed_point<16, -1>{-5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -4>{-5.25}), -84));
        static_assert(identical(cnl::unwrap(cnl::elastic_fixed_point<16, -1>{-5.0}), -10));
    }

    namespace elastic_fixed_point_implicit_conversions {
        using q4_4 = cnl::elastic_fixed_point<8, -4>;
        using q4_1 = cnl::elastic_fixed_point<5, -1>;

        static_assert(
                identical(q4_1{0.0}, static_cast<q4_1>(q4_4{0.25})),
                "conversion 1 (elastic_fixed_point)");
        static_assert(
                identical(q4_1{0.0}, static_cast<q4_1>(q4_4{-0.25})),
                "conversion 2 (elastic_fixed_point)");
        static_assert(
                identical(q4_1{-0.5}, static_cast<q4_1>(q4_4{-0.75})),
                "conversion 3 (elastic_fixed_point)");
        static_assert(
                identical(q4_1{0.5}, static_cast<q4_1>(q4_4{0.75})),
                "conversion 4 (elastic_fixed_point)");
    }

    namespace elastic_fixed_point_multiply {
        using q4_20 = cnl::elastic_fixed_point<24, -20>;
        using q4_4 = cnl::elastic_fixed_point<8, -4>;
        using q4_1 = cnl::elastic_fixed_point<5, -1>;

        static constexpr auto expected1 = q4_1{0.0};
        static constexpr q4_1 result1 = q4_4{0.5} * q4_4{0.5};
        static_assert(
                identical(expected1, result1), "test 1 multiply and round (elastic_fixed_point)");

        static constexpr auto expected2 = q4_1{0.0};
        static constexpr q4_1 result2 = q4_4{-0.5} * q4_4{0.5};
        static_assert(
                identical(expected2, result2), "test 2 multiply and round (elastic_fixed_point)");

        static constexpr auto expected3 = q4_1{-0.5};
        static constexpr q4_1 result3 = q4_4{-3.0} * q4_4{0.25};
        static_assert(
                identical(expected3, result3), "test 3 multiply and round (elastic_fixed_point)");

        static constexpr auto expected4 = q4_1{0.5};
        static constexpr q4_1 result4 = q4_4{3.0} * q4_4{0.25};
        static_assert(
                identical(expected4, result4), "test 4 multiply and round (elastic_fixed_point)");

        static constexpr auto expected5 = q4_20{1.0};
        static constexpr q4_20 result5 = q4_20{2.0} * q4_20{0.5};
        static_assert(
                identical(expected5, result5), "test 5 multiply and round (elastic_fixed_point)");
    }

    namespace elastic_fixed_point_divide {
        using q4_4 = cnl::elastic_fixed_point<8, -4>;
        using q4_1 = cnl::elastic_fixed_point<5, -1>;

        static constexpr auto expected1 = q4_1{1.0};
        static constexpr q4_1 result1 = cnl::quotient(q4_4{0.5}, q4_4{0.5});
        static_assert(
                identical(expected1, result1), "test 1 divide and round (elastic_fixed_point)");

        static constexpr auto expected2 = q4_1{-1.0};
        static constexpr q4_1 result2 = cnl::quotient(q4_4{-0.5}, q4_4{0.5});
        static_assert(
                identical(expected2, result2), "test 2 divide and round (elastic_fixed_point)");

        static constexpr auto expected3 = q4_1{0.0};
        static constexpr q4_1 result3 = cnl::quotient(q4_4{0.5}, q4_4{2.0});
        static_assert(
                identical(expected3, result3), "test 3 divide and round (elastic_fixed_point)");

        static constexpr auto expected4 = q4_1{0.0};
        static constexpr q4_1 result4 = cnl::quotient(q4_4{0.5}, q4_4{1.5});
        static_assert(
                identical(expected4, result4), "test 4 divide and round (elastic_fixed_point)");

        static constexpr auto expected5 = q4_1{0.0};
        static constexpr q4_1 result5 = cnl::quotient(q4_4{-0.5}, q4_4{1.5});
        static_assert(
                identical(expected5, result5), "test 5 divide and round (elastic_fixed_point)");
    }
}
