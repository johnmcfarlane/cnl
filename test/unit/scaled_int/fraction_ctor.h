
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of scaled_integer with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > cnl::width(test_int)::value).

#if !defined(CNL_TEST_FIXED_POINT_FRACTION_CTOR_H)
#define CNL_TEST_FIXED_POINT_FRACTION_CTOR_H

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/fraction.h>
#include <cnl/scaled_integer.h>

#include <algorithm>

template<cnl::_impl::binary_arithmetic_op Op, typename Lhs, typename Rhs>
struct sum_digits {
    using result_type = decltype(Op{}(std::declval<Lhs>(), std::declval<Rhs>()));
    using type = cnl::set_digits_t<
            result_type, std::max(
                                 cnl::digits_v<result_type>,
                                 cnl::digits_v<Lhs> + cnl::digits_v<Rhs>)>;
};

template<cnl::_impl::binary_arithmetic_op Op, typename Lhs, typename Rhs = Lhs>
using sum_digits_t = typename sum_digits<Op, Lhs, Rhs>::type;

template<typename Lhs = test_int, typename Rhs = Lhs>
using quot_digits_t = sum_digits_t<cnl::_impl::divide_op, Lhs, Rhs>;

// https://docs.google.com/presentation/d/1oTYA7wbqDL97mkbAe5F97uEYBwAGvSH5YzDYCbBAUVA/edit?usp=sharing
namespace test_jax2018_example {
    // pi stored as s5.10 (truncated rounding)
    constexpr auto n =
            cnl::_impl::from_rep<cnl::scaled_integer<int16, cnl::power<-10>>>(int16{3216});
#if defined(__cpp_lib_math_constants)
    static_assert(
            cnl::_impl::identical(std::numbers::pi_v<cnl::scaled_integer<int16, cnl::power<-10>>>, n));
#endif
    static_assert(
            cnl::_impl::identical(int16{3216}, cnl::_impl::to_rep(n)),
            "cnl::_impl::to_rep(cnl::scaled_integer)");
    static_assert(
            cnl::_impl::identical(3.140625, static_cast<double>(n)),
            "cnl::scaled_integer::operator double()");

    // 15.875 as s4.3
    constexpr auto d = cnl::scaled_integer<int8, cnl::power<-3>>{15.875};
    static_assert(
            cnl::_impl::identical(int8{127}, cnl::_impl::to_rep(d)), "cnl::scaled_integer::scaled_integer");

    // n / d
    constexpr auto f = make_fraction(n, d);
    static_assert(cnl::_impl::identical(cnl::fraction{n, d}, f), "cnl::fraction CTAD");
    static_assert(
            cnl::_impl::identical(
                    cnl::fraction<
                            cnl::scaled_integer<int16, cnl::power<-10>>,
                            cnl::scaled_integer<int8, cnl::power<-3>>>{n, d},
                    f),
            "cnl::make_fraction");

    // nicely-widened quotient
    constexpr auto nq = cnl::make_scaled_integer(f);
    static_assert(
            cnl::_impl::identical(cnl::make_scaled_integer(cnl::make_fraction(n, d)), nq),
            "cnl::make_scaled_integer(cnl::fraction)");

    // custom-width quotient (must be wide enough to perform widened division)
    constexpr auto cq = cnl::scaled_integer<uint16, cnl::power<-4>>{f};
    static_assert(
            cnl::_impl::identical(cnl::scaled_integer<uint16, cnl::power<-4>>{0.1875}, cq),
            "cnl::scaled_integer::scaled_integer(fraction) w.out CTAD");
}

namespace test_fraction_specific_int {
    constexpr auto third = cnl::make_fraction(test_int{1}, test_int{3});

    constexpr auto named = cnl::quotient(third.numerator, third.denominator);
    static_assert(
            cnl::_impl::identical(
                    cnl::scaled_integer<quot_digits_t<>, cnl::power<-31>>{
                            0.333333333022892475128173828125L},
                    named));

    constexpr auto deduced = cnl::make_scaled_integer(third);
    static_assert(cnl::_impl::identical(named, deduced));

    constexpr auto specific = cnl::scaled_integer<int64, cnl::power<-31>>{third};
    static_assert(
            cnl::_impl::identical(
                    cnl::scaled_integer<int64, cnl::power<-31>>{
                            0.333333333022892475128173828125L},
                    specific));
}

namespace test_fraction_specific_8bit {
    constexpr auto third = cnl::make_fraction(int8{1}, int8{3});

    constexpr auto named = cnl::quotient(third.numerator, third.denominator);
    static_assert(
            cnl::_impl::identical(
                    cnl::scaled_integer<quot_digits_t<int8>, cnl::power<-7>>{0.328125}, named));

    constexpr auto deduced = cnl::make_scaled_integer(third);
    static_assert(cnl::_impl::identical(named, deduced));

    constexpr auto specific = cnl::scaled_integer<test_int, cnl::power<-7>>{third};
    static_assert(
            cnl::_impl::identical(cnl::scaled_integer<test_int, cnl::power<-7>>{0.328125}, specific));
}

namespace test_fraction_specific_16bit {
    constexpr auto third = cnl::make_fraction(int16{1}, int16{3});

    constexpr auto named = cnl::quotient(third.numerator, third.denominator);
    static_assert(
            cnl::_impl::identical(
                    cnl::scaled_integer<quot_digits_t<int16>, cnl::power<-15>>{
                            0.33331298828125},
                    named));

    constexpr auto deduced = cnl::make_scaled_integer(third);
    static_assert(cnl::_impl::identical(named, deduced));

    constexpr auto specific = cnl::scaled_integer<uint8, cnl::power<-7>>{third};
    static_assert(
            cnl::_impl::identical(cnl::scaled_integer<uint8, cnl::power<-7>>{0.328125}, specific));
}

#endif  // CNL_TEST_FIXED_POINT_FRACTION_CTOR_H
