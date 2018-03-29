
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of fixed_point with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > cnl::width(test_int)::value).

#ifndef CNL_FRACTIONAL_CTOR_H
#define CNL_FRACTIONAL_CTOR_H

#include <cnl/fixed_point.h>

namespace {
    using cnl::_impl::identical;

    // https://docs.google.com/presentation/d/1oTYA7wbqDL97mkbAe5F97uEYBwAGvSH5YzDYCbBAUVA/edit?usp=sharing
    namespace test_jax2018_example {
        // pi stored as s5.10 (truncated rounding)
        constexpr auto n = cnl::from_rep<cnl::fixed_point<int16, -10>>()(int16{3216});
#if defined(__cpp_inline_variables)
        static_assert(identical(cnl::math_constants::pi<cnl::fixed_point<int16, -10>>, n));
#endif
        static_assert(identical(int16{3216}, to_rep(n)), "cnl::to_rep(cnl::fixed_point)");
        static_assert(identical(3.140625, static_cast<double>(n)), "cnl::fixed_point::operator double()");

        // 15.875 as s4.3
        constexpr auto d = cnl::fixed_point<int8, -3>{15.875};
        static_assert(identical(int8{127}, to_rep(d)), "cnl::fixed_point::fixed_point");

        // n / d
        constexpr auto f = make_fractional(n, d);
#if defined(__cpp_deduction_guides)
        static_assert(identical(cnl::fractional{n, d}, f), "cnl::fractional CTAD");
#endif
        static_assert(identical(
                cnl::fractional<cnl::fixed_point<int16, -10>, cnl::fixed_point<int8, -3>>{n, d}, f),
                "cnl::make_fractional");

        // nicely-widened quotient
        constexpr auto nq = cnl::make_fixed_point(f);
        static_assert(identical(cnl::divide(n, d), nq), "cnl::make_fixed_point(cnl::fractional)");
#if defined(__cpp_deduction_guides)
        static_assert(
                identical(nq, cnl::fixed_point{f}),
                "cnl::fixed_point::fixed_point(fractional) w. CTAD");
#endif

        // custom-width quotient
        constexpr auto cq = cnl::fixed_point<uint8, -4>{f};
        static_assert(
                identical(cnl::fixed_point<uint8, -4>{0.1875}, cq),
                "cnl::fixed_point::fixed_point(fractional) w.out CTAD");
    }
}

#endif //CNL_FRACTIONAL_CTOR_H
