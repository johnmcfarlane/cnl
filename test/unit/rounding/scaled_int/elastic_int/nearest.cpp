
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_scaled_integer.h>
#include <cnl/rounding.h>
#include <cnl/rounding_integer.h>
#include <test.h>

#include <limits>

namespace {
    namespace test_nearest_round_down {
        constexpr auto expected = cnl::elastic_scaled_integer<4, cnl::power<-1>>{0.5};
        constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, cnl::power<-1>>, cnl::power<>>{}(
                cnl::elastic_scaled_integer<8, cnl::power<-4>>{0.3125});

        static_assert(identical(expected, actual));
    }

    namespace test_nearest_round_up {
        constexpr auto expected = cnl::elastic_scaled_integer<4, cnl::power<-2>>{0.25};
        constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, cnl::power<-2>>, cnl::power<>>{}(
                cnl::elastic_scaled_integer<8, cnl::power<-4>>{0.3125});

        static_assert(identical(expected, actual));
    }

    namespace elastic_scaled_integer_nearest_rounding {

        // Positive
        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<16, cnl::power<0>>>{}(
                        cnl::elastic_scaled_integer<16, cnl::power<-4>>{5.25}),
                cnl::elastic_scaled_integer<16, cnl::power<0>>{5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-4>>{5.25}), 84));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<0>>{5.0}), 5));

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<16, cnl::power<0>>>{}(
                        cnl::elastic_scaled_integer<16, cnl::power<-4>>{5.5}),
                cnl::elastic_scaled_integer<16, cnl::power<0>>{6.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-4>>{5.5}), 88));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<0>>{6.0}), 6));

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<16, cnl::power<-1>>>{}(
                        cnl::elastic_scaled_integer<16, cnl::power<-4>>{5.25}),
                cnl::elastic_scaled_integer<16, cnl::power<-1>>{5.5}));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-4>>{5.25}), 84));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-1>>{5.5}), 11));

        // Negative
        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<16, cnl::power<0>>>{}(
                        cnl::elastic_scaled_integer<16, cnl::power<-4>>{-5.25}),
                cnl::elastic_scaled_integer<16, cnl::power<0>>{-5.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-4>>{-5.25}), -84));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<0>>{-5.0}), -5));

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<16, cnl::power<0>>>{}(
                        cnl::elastic_scaled_integer<16, cnl::power<-4>>{-5.5}),
                cnl::elastic_scaled_integer<16, cnl::power<0>>{-6.0}));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-4>>{-5.5}), -88));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<0>>{-6.0}), -6));

        static_assert(identical(
                cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<16, cnl::power<-1>>>{}(
                        cnl::elastic_scaled_integer<16, cnl::power<-4>>{-5.25}),
                cnl::elastic_scaled_integer<16, cnl::power<-1>>{-5.5}));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-4>>{-5.25}), -84));
        static_assert(identical(cnl::unwrap(cnl::elastic_scaled_integer<16, cnl::power<-1>>{-5.5}), -11));
    }

    template<
            int IntegerDigits, cnl::rounding_tag RoundingTag = cnl::_impl::tag_of_t<cnl::rounding_integer<>>,
            class Narrowest = int>
    using rounding_elastic_integer =
            cnl::rounding_integer<cnl::elastic_integer<IntegerDigits, Narrowest>, RoundingTag>;

    template<
            cnl::rounding_tag RoundingTag = cnl::_impl::tag_of_t<cnl::rounding_integer<>>, class Narrowest = int,
            class Input = int>
    [[nodiscard]] constexpr auto make_rounding_elastic(Input const& input)
    {
        return rounding_elastic_integer<
                std::numeric_limits<Input>::digits, RoundingTag,
                Narrowest>{input};
    }

    template<
            int Digits, int Exponent = 0, cnl::rounding_tag RoundingTag = cnl::nearest_rounding_tag,
            class Narrowest = signed>
    using elastic_scaled_integer_nearest =
            cnl::scaled_integer<rounding_elastic_integer<Digits, RoundingTag, Narrowest>, cnl::power<Exponent>>;

    namespace elastic_scaled_integer_nearest_implicit_conversions {
        using q4_4 = elastic_scaled_integer_nearest<8, -4>;
        using q4_1 = elastic_scaled_integer_nearest<5, -1>;

        static_assert(
                identical(q4_1{0.5}, static_cast<q4_1>(q4_4{0.25})),
                "conversion 1 (elastic_scaled_integer_nearest)");
        static_assert(
                identical(q4_1{-0.5}, static_cast<q4_1>(q4_4{-0.25})),
                "conversion 2 (elastic_scaled_integer_nearest)");
        static_assert(
                identical(q4_1{-1.0}, static_cast<q4_1>(q4_4{-0.75})),
                "conversion 3 (elastic_scaled_integer_nearest)");
        static_assert(
                identical(q4_1{1.0}, static_cast<q4_1>(q4_4{0.75})),
                "conversion 4 (elastic_scaled_integer_nearest)");
    }

    namespace elastic_scaled_integer_nearest_multiply {
        using q4_20 = elastic_scaled_integer_nearest<24, -20>;
        using q4_4 = elastic_scaled_integer_nearest<8, -4>;
        using q4_1 = elastic_scaled_integer_nearest<5, -1>;

        constexpr auto expected1 = q4_1{0.5};
        constexpr q4_1 result1 = q4_4{0.5} * q4_4{0.5};
        static_assert(
                identical(expected1, result1),
                "test 1 multiply and round (elastic_scaled_integer_nearest)");

        constexpr auto expected2 = q4_1{-0.5};
        constexpr q4_1 result2 = q4_4{-0.5} * q4_4{0.5};
        static_assert(
                identical(expected2, result2),
                "test 2 multiply and round (elastic_scaled_integer_nearest)");

        constexpr auto expected3 = q4_1{-1.0};
        constexpr q4_1 result3 = q4_4{-3.0} * q4_4{0.25};
        static_assert(
                identical(expected3, result3),
                "test 3 multiply and round (elastic_scaled_integer_nearest)");

        constexpr auto expected4 = q4_1{1.0};
        constexpr q4_1 result4 = q4_4{3.0} * q4_4{0.25};
        static_assert(
                identical(expected4, result4),
                "test 4 multiply and round (elastic_scaled_integer_nearest)");

        constexpr auto expected5 = q4_20{1.0};
        constexpr q4_20 result5 = q4_20{2.0} * q4_20{0.5};
        static_assert(
                identical(expected5, result5),
                "test 5 multiply and round (elastic_scaled_integer_nearest)");
    }

    namespace elastic_scaled_integer_nearest_divide {
        using q4_4 = elastic_scaled_integer_nearest<8, -4>;
        using q4_1 = elastic_scaled_integer_nearest<5, -1>;

        constexpr auto expected1 = q4_1{1.0};
        constexpr q4_1 result1 = cnl::quotient(q4_4{0.5}, q4_4{0.5});
        static_assert(
                identical(expected1, result1),
                "test 1 divide and round (elastic_scaled_integer_nearest)");

        constexpr auto expected2 = q4_1{-1.0};
        constexpr q4_1 result2 = cnl::quotient(q4_4{-0.5}, q4_4{0.5});
        static_assert(
                identical(expected2, result2),
                "test 2 divide and round (elastic_scaled_integer_nearest)");

        constexpr auto expected3 = q4_1{0.5};
        constexpr q4_1 result3 = cnl::quotient(q4_4{0.5}, q4_4{2.0});
        static_assert(
                identical(expected3, result3),
                "test 3 divide and round (elastic_scaled_integer_nearest)");

        constexpr auto expected4 = q4_1{0.5};
        constexpr q4_1 result4 = cnl::quotient(q4_4{0.5}, q4_4{1.5});
        static_assert(
                identical(expected4, result4),
                "test 4 divide and round (elastic_scaled_integer_nearest)");

        constexpr auto expected5 = q4_1{-0.5};
        constexpr q4_1 result5 = cnl::quotient(q4_4{-0.5}, q4_4{1.5});
        static_assert(
                identical(expected5, result5),
                "test 5 divide and round (elastic_scaled_integer_nearest)");
    }
}
