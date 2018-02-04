
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/overflow_integer.h>

namespace cnl {
    // precise safe integer
    template<
            int IntegerDigits,
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int>
    using precise_safe_int = elastic_integer<
            IntegerDigits,
            rounding_integer<
                    overflow_integer<
                            Narrowest,
                            OverflowTag
                    >,
                    RoundingTag
            >
    >;

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            class Input = int,
            class = _impl::enable_if_t<!_impl::is_constant<Input>::value>>
    precise_safe_int<
            numeric_limits<Input>::digits,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_precise_safe_int(Input const& input)
    {
        return input;
    }

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE InputValue = 0>
    precise_safe_int<
            used_bits(InputValue),
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_precise_safe_int(constant<InputValue>)
    {
        return InputValue;
    }
}

namespace {
    using cnl::make_precise_safe_int;
    using cnl::precise_safe_int;
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        using cnl::rounding_integer;
        using cnl::overflow_integer;
        using cnl::elastic_integer;

        static_assert(
                is_same<precise_safe_int<1>::rep::rep::rep, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_multiply {
        static_assert(identical(cnl::precise_safe_int<6>{7}*cnl::precise_safe_int<13>{321}, cnl::precise_safe_int<19>{2247}), "");
    }

    namespace test_make_precise_safe_int {
        using namespace cnl::literals;
        static_assert(identical(make_precise_safe_int(cnl::int16{7}), precise_safe_int<15>{7}), "");
        static_assert(identical(make_precise_safe_int(7_c), precise_safe_int<3>{7}), "");
    }
}
