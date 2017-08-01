
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/elastic_integer.h>
#include <cnl/precise_integer.h>
#include <cnl/safe_integer.h>

namespace cnl {
    // precise safe elastic fixed-point
    template<
            int IntegerDigits,
            int FractionalDigits = 0,
            class OverflowTag = safe_integer<>::overflow_tag,
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int>
    using psefp = fixed_point<
            elastic_integer<
                    IntegerDigits+FractionalDigits,
                    precise_integer<
                            safe_integer<
                                    Narrowest,
                                    OverflowTag
                            >,
                            RoundingTag
                    >
            >,
            -FractionalDigits
    >;

    template<
            class OverflowTag = safe_integer<>::overflow_tag,
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int,
            class Input = int>
    psefp<
            std::numeric_limits<Input>::digits, 0,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_psefp(Input const& input)
    {
        return input;
    }

    template<
            class OverflowTag = safe_integer<>::overflow_tag,
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int,
            class Input = int,
            class Integral,
            Integral Value,
            int Digits,
            int Exponent>
    psefp<
            Digits, -Exponent,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_psefp(const_integer<Integral, Value, Digits, Exponent> const&)
    {
        return Value;
    }
}

namespace {
    using cnl::make_psefp;
    using cnl::psefp;
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        using cnl::precise_integer;
        using cnl::safe_integer;
        using cnl::elastic_integer;

        static_assert(
                is_same<psefp<1>::rep::rep::rep::rep, int>::value,
                "cnl::precise_integer parameter default test failed");
    }

    namespace test_make_psefp {
        using namespace cnl::literals;
        static_assert(identical(make_psefp(std::int16_t{7}), psefp<15>{7}), "");
        static_assert(identical(make_psefp(444_c), psefp<9, -2>{444}), "");
    }

    namespace test_multiply {
        static_assert(identical(cnl::psefp<6>{7}*cnl::psefp<13>{321}, cnl::psefp<19>{2247}), "");
    }
}
