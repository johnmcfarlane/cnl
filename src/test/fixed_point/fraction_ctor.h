
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of fixed_point with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > cnl::width(test_int)::value).

#ifndef CNL_TEST_FIXED_POINT_FRACTION_CTOR_H
#define CNL_TEST_FIXED_POINT_FRACTION_CTOR_H

#include <cnl/fixed_point.h>
#include <cnl/fraction.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::_impl::identical;

    // https://docs.google.com/presentation/d/1oTYA7wbqDL97mkbAe5F97uEYBwAGvSH5YzDYCbBAUVA/edit?usp=sharing
    namespace test_jax2018_example {
        // pi stored as s5.10 (truncated rounding)
        constexpr auto n = cnl::from_rep<cnl::fixed_point<int16, -10>>()(int16{3216});
#if defined(__cpp_inline_variables)
        static_assert(identical(cnl::pi<cnl::fixed_point<int16, -10>>, n));
#endif
        static_assert(identical(int16{3216}, cnl::_impl::to_rep(n)), "cnl::_impl::to_rep(cnl::fixed_point)");
        static_assert(identical(3.140625, static_cast<double>(n)), "cnl::fixed_point::operator double()");

        // 15.875 as s4.3
        constexpr auto d = cnl::fixed_point<int8, -3>{15.875};
        static_assert(identical(int8{127}, cnl::_impl::to_rep(d)), "cnl::fixed_point::fixed_point");

        // n / d
        constexpr auto f = make_fraction(n, d);
#if defined(__cpp_deduction_guides)
        static_assert(identical(cnl::fraction{n, d}, f), "cnl::fraction CTAD");
#endif
        static_assert(identical(
                cnl::fraction<cnl::fixed_point<int16, -10>, cnl::fixed_point<int8, -3>>{n, d}, f),
                "cnl::make_fraction");

        // nicely-widened quotient
        constexpr auto nq = cnl::make_fixed_point(f);
        static_assert(identical(cnl::quotient(n, d), nq), "cnl::make_fixed_point(cnl::fraction)");
#if defined(__cpp_deduction_guides)
        static_assert(
                identical(nq, cnl::fixed_point{f}),
                "cnl::fixed_point::fixed_point(fraction) w. CTAD");
#endif

        // custom-width quotient (must be wide enough to perform widened division)
        constexpr auto cq = cnl::fixed_point<uint16, -4>{f};
        static_assert(
                identical(cnl::fixed_point<uint16, -4>{0.1875}, cq),
                "cnl::fixed_point::fixed_point(fraction) w.out CTAD");
    }

#if !defined(TEST_WIDE_INTEGER)
    namespace test_fraction_deduced {
        constexpr auto third = cnl::make_fraction(test_int{1}, test_int{3});

        constexpr auto named = cnl::quotient(third.numerator, third.denominator);
        static_assert(identical(fixed_point<int64, -31>{0.333333333022892475128173828125L}, named), "");

#if defined(__cpp_deduction_guides)
        constexpr auto deduced = cnl::fixed_point{third};
        static_assert(identical(named, deduced));
#endif
    }

    namespace test_fraction_specific_int {
        constexpr auto third = cnl::make_fraction(test_int{1}, test_int{3});

        constexpr auto named = cnl::quotient(third.numerator, third.denominator);
        static_assert(identical(cnl::fixed_point<int64, -31>{0.333333333022892475128173828125L}, named), "");

#if defined(__cpp_deduction_guides)
        constexpr auto deduced = cnl::fixed_point{third};
        static_assert(identical(named, deduced));
#endif

        constexpr auto specific = cnl::fixed_point<int64, -31>{third};
        static_assert(identical(cnl::fixed_point<int64, -31>{0.333333333022892475128173828125L}, specific), "");
    }

    namespace test_fraction_specific_8bit {
        constexpr auto third = cnl::make_fraction(int8{1}, int8{3});

        constexpr auto named = cnl::quotient(third.numerator, third.denominator);
        static_assert(identical(cnl::fixed_point<test_int, -7>{0.328125}, named), "");

#if defined(__cpp_deduction_guides)
        constexpr auto deduced = cnl::fixed_point{third};
        static_assert(identical(named, deduced));
#endif

        constexpr auto specific = cnl::fixed_point<test_int, -7>{third};
        static_assert(identical(cnl::fixed_point<test_int, -7>{0.328125}, specific), "");
    }

    namespace test_fraction_specific_16bit {
        constexpr auto third = cnl::make_fraction(int16{1}, int16{3});

        constexpr auto named = cnl::quotient(third.numerator, third.denominator);
        static_assert(identical(cnl::fixed_point<test_int, -15>{0.33331298828125}, named), "");

#if defined(__cpp_deduction_guides)
        constexpr auto deduced = cnl::fixed_point{third};
        static_assert(identical(named, deduced));
#endif

        constexpr auto specific = cnl::fixed_point<uint8, -7>{third};
        static_assert(identical(cnl::fixed_point<uint8, -7>{0.328125}, specific), "");
    }
#endif  // !defined(TEST_WIDE_INTEGER)
}

#endif //CNL_TEST_FIXED_POINT_FRACTION_CTOR_H
