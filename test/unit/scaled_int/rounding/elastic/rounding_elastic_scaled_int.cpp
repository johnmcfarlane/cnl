
//          Copyright Heikki Berg 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/all.h>

#include <test.h>

namespace {

    template<
            int IntegerDigits, cnl::rounding_tag RoundingTag = cnl::nearest_rounding_tag, class Narrowest = int>
    using rounding_elastic_integer =
            cnl::rounding_integer<cnl::elastic_integer<IntegerDigits, Narrowest>, RoundingTag>;

    template<
            int Digits, int Exponent = 0, cnl::rounding_tag RoundingTag = cnl::nearest_rounding_tag,
            class Narrowest = signed>
    using rounding_elastic_scaled_integer = cnl::scaled_integer<
            rounding_elastic_integer<Digits, RoundingTag, Narrowest>, cnl::power<Exponent>>;

    namespace test_ctor {
        static_assert(
                identical(
                        rounding_elastic_scaled_integer<24, -20>{1.5},
                        rounding_elastic_scaled_integer<24, -20>{
                                rounding_elastic_scaled_integer<49, -40>{1.5}}));
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        rounding_elastic_scaled_integer<48, -40>{1.5},
                        rounding_elastic_scaled_integer<48, -40>{
                                rounding_elastic_scaled_integer<97, -80>{1.5}}));
#endif
    }

    namespace test_addition {
        static_assert(
                identical(
                        rounding_elastic_scaled_integer<25, -8>{2},
                        rounding_elastic_scaled_integer<16>{1}
                                + rounding_elastic_scaled_integer<16, -8>{1}),
                "rounding_elastic_scaled_integer addition");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        rounding_elastic_scaled_integer<63, -30>{2LL},
                        rounding_elastic_scaled_integer<32>{1}
                                + rounding_elastic_scaled_integer<32, -30>{1}),
                "larger rounding_elastic_scaled_integer addition");
#endif
    }

    namespace test_shift {
        constexpr auto a = rounding_elastic_scaled_integer<16, -8>{1};
        static_assert(
                identical(rounding_elastic_scaled_integer<16, -8>{2}, a << 1),
                "rounding_elastic_scaled_integer shift");
        static_assert(
                identical(rounding_elastic_scaled_integer<16, -8>{.5}, a >> 1),
                "rounding_elastic_scaled_integer shift");
        static_assert(
                identical(rounding_elastic_scaled_integer<16, -8>{1}, a << 0),
                "rounding_elastic_scaled_integer shift");
        static_assert(
                identical(rounding_elastic_scaled_integer<16, -8>{1}, a >> 0),
                "rounding_elastic_scaled_integer shift");
    }

    namespace test_division {

        static_assert(
                identical(
                        rounding_elastic_scaled_integer<16, -8>{.5},
                        cnl::quotient(
                                rounding_elastic_scaled_integer<8, 0>{1},
                                rounding_elastic_scaled_integer<8, 0>{2})),
                "cnl::quotient(rounding_elastic_scaled_integer, rounding_elastic_scaled_integer)");

        static_assert(
                identical(
                        rounding_elastic_scaled_integer<62, -31>{2.},
                        cnl::quotient(
                                rounding_elastic_scaled_integer<31, 0>{2},
                                rounding_elastic_scaled_integer<31, 0>{1})),
                "cnl::quotient(rounding_elastic_scaled_integer, rounding_elastic_scaled_integer)");

#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        rounding_elastic_scaled_integer<96, -48>{2.},
                        cnl::quotient(
                                rounding_elastic_scaled_integer<48, 0>{2},
                                rounding_elastic_scaled_integer<48, 0>{1})),
                "cnl::quotient(rounding_elastic_scaled_integer, rounding_elastic_scaled_integer)");
#endif

        static_assert(
                identical(
                        rounding_elastic_scaled_integer<16, -8>{.5},
                        cnl::quotient(
                                rounding_elastic_scaled_integer<8, -4>{1},
                                rounding_elastic_scaled_integer<8, -4>{2})),
                "cnl::quotient(rounding_elastic_scaled_integer, rounding_elastic_scaled_integer)");

        static_assert(
                identical(
                        rounding_elastic_scaled_integer<16, -8>{.5},
                        make_scaled_integer(cnl::make_fraction(
                                rounding_elastic_scaled_integer<8, 0>{1},
                                rounding_elastic_scaled_integer<8, 0>{2}))),
                "cnl::rounding_elastic_scaled_integer division");

        static_assert(
                identical(
                        rounding_elastic_scaled_integer<62, -31>{.5},
                        make_scaled_integer(cnl::make_fraction(
                                rounding_elastic_scaled_integer<31, 0>{1},
                                rounding_elastic_scaled_integer<31, 0>{2}))),
                "cnl::rounding_elastic_scaled_integer division");

#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        rounding_elastic_scaled_integer<96, -48>{.5},
                        make_scaled_integer(cnl::make_fraction(
                                rounding_elastic_scaled_integer<48, 0>{1},
                                rounding_elastic_scaled_integer<48, 0>{2}))),
                "cnl::rounding_elastic_scaled_integer division");

        static_assert(
                identical(
                        rounding_elastic_scaled_integer<124, -62>{.5},
                        make_scaled_integer(cnl::make_fraction(
                                rounding_elastic_scaled_integer<62, 0>{1},
                                rounding_elastic_scaled_integer<62, 0>{2}))),
                "cnl::rounding_elastic_scaled_integer division");
#endif
    }

    namespace test_static_cast {
#if defined(CNL_INT128_ENABLED)
        constexpr auto c = static_cast<rounding_elastic_scaled_integer<24, -20>>(
                rounding_elastic_scaled_integer<48, -40>{0.21875});
        static_assert(
                identical(rounding_elastic_scaled_integer<24, -20>{0.21875}, c),
                "rounding_elastic_scaled_integer assignment");
#endif
    }
}
