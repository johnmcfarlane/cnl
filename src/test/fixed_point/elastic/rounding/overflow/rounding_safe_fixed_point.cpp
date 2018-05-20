
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/overflow_integer.h>

namespace cnl {
    // rounding safe fixed-point
    template<
            int IntegerDigits,
            int FractionalDigits = 0,
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int>
    using rsfp = fixed_point<
            rounding_integer<
                    overflow_integer<
                            elastic_integer<
                                    IntegerDigits+FractionalDigits,
                                    Narrowest
                            >,
                            OverflowTag
                    >,
                    RoundingTag
            >,
            -FractionalDigits
    >;

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            class Input = int>
    rsfp<
            numeric_limits<Input>::digits, 0,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_rsfp(Input const& input)
    {
        return input;
    }

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            class Input = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    rsfp<
            used_digits(Value), -trailing_bits(Value),
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_rsfp(constant<Value> const&)
    {
        return Value;
    }
}

namespace {
    using cnl::_impl::identical;

    namespace default_parameters {
        static_assert(
                std::is_same<cnl::rsfp<1>::rep::rep::rep::rep, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_make_rsfp {
        using namespace cnl::literals;
        static_assert(identical(cnl::make_rsfp(cnl::int16{7}), cnl::rsfp<15>{7}), "");
        static_assert(identical(cnl::make_rsfp(444_c), cnl::rsfp<9, -2>{444}), "");
    }

    namespace test_multiply {
        static_assert(identical(cnl::rsfp<6>{7}*cnl::rsfp<13>{321}, cnl::rsfp<19>{2247}), "");
    }
}
