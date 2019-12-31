
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/number/tag.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_integer.h>
#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

namespace cnl {
    // rounding safe integer
    template<
            int IntegerDigits,
            class OverflowTag = _impl::tag_t<overflow_integer<>>,
            class RoundingTag = _impl::tag_t<rounding_integer<>>,
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
            class OverflowTag = _impl::tag_t<overflow_integer<>>,
            class RoundingTag = _impl::tag_t<rounding_integer<>>,
            class Narrowest = int,
            class Input = int,
            class = _impl::enable_if_t<!_impl::is_constant<Input>::value>>
    CNL_NODISCARD rounding_safe_int<
            numeric_limits<Input>::digits,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_rounding_safe_int(Input const& input)
    {
        return input;
    }

    template<
            class OverflowTag = _impl::tag_t<overflow_integer<>>,
            class RoundingTag = _impl::tag_t<rounding_integer<>>,
            class Narrowest = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE InputValue = 0>
    CNL_NODISCARD rounding_safe_int<
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
    using cnl::_impl::rep_t;

    namespace default_parameters {
        using cnl::_impl::rep_t;
        static_assert(
                std::is_same<rep_t<rep_t<rep_t<cnl::rounding_safe_int<1>>>>, int>::value,
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
