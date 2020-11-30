
//          Copyright Heikki Berg 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/all.h>
#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace elastic_fixed_point_tie_to_pos_inf_rounding {

        // Positive
        static_assert(identical(
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, -1>>
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
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, 0>>
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
                cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::elastic_fixed_point<16, -1>>
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

    template<
            int IntegerDigits,
            class RoundingTag = cnl::_impl::tag_t<cnl::rounding_integer<>>,
            class Narrowest = int>
    using rounding_elastic_integer = cnl::rounding_integer<
            cnl::elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            RoundingTag>;

    template<
            class RoundingTag = cnl::_impl::tag_t<cnl::rounding_integer<>>,
            class Narrowest = int,
            class Input = int>
    CNL_NODISCARD rounding_elastic_integer<
            cnl::numeric_limits<Input>::digits,
            RoundingTag,
            Narrowest>
    constexpr make_rounding_elastic(Input const& input)
    {
        return input;
    }

    template<int Digits, int Exponent = 0, class RoundingTag = cnl::tie_to_pos_inf_rounding_tag, class Narrowest = signed>
    using elastic_fixed_point_tie_to_pos_inf = cnl::fixed_point<rounding_elastic_integer<Digits, RoundingTag, Narrowest>, Exponent>;


    namespace elastic_fixed_point_tie_to_pos_inf_implicit_conversions {
        using q4_4 = elastic_fixed_point_tie_to_pos_inf<8, -4>;
        using q4_1 = elastic_fixed_point_tie_to_pos_inf<5, -1>;

        static_assert(identical(q4_1{0.5}, static_cast<q4_1>(q4_4{0.25})),
                "conversion 1 (elastic_fixed_point_tie_to_pos_inf)");
        static_assert(identical(q4_1{0.0}, static_cast<q4_1>(q4_4{-0.25})),
                "conversion 2 (elastic_fixed_point_tie_to_pos_inf)");
        static_assert(identical(q4_1{-0.5}, static_cast<q4_1>(q4_4{-0.75})),
                "conversion 3 (elastic_fixed_point_tie_to_pos_inf)");
        static_assert(identical(q4_1{1.0}, static_cast<q4_1>(q4_4{0.75})),
                "conversion 4 (elastic_fixed_point_tie_to_pos_inf)");
    }

    namespace elastic_fixed_point_tie_to_pos_inf_multiply {
        using q4_20 = elastic_fixed_point_tie_to_pos_inf<24, -20>;
        using q4_4 = elastic_fixed_point_tie_to_pos_inf<8, -4>;
        using q4_1 = elastic_fixed_point_tie_to_pos_inf<5, -1>;

        static constexpr auto expected1 = q4_1{0.5};
        static constexpr q4_1 result1 = q4_4{0.5}*q4_4{0.5};
        static_assert(identical(expected1, result1), "test 1 multiply and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected2 = q4_1{0.0};
        static constexpr q4_1 result2 = q4_4{-0.5}*q4_4{0.5};
        static_assert(identical(expected2, result2), "test 2 multiply and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected3 = q4_1{-0.5};
        static constexpr q4_1 result3 = q4_4{-3.0}*q4_4{0.25};
        static_assert(identical(expected3, result3), "test 3 multiply and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected4 = q4_1{1.0};
        static constexpr q4_1 result4 = q4_4{3.0}*q4_4{0.25};
        static_assert(identical(expected4, result4), "test 4 multiply and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected5 = q4_20{1.0};
        static constexpr q4_20 result5 = q4_20{2.0}*q4_20{0.5};
        static_assert(identical(expected5, result5), "test 5 multiply and round (elastic_fixed_point_tie_to_pos_inf)");
    }

    namespace elastic_fixed_point_tie_to_pos_inf_divide {
        using q4_4 = elastic_fixed_point_tie_to_pos_inf<8, -4>;
        using q4_1 = elastic_fixed_point_tie_to_pos_inf<5, -1>;

        static constexpr auto expected1 = q4_1{1.0};
        static constexpr q4_1 result1 = cnl::quotient(q4_4{0.5}, q4_4{0.5});
        static_assert(identical(expected1, result1), "test 1 divide and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected2 = q4_1{-1.0};
        static constexpr q4_1 result2 = cnl::quotient(q4_4{-0.5}, q4_4{0.5});
        static_assert(identical(expected2, result2), "test 2 divide and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected3 = q4_1{0.5};
        static constexpr q4_1 result3 = cnl::quotient(q4_4{0.5}, q4_4{2.0});
        static_assert(identical(expected3, result3), "test 3 divide and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected4 = q4_1{0.5};
        static constexpr q4_1 result4 = cnl::quotient(q4_4{0.5}, q4_4{1.5});
        static_assert(identical(expected4, result4), "test 4 divide and round (elastic_fixed_point_tie_to_pos_inf)");

        static constexpr auto expected5 = q4_1{-0.5};
        static constexpr q4_1 result5 = cnl::quotient(q4_4{-0.5}, q4_4{1.5});
        static_assert(identical(expected5, result5), "test 5 divide and round (elastic_fixed_point_tie_to_pos_inf)");
    }
}
