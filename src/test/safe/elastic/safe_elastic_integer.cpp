
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/safe_integer.h>

#include "../../number_test.h"

namespace cnl {
    // safe elastic integer
    template<
            int IntegerDigits,
            class OverflowTag = throwing_overflow_tag,
            class Narrowest = int>
    using safe_elastic_integer = safe_integer<
            elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            OverflowTag>;

    template<
            class OverflowTag = throwing_overflow_tag,
            class Narrowest = int,
            class Input = int>
    safe_elastic_integer<
            numeric_limits<Input>::digits,
            OverflowTag,
            Narrowest>
    constexpr make_safe_elastic(Input const& input)
    {
        return input;
    }
}

namespace {
    using cnl::safe_elastic_integer;
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        static_assert(
                is_same<safe_elastic_integer<1>::rep::rep, int>::value,
                "cnl::safe_elastic_integer parameter default test failed");
    }

    namespace test_numeric_limits {
        using safe_saturating_integer_2 = cnl::safe_integer<cnl::elastic_integer<2, char>, cnl::saturated_overflow_tag>;
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
                cnl::convert<cnl::elastic_integer<10>>(cnl::throwing_overflow, 0),
                cnl::elastic_integer<10>{0}), "");
        static_assert(safe_elastic_integer<10>(0b1010101010)==safe_elastic_integer<10>(0b1010101010), "");
    }

    namespace test_make_safe_elastic {
        static_assert(identical(cnl::make_safe_elastic(std::int16_t{7}), safe_elastic_integer<15>{7}), "");
        static_assert(identical(safe_elastic_integer<34>{0}, safe_elastic_integer<34>{0}), "");
    }

    namespace test_add {
        static_assert(
                identical(
                        safe_elastic_integer<2>{2},
                        safe_elastic_integer<1>{1}+safe_elastic_integer<1>{1}),
                "safe_elastic_integer operator+");
    }

    namespace test_add {
        static_assert(
                identical(
                        safe_elastic_integer<2>{2}-safe_elastic_integer<2>{1},
                        safe_elastic_integer<3>{1}),
                "safe_elastic_integer operator+");
    }

    namespace test_multiply {
        static_assert(identical(safe_elastic_integer<6>{55}*safe_elastic_integer<6>{4}, safe_elastic_integer<12>{220}), "safe_elastic_integer operator*");
        static_assert(identical(safe_elastic_integer<3>{7}*safe_elastic_integer<4>{10}, safe_elastic_integer<7>{70}), "safe_elastic_integer operator*");
#if defined(__clang__) || ! defined(__GNUG__)
        static_assert(identical(safe_elastic_integer<3>{3}*.25, .75), "safe_elastic_integer operator*");
#endif
    }

    namespace test_scale {
        static_assert(identical(safe_elastic_integer<3>{7}*safe_elastic_integer<4>{10}, safe_elastic_integer<7>{70}), "safe_elastic_integer operator*");
    }

    namespace test_is_composite {
        using cnl::is_composite;

        static_assert(is_composite<safe_elastic_integer<10>>::value, "is_composite<safe_elastic_integer<10>> test failed");
    }

    namespace test_digits {
        using cnl::digits;
        using cnl::set_digits_t;

        static_assert(digits<safe_elastic_integer<3>>::value>=3, "cnl::digits / cnl::set_digits test failed");
        static_assert(identical(set_digits_t<safe_elastic_integer<1>, 3>{6}, safe_elastic_integer<3>{6}), "cnl::digits / cnl::set_digits test failed");
    }

    namespace test_used_bits {
        using cnl::used_bits;
        using cnl::throwing_overflow_tag;

        static_assert(used_bits(safe_elastic_integer<1, throwing_overflow_tag, char>{0}) == 0, "used_bits(safe_elastic_integer)");
        static_assert(used_bits(safe_elastic_integer<22, throwing_overflow_tag>{77}) == 7, "used_bits(safe_elastic_integer)");
    }

    namespace test_leading_bits {
        using cnl::leading_bits;
        using cnl::throwing_overflow_tag;

        static_assert(leading_bits(safe_elastic_integer<1, throwing_overflow_tag, char>{0}) == 1, "leading_bits(safe_elastic_integer)");
        static_assert(leading_bits(safe_elastic_integer<22, throwing_overflow_tag>{77}) == 15, "leading_bits(safe_elastic_integer)");
    }
}

// given a rounding tag, invokes number_test_suite for precise_integers of all built-in types
template<int NumDigits, class OverflowTag>
struct test_safe_elastic_integer {
    template<class Rep>
    using test_subject = safe_elastic_integer<NumDigits, OverflowTag, Rep>;

    constexpr static number_test_by_rep<test_subject> instance{};
};

template struct test_safe_elastic_integer<1, cnl::throwing_overflow_tag>;
template struct test_safe_elastic_integer<5, cnl::throwing_overflow_tag>;
template struct test_safe_elastic_integer<21, cnl::throwing_overflow_tag>;

#if defined(__clang__) || ! defined(__GNUG__) || (__GNUG__ > 6)
template struct test_safe_elastic_integer<2, cnl::saturated_overflow_tag>;
template struct test_safe_elastic_integer<8, cnl::saturated_overflow_tag>;
template struct test_safe_elastic_integer<34, cnl::saturated_overflow_tag>;
#endif

template struct test_safe_elastic_integer<3, cnl::native_overflow_tag>;
template struct test_safe_elastic_integer<13, cnl::native_overflow_tag>;
template struct test_safe_elastic_integer<55, cnl::native_overflow_tag>;
