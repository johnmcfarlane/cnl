
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/overflow_integer.h>

namespace cnl {
    // rounding safe integer
    template<
            int IntegerDigits,
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int>
    using rounding_safe_int = elastic_integer<
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
    rounding_safe_int<
            numeric_limits<Input>::digits,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_rounding_safe_int(Input const& input)
    {
        return input;
    }

    template<
            class OverflowTag = overflow_integer<>::overflow_tag,
            class RoundingTag = rounding_integer<>::rounding,
            class Narrowest = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE InputValue = 0>
    rounding_safe_int<
            _impl::used_digits(InputValue),
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_rounding_safe_int(constant<InputValue>)
    {
        return InputValue;
    }
}

namespace {
    using cnl::_impl::identical;

    namespace default_parameters {
        static_assert(
                std::is_same<cnl::rounding_safe_int<1>::rep::rep::rep, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_multiply {
        static_assert(identical(cnl::rounding_safe_int<6>{7}*cnl::rounding_safe_int<13>{321},
                cnl::rounding_safe_int<19>{2247}), "");
    }

    namespace test_make_rounding_safe_int {
        using namespace cnl::literals;
        static_assert(identical(cnl::make_rounding_safe_int(cnl::int16{7}), cnl::rounding_safe_int<15>{7}), "");
        static_assert(identical(cnl::make_rounding_safe_int(7_c), cnl::rounding_safe_int<3>{7}), "");
    }
}
