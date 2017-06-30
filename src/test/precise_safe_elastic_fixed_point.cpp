
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/fixed_point>
#include <sg14/auxiliary/elastic_integer.h>
#include <sg14/auxiliary/precise_integer.h>
#include <sg14/auxiliary/safe_integer.h>

namespace sg14 {
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
    constexpr make_psefp(const const_integer<Integral, Value, Digits, Exponent>&)
    {
        return Value;
    }
}

namespace {
    using sg14::make_psefp;
    using sg14::psefp;
    using std::is_same;
    using sg14::_impl::identical;

    namespace default_parameters {
        using sg14::precise_integer;
        using sg14::safe_integer;
        using sg14::elastic_integer;

        static_assert(
                is_same<psefp<1>::rep::rep::rep::rep, int>::value,
                "sg14::precise_integer parameter default test failed");
    }

    namespace test_make_psefp {
        using namespace sg14::literals;
        static_assert(identical(make_psefp(std::int16_t{7}), psefp<15>{7}), "");
        static_assert(identical(make_psefp(444_c), psefp<9, -2>{444}), "");
    }

    namespace test_multiply {
        static_assert(identical(sg14::psefp<6>{7}*sg14::psefp<13>{321}, sg14::psefp<19>{2247}), "");
    }
}
