#include <cnl.h>

namespace {

    template<
            int IntegerDigits,
            class RoundingTag = cnl::nearest_rounding_tag,
            class Narrowest = int>
    using rounding_elastic_integer = cnl::rounding_integer<
            cnl::elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            RoundingTag>;

    template<int Digits, int Exponent = 0, class RoundingTag = cnl::nearest_rounding_tag, class Narrowest = signed>
    using rounding_elastic_number = cnl::fixed_point<rounding_elastic_integer<Digits, RoundingTag, Narrowest>, Exponent>;

    namespace test_addition {
        static constexpr auto lhs = rounding_elastic_number<16>{1};
        static constexpr auto rhs = rounding_elastic_number<16, -8>{1};
        static constexpr auto expected = rounding_elastic_number<25, -8>{2};
        static constexpr auto sum = lhs + rhs;
        static_assert(identical(expected, sum), "rounding_elastic_number addition");
    }

    namespace test_addition_larger {
        static constexpr auto lhs = rounding_elastic_number<32>{1};
        static constexpr auto rhs = rounding_elastic_number<32, -30>{1};
        static constexpr auto expected = rounding_elastic_number<63, -30>{2LL};
        static constexpr auto sum = lhs + rhs;
        static_assert(identical(expected, sum), "larger rounding_elastic_number addition");
    }

    namespace test_division {

        static_assert(identical(
                rounding_elastic_number<16, -8>{.5},
                cnl::quotient(rounding_elastic_number<8, 0>{1}, rounding_elastic_number<8, 0>{2})),
                      "cnl::divide(rounding_elastic_number, rounding_elastic_number)");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                rounding_elastic_number<62, -31>{2.},
                cnl::quotient(rounding_elastic_number<31, 0>{2}, rounding_elastic_number<31, 0>{1})),
                      "cnl::divide(rounding_elastic_number, rounding_elastic_number)");
#endif

#if defined(CNL_INT256_ENABLED)
        static_assert(identical(
    rounding_elastic_number<96, -48>{2.},
    cnl::quotient(rounding_elastic_number<48, 0>{2}, rounding_elastic_number<48, 0>{1})),
              "cnl::divide(rounding_elastic_number, rounding_elastic_number)");
#endif

        static_assert(identical(
                rounding_elastic_number<16, -8>{.5},
                cnl::quotient(rounding_elastic_number<8, -4>{1}, rounding_elastic_number<8, -4>{2})),
                      "cnl::divide(rounding_elastic_number, rounding_elastic_number)");

        static_assert(identical(
                rounding_elastic_number<16, -8>{.5},
                make_fixed_point(cnl::make_fractional(
                        rounding_elastic_number<8, 0>{1},
                        rounding_elastic_number<8, 0>{2}))), "cnl::rounding_elastic_number division");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                rounding_elastic_number<62, -31>{.5},
                make_fixed_point(cnl::make_fractional(
                        rounding_elastic_number<31, 0>{1},
                        rounding_elastic_number<31, 0>{2}))), "cnl::rounding_elastic_number division");
#endif

#if defined(CNL_INT256_ENABLED)
        static_assert(identical(
                rounding_elastic_number<96, -48>{.5},
                make_fixed_point(cnl::make_fractional(
                        rounding_elastic_number<48, 0>{1},
                        rounding_elastic_number<48, 0>{2}))), "cnl::rounding_elastic_number division");

        static_assert(identical(
                rounding_elastic_number<124, -62>{.5},
                make_fixed_point(cnl::make_fractional(
                        rounding_elastic_number<62, 0>{1},
                        rounding_elastic_number<62, 0>{2}))), "cnl::rounding_elastic_number division");
#endif
    }
}