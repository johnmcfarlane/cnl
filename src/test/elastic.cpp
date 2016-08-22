
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests sg14::elastic type with a variety of instantiations

#include "elastic_common.h"

namespace {
    ////////////////////////////////////////////////////////////////////////////////
    // fast tests of sg14::elastic<> at its limits;
    // if something broke it may show up here first

    using sg14::_fixed_point_impl::intermediate;
    using sg14::_fixed_point_impl::result;
    using sg14::_fixed_point_impl::shift_left;
    using sg14::_fixed_point_impl::shift_right;
    using sg14::_fixed_point_impl::wide_tag;
    using sg14::_impl::divide_tag;
    using sg14::_impl::identical;
    using sg14::_impl::multiply_tag;
    using sg14::elastic_integer;
    using sg14::fixed_point;
    using sg14::set_width_t;
    using sg14::width;

    static_assert(identical(
            elastic_integer<15, int>{0x7fff}*elastic_integer<16, unsigned>{0xffff},
            elastic_integer<31, int>{0x7FFE8001}), "sg14::elastic_integer test failed");
#if defined(SG14_INT128_ENABLED)
    static_assert(identical(
            elastic_integer<31, int>{0x7fffffff}*elastic_integer<32, int>{0xffffffffu},
            elastic_integer<63, int>{0x7FFFFFFE80000001LL}), "sg14::elastic_integer test failed");
#endif

    static_assert(shift_left<1, sg14::elastic_integer<31, int>>(0) == 0, "");
    static_assert(shift_left<1, sg14::elastic_integer<32, unsigned>>(0) == 0u, "");
#if defined(SG14_INT128_ENABLED)
    static_assert(shift_left<1, sg14::elastic_integer<63, int>>(0) == 0, "");
    static_assert(shift_left<1, sg14::elastic_integer<64, unsigned>>(0) == 0u, "");
#endif

    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<15, int>, 0>, fixed_point<elastic_integer<15, int>, 0>>::type{0},
            fixed_point<elastic_integer<30, int>, -15>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<16, unsigned>, 0>, fixed_point<elastic_integer<16, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<32, unsigned>, -16>{0}), "sg14::elastic_integer test failed");
#if defined(SG14_INT128_ENABLED)
    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<31, int>, 0>, fixed_point<elastic_integer<31, int>, 0>>::type{0},
            fixed_point<elastic_integer<62, int>, -31>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<32, unsigned>, 0>, fixed_point<elastic_integer<32, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<64, unsigned>, -32>{0}), "sg14::elastic_integer test failed");
#endif

    static_assert(identical(
            intermediate<wide_tag, multiply_tag, fixed_point<elastic_integer<27, unsigned int>, -27>, fixed_point<elastic_integer<27, unsigned int>, -27>>::lhs_type{0},
            fixed_point<elastic_integer<27, unsigned int>, -27>{0}), "sg14::elastic_integer test failed");

    static_assert(width<set_width_t<elastic_integer<15, uint8_t>, 22>>::value == 22, "sg14::elastic_integer test failed");

    static_assert(identical(
            set_width_t<elastic_integer<15, uint8_t>, 22>{10000},
            elastic_integer<22, uint8_t>{10000}), "sg14::elastic_integer test failed");
}

template
struct elastic_test_with_integer_digits<-43>;

template
struct elastic_test_with_integer_digits<-31>;

template
struct elastic_test_with_integer_digits<-19>;

template
struct elastic_test_with_integer_digits<-5>;

template
struct elastic_test_with_integer_digits<-1>;

template
struct elastic_test_with_integer_digits<0>;

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<8>;

template
struct elastic_test_with_integer_digits<13>;

template
struct elastic_test_with_integer_digits<16>;

template
struct elastic_test_with_integer_digits<19>;

template
struct elastic_test_with_integer_digits<31>;
