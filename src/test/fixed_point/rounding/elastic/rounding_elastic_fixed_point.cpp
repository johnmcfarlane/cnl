
//          Copyright Heikki Berg 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/all.h>

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
    using rounding_elastic_fixed_point = cnl::fixed_point<rounding_elastic_integer<Digits, RoundingTag, Narrowest>, Exponent>;

    namespace test_ctor {
        static_assert(
                identical(
                        rounding_elastic_fixed_point<24, -20>{1.5},
                        rounding_elastic_fixed_point<24, -20>{rounding_elastic_fixed_point<49, -40>{1.5}}),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        rounding_elastic_fixed_point<48, -40>{1.5},
                        rounding_elastic_fixed_point<48, -40>{rounding_elastic_fixed_point<97, -80>{1.5}}),
                "");
#endif
    }

    namespace test_addition {
        static_assert(
                identical(
                        rounding_elastic_fixed_point<25, -8>{2},
                        rounding_elastic_fixed_point<16>{1}+rounding_elastic_fixed_point<16, -8>{1}),
                "rounding_elastic_fixed_point addition");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        rounding_elastic_fixed_point<63, -30>{2LL},
                        rounding_elastic_fixed_point<32>{1}+rounding_elastic_fixed_point<32, -30>{1}),
                "larger rounding_elastic_fixed_point addition");
#endif
    }

    namespace test_shift {
        static constexpr auto a = rounding_elastic_fixed_point<16, -8>{1};
        static_assert(identical(rounding_elastic_fixed_point<16, -8>{2}, a << 1), "rounding_elastic_fixed_point shift");
        static_assert(identical(rounding_elastic_fixed_point<16, -8>{.5}, a >> 1), "rounding_elastic_fixed_point shift");
        static_assert(identical(rounding_elastic_fixed_point<16, -8>{1}, a << 0), "rounding_elastic_fixed_point shift");
        static_assert(identical(rounding_elastic_fixed_point<16, -8>{1}, a >> 0), "rounding_elastic_fixed_point shift");
    }

    namespace test_division {

        static_assert(identical(
                rounding_elastic_fixed_point<16, -8>{.5},
                cnl::quotient(rounding_elastic_fixed_point<8, 0>{1}, rounding_elastic_fixed_point<8, 0>{2})),
                      "cnl::quotient(rounding_elastic_fixed_point, rounding_elastic_fixed_point)");

        static_assert(identical(
                rounding_elastic_fixed_point<62, -31>{2.},
                cnl::quotient(rounding_elastic_fixed_point<31, 0>{2}, rounding_elastic_fixed_point<31, 0>{1})),
                      "cnl::quotient(rounding_elastic_fixed_point, rounding_elastic_fixed_point)");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                rounding_elastic_fixed_point<96, -48>{2.},
                cnl::quotient(rounding_elastic_fixed_point<48, 0>{2}, rounding_elastic_fixed_point<48, 0>{1})),
                          "cnl::quotient(rounding_elastic_fixed_point, rounding_elastic_fixed_point)");
#endif

        static_assert(identical(
                rounding_elastic_fixed_point<16, -8>{.5},
                cnl::quotient(rounding_elastic_fixed_point<8, -4>{1}, rounding_elastic_fixed_point<8, -4>{2})),
                      "cnl::quotient(rounding_elastic_fixed_point, rounding_elastic_fixed_point)");

        static_assert(identical(
                rounding_elastic_fixed_point<16, -8>{.5},
                make_fixed_point(cnl::make_fraction(
                        rounding_elastic_fixed_point<8, 0>{1},
                        rounding_elastic_fixed_point<8, 0>{2}))), "cnl::rounding_elastic_fixed_point division");

        static_assert(identical(
                rounding_elastic_fixed_point<62, -31>{.5},
                make_fixed_point(cnl::make_fraction(
                        rounding_elastic_fixed_point<31, 0>{1},
                        rounding_elastic_fixed_point<31, 0>{2}))), "cnl::rounding_elastic_fixed_point division");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                rounding_elastic_fixed_point<96, -48>{.5},
                make_fixed_point(cnl::make_fraction(
                        rounding_elastic_fixed_point<48, 0>{1},
                        rounding_elastic_fixed_point<48, 0>{2}))), "cnl::rounding_elastic_fixed_point division");

        static_assert(identical(
                rounding_elastic_fixed_point<124, -62>{.5},
                make_fixed_point(cnl::make_fraction(
                        rounding_elastic_fixed_point<62, 0>{1},
                        rounding_elastic_fixed_point<62, 0>{2}))), "cnl::rounding_elastic_fixed_point division");
#endif
    }

    namespace test_static_cast {
#if defined(CNL_INT128_ENABLED)
        static constexpr auto c = static_cast<rounding_elastic_fixed_point<24, -20>>(
                rounding_elastic_fixed_point<48, -40>{0.21875});
        static_assert(identical(rounding_elastic_fixed_point<24, -20>{0.21875}, c), "rounding_elastic_fixed_point assignment");
#endif
    }
}
