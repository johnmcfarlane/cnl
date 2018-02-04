
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/overflow_integer.h>

namespace cnl {
    // precise safe fixed-point
    template<
            int IntegerDigits,
            int FractionalDigits = 0,
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int>
    using psfp = fixed_point<
            elastic_integer<
                    IntegerDigits+FractionalDigits,
                    rounding_integer<
                            overflow_integer<
                                    Narrowest,
                                    OverflowTag
                            >,
                            RoundingTag
                    >
            >,
            -FractionalDigits
    >;

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            class Input = int>
    psfp<
            numeric_limits<Input>::digits, 0,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_psfp(Input const& input)
    {
        return input;
    }

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            class Input = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    psfp<
            used_bits(Value), -trailing_bits(Value),
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_psfp(constant<Value> const&)
    {
        return Value;
    }
}

namespace {
    using cnl::make_psfp;
    using cnl::psfp;
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        using cnl::rounding_integer;
        using cnl::overflow_integer;
        using cnl::elastic_integer;

        static_assert(
                is_same<psfp<1>::rep::rep::rep::rep, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_make_psfp {
        using namespace cnl::literals;
        static_assert(identical(make_psfp(cnl::int16{7}), psfp<15>{7}), "");
        static_assert(identical(make_psfp(444_c), psfp<9, -2>{444}), "");
    }

    namespace test_multiply {
        static_assert(identical(cnl::psfp<6>{7}*cnl::psfp<13>{321}, cnl::psfp<19>{2247}), "");
    }
}
