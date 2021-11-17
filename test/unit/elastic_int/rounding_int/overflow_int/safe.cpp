
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/wrapper/tag_of.h>
#include <cnl/elastic_integer.h>
#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

namespace cnl {
    // rounding safe integer
    template<
            int IntegerDigits, overflow_tag OverflowTag = _impl::tag_of_t<overflow_integer<>>,
            rounding_tag RoundingTag = _impl::tag_of_t<rounding_integer<>>, class Narrowest = int>
    using rounding_safe_int = elastic_integer<
            IntegerDigits, rounding_integer<overflow_integer<Narrowest, OverflowTag>, RoundingTag>>;

    template<
            overflow_tag OverflowTag = _impl::tag_of_t<overflow_integer<>>,
            rounding_tag RoundingTag = _impl::tag_of_t<rounding_integer<>>, class Narrowest = int,
            class Input = int>
    [[nodiscard]] constexpr auto make_rounding_safe_int(Input const& input) requires(!_impl::is_constant<Input>::value)
    {
        return rounding_safe_int<
                numeric_limits<Input>::digits, OverflowTag, RoundingTag,
                Narrowest>{input};
    }

    template<
            overflow_tag OverflowTag = _impl::tag_of_t<overflow_integer<>>,
            rounding_tag RoundingTag = _impl::tag_of_t<rounding_integer<>>, class Narrowest = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE InputValue = 0>
    [[nodiscard]] constexpr auto make_rounding_safe_int(constant<InputValue>)
    {
        return rounding_safe_int<
                _impl::used_digits(InputValue), OverflowTag, RoundingTag,
                Narrowest>{InputValue};
    }
}

namespace {
    using cnl::_impl::identical;
    using cnl::_impl::rep_of_t;

    namespace default_parameters {
        using cnl::_impl::rep_of_t;
        static_assert(
                std::is_same<rep_of_t<rep_of_t<rep_of_t<cnl::rounding_safe_int<1>>>>, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_multiply {
        static_assert(
                identical(
                        cnl::rounding_safe_int<6>{7} * cnl::rounding_safe_int<13>{321},
                        cnl::rounding_safe_int<19>{2247}));
    }

    namespace test_make_rounding_safe_int {
        using namespace cnl::literals;
        static_assert(
                identical(
                        cnl::make_rounding_safe_int(std::int16_t{7}), cnl::rounding_safe_int<15>{7}));
        static_assert(
                identical(cnl::make_rounding_safe_int(7_c), cnl::rounding_safe_int<3>{7}));
    }
}
