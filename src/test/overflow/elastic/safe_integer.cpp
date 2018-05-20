
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/overflow_integer.h>

#include "../../number_test.h"

namespace cnl {
    // safe elastic integer
    template<
            int IntegerDigits,
            class OverflowTag = throwing_overflow_tag,
            class Narrowest = int>
    using safe_integer = overflow_integer<
            elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            OverflowTag>;

    template<
            class OverflowTag = throwing_overflow_tag,
            class Narrowest = int,
            class Input = int>
    safe_integer<
            numeric_limits<Input>::digits,
            OverflowTag,
            Narrowest>
    constexpr make_safe_int(Input const& input)
    {
        return input;
    }
}

namespace {
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        static_assert(
                is_same<cnl::safe_integer<1>::rep::rep, int>::value,
                "cnl::safe_integer parameter default test failed");
    }

    namespace test_numeric_limits {
        using safe_saturating_integer_2 = cnl::overflow_integer<cnl::elastic_integer<2, char>, cnl::saturated_overflow_tag>;
        static_assert(identical(
                cnl::numeric_limits<safe_saturating_integer_2>::lowest(),
                safe_saturating_integer_2{-3}), "");
        static_assert(identical(
                cnl::numeric_limits<safe_saturating_integer_2>::max(),
                safe_saturating_integer_2{3}), "");
        static_assert(cnl::numeric_limits<safe_saturating_integer_2>::lowest() < cnl::numeric_limits<safe_saturating_integer_2>::max(), "");
    }

    namespace test_comparison {
        static_assert(identical(
                cnl::convert<cnl::throwing_overflow_tag, cnl::elastic_integer<10>, int>{}(0),
                cnl::elastic_integer<10>{0}), "");
#if defined(__cpp_binary_literals)
        static_assert(cnl::safe_integer<10>(0b1010101010)==cnl::safe_integer<10>(0b1010101010), "");
#endif
    }

    namespace test_make_safe_int {
        static_assert(identical(cnl::make_safe_int(cnl::int16{7}), cnl::safe_integer<15>{7}), "");
        static_assert(identical(cnl::safe_integer<34>{0}, cnl::safe_integer<34>{0}), "");
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::safe_integer<2>{2},
                        cnl::safe_integer<1>{1}+cnl::safe_integer<1>{1}),
                "cnl::safe_integer operator+");
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::safe_integer<2>{2}-cnl::safe_integer<2>{1},
                        cnl::safe_integer<3>{1}),
                "cnl::safe_integer operator+");
    }

    namespace test_multiply {
        static_assert(identical(cnl::safe_integer<6>{55}*cnl::safe_integer<6>{4}, cnl::safe_integer<12>{220}), "cnl::safe_integer operator*");
        static_assert(identical(cnl::safe_integer<3>{7}*cnl::safe_integer<4>{10}, cnl::safe_integer<7>{70}), "cnl::safe_integer operator*");
#if defined(__clang__) || ! defined(__GNUG__)
        static_assert(identical(cnl::safe_integer<3>{3}*.25, .75), "cnl::safe_integer operator*");
#endif
    }

    namespace test_scale {
        static_assert(identical(cnl::safe_integer<3>{7}*cnl::safe_integer<4>{10}, cnl::safe_integer<7>{70}), "cnl::safe_integer operator*");
    }

    namespace test_is_composite {
        using cnl::is_composite;

        static_assert(is_composite<cnl::safe_integer<10>>::value, "is_composite<cnl::safe_integer<10>> test failed");
    }

    namespace test_digits {
        using cnl::digits;
        using cnl::set_digits_t;

        static_assert(digits<cnl::safe_integer<3>>::value>=3, "cnl::digits / cnl::set_digits test failed");
        static_assert(identical(set_digits_t<cnl::safe_integer<1>, 3>{6}, cnl::safe_integer<3>{6}), "cnl::digits / cnl::set_digits test failed");
    }

    namespace test_used_digits {
        using cnl::throwing_overflow_tag;

        static_assert(cnl::used_digits(cnl::safe_integer<1, throwing_overflow_tag, char>{0})==0, "");
        static_assert(cnl::used_digits(cnl::safe_integer<22, throwing_overflow_tag>{77})==7, "");
    }

    namespace test_leading_bits {
        using cnl::leading_bits;
        using cnl::throwing_overflow_tag;

        static_assert(leading_bits(cnl::safe_integer<1, throwing_overflow_tag, char>{0}) == 1, "leading_bits(cnl::safe_integer)");
        static_assert(leading_bits(cnl::safe_integer<22, throwing_overflow_tag>{77}) == 15, "leading_bits(cnl::safe_integer)");
    }

    namespace test_shift {
        static_assert(identical(cnl::safe_integer<3>{2}, cnl::_impl::shift<1>(cnl::safe_integer<2>{1})),
                "cnl::shift<..., cnl::safe_integer<>>");

        static_assert(identical(cnl::safe_integer<3>{2}, cnl::_impl::shift<1>(cnl::safe_integer<2>{1})),
                "cnl::shift<..., cnl::safe_integer<>>");
    }
}

// given a rounding tag, invokes number_test_suite for integers of all built-in types
template<int NumDigits, class OverflowTag>
struct test_safe_int {
    template<class Rep>
    using test_subject = cnl::safe_integer<NumDigits, OverflowTag, Rep>;

    constexpr static number_test_by_rep<test_subject> instance{};
};

template struct test_safe_int<1, cnl::throwing_overflow_tag>;
template struct test_safe_int<5, cnl::throwing_overflow_tag>;
template struct test_safe_int<21, cnl::throwing_overflow_tag>;

#if defined(__clang__) || ! defined(__GNUG__) || (__GNUG__ > 6)
template struct test_safe_int<2, cnl::saturated_overflow_tag>;
template struct test_safe_int<8, cnl::saturated_overflow_tag>;
template struct test_safe_int<34, cnl::saturated_overflow_tag>;
#endif

template struct test_safe_int<3, cnl::native_overflow_tag>;
template struct test_safe_int<13, cnl::native_overflow_tag>;
template struct test_safe_int<55, cnl::native_overflow_tag>;
